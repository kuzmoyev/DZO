#include "utility.h"


namespace utility {
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

    namespace {
        bool onBoundary(int x, int y, const QImage& mask) {
            return (x == 0 || x == mask.width() - 1 || y == 0 || y == mask.height() - 1) ||
                   (mask.pixel(x - 1, y) != WHITE || mask.pixel(x + 1, y) != WHITE) ||
                   (mask.pixel(x, y - 1) != WHITE || mask.pixel(x, y + 1) != WHITE);
        }
    }


    ulong generateCrsMatrix(
            const QImage& target,
            const QImage& source,
            const QImage& mask,
            std::vector<int>& ptr,
            std::vector<int>& col,
            std::vector<int>& matrix_values,
            std::vector<int>& rhsR,
            std::vector<int>& rhsG,
            std::vector<int>& rhsB,
            std::vector<std::pair<int, int>>& pixels
    ) {
        std::map<std::pair<int, int>, int> pixelsColumn;
        calcMaskPixels(mask, pixels, pixelsColumn);

        size_t n = pixels.size();

        ptr.reserve(n + 1);
        ptr.push_back(0);

        col.reserve(n * 5);
        matrix_values.reserve(n * 5);

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
                matrix_values.push_back(1);

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
                        matrix_values.push_back(1);
                    }

                }

                col.push_back(k);
                matrix_values.push_back(-4);

                rhsR[k] = rhsValR;
                rhsG[k] = rhsValG;
                rhsB[k] = rhsValB;
            }

            ptr.push_back(int(col.size()));
            k++;
        }

        return n;
    }
}
