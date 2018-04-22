#include <vector>

#include "simple_poisson_solver/util.h"
#include "simple_poisson_solver/matrix.h"

using namespace std;

namespace gauss_seidel_solver {


    const size_t TOO_MANY_PIXELS = 10 * 10;
    const uint MAX_OPERATIONS = 2 << 27;
    const uint RESIZE_FACTOR = 2;


    using namespace std::chrono;
    auto log_start = high_resolution_clock::now();

    auto qDebugWithTs() {
        auto delta = duration_cast<microseconds>(high_resolution_clock::now() - log_start).count();
        return qDebug() << "[" << (delta / 1000.) << " ms] ";
    }


    template <typename VALUE_TYPE, typename SOLUTION_TYPE>
    void gauss_seidel_solve(
            const std::vector<int>& ptr,
            const std::vector<int>& col,
            const std::vector<VALUE_TYPE>& matrix_values,
            const std::vector<int>& rhs,
            std::vector<SOLUTION_TYPE>& result,
            int iterations
    ) {
        vector<SOLUTION_TYPE> tmp_result(result.size());

        for (int it = 0; it < iterations; it++) {
            for (size_t i = 0; i < result.size(); i++) {
                SOLUTION_TYPE x = 0;
                VALUE_TYPE coef = 0;

                for (int s = ptr[i], e = ptr[i + 1]; s != e; s++) {
                    if (col[s] != int(i)) {
                        x -= matrix_values[s] * result[col[s]];
                    } else {
                        x += rhs[i];
                        coef = matrix_values[s];
                    }
                }
                tmp_result[i] = x / coef;
            }
            swap(result, tmp_result);
        }
    }

    QImage solve(
            const QImage& target,
            const QImage& source,
            const QImage& mask,
            bool reset_log = true
    ) {
        if (reset_log)
            log_start = high_resolution_clock::now();

        std::vector<std::pair<int, int>> pixels;
        std::vector<double> result_r;
        std::vector<double> result_g;
        std::vector<double> result_b;

        std::vector<int> ptr;
        std::vector<int> col;
        std::vector<int> matrix_values;

        std::vector<int> rhs_r;
        std::vector<int> rhs_g;
        std::vector<int> rhs_b;

        ulong n = matrix::generateMatrix(target, source, mask, ptr, col, matrix_values, rhs_r, rhs_g, rhs_b, pixels);
        qDebugWithTs() << "Matrix ready";


        result_r.resize(n, 127);
        result_g.resize(n, 127);
        result_b.resize(n, 127);

        if (pixels.size() >= TOO_MANY_PIXELS) {
            // Precompute results on a smaller image
            auto tmp_target = target.scaled(target.width() / RESIZE_FACTOR, target.height() / RESIZE_FACTOR);
            auto tmp_source = source.scaled(source.width() / RESIZE_FACTOR, source.height() / RESIZE_FACTOR);
            auto tmp_mask = mask.scaled(mask.width() / RESIZE_FACTOR, mask.height() / RESIZE_FACTOR);

            auto tmp_result = solve(tmp_target, tmp_source, tmp_mask, false).scaled(target.width(), target.height());

            for (size_t i = 0; i < pixels.size(); i++) {
                int x, y;
                boost::tie(x, y) = pixels[i];
                auto color = tmp_result.pixelColor(x, y);

                result_r[i] = color.red();
                result_g[i] = color.green();
                result_b[i] = color.blue();
            }
            qDebugWithTs() << "Precomputed on:" << target.width() / 2 << "x" << target.height() / 2;
        }

        gauss_seidel_solve(ptr, col, matrix_values, rhs_r, result_r, MAX_OPERATIONS / int(pixels.size()));
        gauss_seidel_solve(ptr, col, matrix_values, rhs_g, result_g, MAX_OPERATIONS / int(pixels.size()));
        gauss_seidel_solve(ptr, col, matrix_values, rhs_b, result_b, MAX_OPERATIONS / int(pixels.size()));

        qDebugWithTs() << "Solved in" << MAX_OPERATIONS / int(pixels.size()) << "iterations";


        QImage result(target);
        for (size_t i = 0; i < pixels.size(); i++) {
            int r = bound(int(result_r[i]));
            int g = bound(int(result_g[i]));
            int b = bound(int(result_b[i]));

            QColor c(r, g, b);
            int x = pixels[i].first;
            int y = pixels[i].second;
            result.setPixel(x, y, c.rgb());
        }

        return result;
    }


}