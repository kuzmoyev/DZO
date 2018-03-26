#include <iostream>
#include <utility>

#include "simple_poisson_solver/poisson.h"

using namespace std;

namespace simple_solver {

    QRgb WHITE = 0xffffffff;

    void getMaskPixels(const QImage& img, vector<pair<int, int>>& pixels, map<pair<int, int>, int>& pixelsColumn) {
        for (int x = 0; x < img.width(); x++) {
            for (int y = 0; y < img.height(); y++) {
                if (img.pixel(x, y) == WHITE) {
                    pixelsColumn[make_pair(x, y)] = int(pixels.size());
                    pixels.emplace_back(x, y);
                }
            }
        }
    }

    bool onBoundary(int x, int y, const QImage& mask) {
        return (x == 0 || x == mask.width() - 1 || y == 0 || y == mask.height() - 1) ||
               (mask.pixel(x - 1, y) == WHITE || mask.pixel(x + 1, y) == WHITE) ||
               (mask.pixel(x, y - 1) == WHITE || mask.pixel(x, y + 1) == WHITE);
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
            vector<pair<int, int>>& pixels
    ) {
        map<pair<int, int>, int> pixelsColumn;
        getMaskPixels(mask, pixels, pixelsColumn);


        ulong n = pixels.size();

        ptr.reserve(n + 1);
        ptr.push_back(0);

        col.reserve(n * 5); // We use 5-point stencil, so the matrix

        valR.reserve(n * 5); // will have at most n * 5 nonzero elements.
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


                constexpr pair<int, int> neighbours[]{
                        {-1, 0},
                        {1,  0},
                        {0,  1},
                        {0,  -1}
                };
                int dx, dy;
                for (auto d : neighbours) {
                    tie(dx, dy) = d;
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
                        col.push_back(pixelsColumn[make_pair(x + dx, y + dy)]);
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


    QImage poisson(const QImage target, const QImage source, const QImage mask) {
        std::vector<int> ptr;
        std::vector<int> col;

        std::vector<int> valR;
        std::vector<int> valG;
        std::vector<int> valB;

        std::vector<int> rhsR;
        std::vector<int> rhsG;
        std::vector<int> rhsB;

        vector<pair<int, int>> pixels;

        poisson(target, source, mask, ptr, col, valR, valG, valB, rhsR, rhsG, rhsB, pixels);


        return source;
    }
}
