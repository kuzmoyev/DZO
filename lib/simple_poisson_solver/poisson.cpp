#include "simple_poisson_solver/poisson.h"

namespace simple_solver {

    enum BlendingMode {
        OVER, ADDITIVE, MAXIMUM, DIRECTIONAL
    };


    QImage poisson(QImage target, QImage source, const QImage& mask) {
        std::vector<std::pair<int, int>> pixels;
        std::vector<double> result_r;
        std::vector<double> result_g;
        std::vector<double> result_b;

        gauss_seidel_solver::solve(target, source, mask, pixels, result_r, result_g, result_b);

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

        return blur::gaussBlur(result);
    }
}
