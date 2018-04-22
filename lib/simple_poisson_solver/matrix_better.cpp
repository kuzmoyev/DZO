#include "simple_poisson_solver/matrix_better.h"

namespace matrix_better {
    bool onBoundary(int x, int y, const QImage& mask) {
        return (x == 0 || x == mask.width() - 1 || y == 0 || y == mask.height() - 1) ||
               (mask.pixel(x - 1, y) != WHITE || mask.pixel(x + 1, y) != WHITE) ||
               (mask.pixel(x, y - 1) != WHITE || mask.pixel(x, y + 1) != WHITE);
    }

    uint calcMaskPixels(const QImage& mask,
                        std::vector<std::pair<int, int>>& pixels,
                        std::vector<std::vector<int>>& pixelsColumn) {
        uint known = 0;
        uint unknown = 0;
        for (int x = 0; x < mask.width(); x++) {
            for (int y = 0; y < mask.height(); y++) {
                if (mask.pixel(x, y) == WHITE) {
                    if (onBoundary(x, y, mask)) {
                        known++;
                    } else {
                        unknown++;
                    }
                }
            }
        }

        pixelsColumn.resize((size_t) mask.width(), std::vector<int>((size_t) mask.height()));
        pixels.resize(known + unknown);

        int known_idx = unknown;
        int unknown_idx = 0;
        for (int x = 0; x < mask.width(); x++) {
            for (int y = 0; y < mask.height(); y++) {
                if (mask.pixel(x, y) == WHITE) {
                    if (onBoundary(x, y, mask)) {
                        pixelsColumn[x][y] = known_idx;
                        pixels[known_idx] = std::make_pair(x, y);
                        known_idx++;
                    } else {
                        pixelsColumn[x][y] = unknown_idx;
                        pixels[unknown_idx] = std::make_pair(x, y);
                        unknown_idx++;
                    }
                }
            }
        }
        return unknown;
    }


    void generateMatrix(
            const QImage& target,
            const QImage& source,
            const QImage& mask,
            std::vector<std::array<int, 4>>& mat,
            std::vector<float>& sol_r,
            std::vector<float>& sol_g,
            std::vector<float>& sol_b,
            std::vector<int>& rhs_r,
            std::vector<int>& rhs_g,
            std::vector<int>& rhs_b,
            std::vector<std::pair<int, int>>& pixels
    ) {
        std::vector<std::vector<int>> pixelsColumn;
        uint unknown = calcMaskPixels(mask, pixels, pixelsColumn);

        auto zero_pixel_index = (int) pixels.size();
        mat.resize(unknown, {zero_pixel_index, zero_pixel_index, zero_pixel_index, zero_pixel_index});

        sol_r.resize(pixels.size() + 1, 127);
        sol_g.resize(pixels.size() + 1, 127);
        sol_b.resize(pixels.size() + 1, 127);

        sol_r[zero_pixel_index] = 0;
        sol_g[zero_pixel_index] = 0;
        sol_b[zero_pixel_index] = 0;


        rhs_r.resize(unknown);
        rhs_g.resize(unknown);
        rhs_b.resize(unknown);


        int x;
        int y;

        // Fill known values
        for (uint p = unknown; p < pixels.size(); p++) {
            std::tie(x, y) = pixels[p];
            auto tcolor = target.pixelColor(x, y);

            sol_r[p] = tcolor.red();
            sol_g[p] = tcolor.green();
            sol_b[p] = tcolor.blue();
        }

        for (uint p = 0; p < unknown; p++) {
            std::tie(x, y) = pixels[p];

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

            int dx, dy, n = 0;
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
                    mat[p][n] = pixelsColumn[nx][ny];
                }

                n++;
            }
            rhs_r[p] = rhsValR;
            rhs_g[p] = rhsValG;
            rhs_b[p] = rhsValB;
        }
    }
}