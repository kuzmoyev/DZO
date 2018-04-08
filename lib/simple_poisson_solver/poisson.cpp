#include <tuple>
#include <utility>

#include <amgcl/make_solver.hpp>
#include <amgcl/solver/bicgstab.hpp>
#include <amgcl/amg.hpp>
#include <amgcl/coarsening/smoothed_aggregation.hpp>
#include <amgcl/relaxation/spai0.hpp>
#include <amgcl/adapter/crs_tuple.hpp>

#include <QDebug>

#include "simple_poisson_solver/poisson.h"


namespace simple_solver {
    QRgb WHITE = 0xffffffff;

    typedef amgcl::backend::builtin<double> Backend;
    typedef amgcl::make_solver<
            // Use AMG as preconditioner:
            amgcl::amg<
                    Backend,
                    amgcl::coarsening::smoothed_aggregation,
                    amgcl::relaxation::spai0
            >,
            // And BiCGStab as iterative solver:
            amgcl::solver::bicgstab<Backend>
    > PoissonSolver;

    template <typename T>
    T clip(T x, T min, T max) {
    	return std::max(std::min(x, max), min);
    }

    void calcMaskPixels(const QImage& img,
						std::vector<std::pair<int, int>>& pixels,
						std::map<std::pair<int, int>, int>& pixelsColumn) {
        for (int x = 0; x < img.width(); x++) {
            for (int y = 0; y < img.height(); y++) {
                if (img.pixel(x, y) == WHITE) {
                    pixelsColumn[std::make_pair(x, y)] = int(pixels.size());
                    pixels.emplace_back(x, y);
                }
            }
        }
    }

    bool onBoundary(int x, int y, const QImage& mask) {
        return (x == 0 || x == mask.width() - 1 || y == 0 || y == mask.height() - 1) ||
               (mask.pixel(x - 1, y) != WHITE || mask.pixel(x + 1, y) != WHITE) ||
               (mask.pixel(x, y - 1) != WHITE || mask.pixel(x, y + 1) != WHITE);
    }

    int getterR(const QColor& c) {
        return c.red();
    }

    int getterG(const QColor& c) {
        return c.green();
    }

    int getterB(const QColor& c) {
        return c.blue();
    }

    template <typename ColorGetter>
    int getGradientedColor(int x, int y,
						   const QImage& source, const QImage& target,
						   ColorGetter colorGetter) {
        int s = colorGetter(source.pixelColor(x, y));
        int sxp = colorGetter(source.pixelColor(x + 1, y));
        int sxm = colorGetter(source.pixelColor(x - 1, y));
        int syp = colorGetter(source.pixelColor(x, y + 1));
        int sym = colorGetter(source.pixelColor(x, y - 1));

        int t = colorGetter(target.pixelColor(x, y));
        int txp = colorGetter(target.pixelColor(x + 1, y));
        int txm = colorGetter(target.pixelColor(x - 1, y));
        int typ = colorGetter(target.pixelColor(x, y + 1));
        int tym = colorGetter(target.pixelColor(x, y - 1));

        int sdx = sxp + sxm - 2 * s;
        int sdy = syp + sym - 2 * s;

        int tdx = txp + txm - 2 * s;
        int tdy = typ + tym - 2 * s;

        if (sdx * sdx + sdy * sdy > tdx * tdx + tdy * tdy)
            return s;
        else
            return t;
    }


    // Assembles matrix for Poisson's equation with boundary condition on the masks boundary.
    // The matrix is returned in the CRS components ptr, col, and val.
    ulong poisson(
            const QImage& target,
            const QImage& source,
            const QImage& mask,
            std::vector<int>& ptr,
            std::vector<int>& col,
            std::vector<int>& valR,
            std::vector<int>& valG,
            std::vector<int>& valB,
            std::vector<int>& rhsR,
            std::vector<int>& rhsG,
            std::vector<int>& rhsB,
			std::vector<std::pair<int, int>>& pixels
    ) {
		std::map<std::pair<int, int>, int> pixelsColumn;
        calcMaskPixels(mask, pixels, pixelsColumn);

        ulong n = pixels.size();

        ptr.reserve(n + 1);
        ptr.push_back(0);

        col.reserve(n * 5);

        valR.reserve(n * 5);
        valG.reserve(n * 5);
        valB.reserve(n * 5);

        rhsR.resize(n);
        rhsG.resize(n);
        rhsB.resize(n);

        int k = 0;
        for (auto& p : pixels) {
            int x = p.first;
            int y = p.second;

            if (onBoundary(x, y, mask)) {
                // Boundary point. Use Dirichlet condition.
                col.push_back(k);
                valR.push_back(1);
                valG.push_back(1);
                valB.push_back(1);

                rhsR[k] = target.pixelColor(x, y).red();
                rhsG[k] = target.pixelColor(x, y).green();
                rhsB[k] = target.pixelColor(x, y).blue();
            } else {
                // Interior point. Use 5-point finite difference stencil.

                auto scolor = source.pixelColor(x, y);

                int rhsValR = -4 * scolor.red();
                int rhsValG = -4 * scolor.green();
                int rhsValB = -4 * scolor.blue();


                constexpr std::pair<int, int> neighbours[]{
                        {-1, 0},
                        {1,  0},
                        {0,  1},
                        {0,  -1}
                };
                int dx, dy;
                for (auto d : neighbours) {
					std::tie(dx, dy) = d;
                    int nx = x + dx;
                    int ny = y + dy;

                    // adding values from source to rhs
                    scolor = source.pixelColor(nx, ny);
                    rhsValR += scolor.red();
                    rhsValG += scolor.green();
                    rhsValB += scolor.blue();

                    if (onBoundary(nx, ny, mask)) {
                        // subtracting known target values on boundary from rhs
                        auto tcolor = target.pixelColor(nx, ny);
                        rhsValR -= tcolor.red();
                        rhsValG -= tcolor.green();
                        rhsValB -= tcolor.blue();
                    } else {
                        // adding value to sparse matrix
                        col.push_back(pixelsColumn[std::make_pair(nx, ny)]);
                        valR.push_back(1);
                        valG.push_back(1);
                        valB.push_back(1);
                    }

                }

                col.push_back(k);
                valR.push_back(-4);
                valG.push_back(-4);
                valB.push_back(-4);

                rhsR[k] = rhsValR;
                rhsG[k] = rhsValG;
                rhsB[k] = rhsValB;
            }

            ptr.push_back(int(col.size()));
            k++;
        }

        return n;
    }

    QImage poisson(const QImage& target, const QImage& source, const QImage& mask) {
		std::vector<int> ptr;
		std::vector<int> col;

		std::vector<int> valR;
		std::vector<int> valG;
		std::vector<int> valB;

		std::vector<int> rhsR;
		std::vector<int> rhsG;
		std::vector<int> rhsB;

		std::vector<std::pair<int, int>> pixels;

        ulong n = poisson(target, source, mask, ptr, col, valR, valG, valB, rhsR, rhsG, rhsB, pixels);

        qDebug() << "Built matrix";

        PoissonSolver::params prm;
        prm.solver.maxiter = 2;

        PoissonSolver solveR(boost::tie(n, ptr, col, valR), prm);
        PoissonSolver solveG(boost::tie(n, ptr, col, valG), prm);
        PoissonSolver solveB(boost::tie(n, ptr, col, valB), prm);

        std::vector<double> R(n, 0);
		std::vector<double> G(n, 0);
		std::vector<double> B(n, 0);

		int iters;
		double error;

        boost::tie(iters, error) = solveR(rhsR, R);
        qDebug() << "R: error-" << error << ", iters-" << iters;

        boost::tie(iters, error) = solveG(rhsG, G);
        qDebug() << "G: error-" << error << ", iters-" << iters;

        boost::tie(iters, error) = solveB(rhsB, B);
        qDebug() << "B: error-" << error << ", iters-" << iters;


        QImage result = target.copy();

        for (uint i = 0; i < pixels.size(); i++) {
            int r = clip(int(R[i]), 255, 0);
            int g = clip(int(G[i]), 255, 0);
            int b = clip(int(B[i]), 255, 0);

            QColor c(r, g, b);
            int x = pixels[i].first;
            int y = pixels[i].second;
            result.setPixel(x, y, c.rgb());
        }

        return result;
    }
}
