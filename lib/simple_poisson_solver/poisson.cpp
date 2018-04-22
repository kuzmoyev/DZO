#include "simple_poisson_solver/poisson.h"

namespace simple_solver {

    enum BlendingMode {
        OVER, ADDITIVE, MAXIMUM, DIRECTIONAL
    };


    QImage poisson(QImage target, QImage source, const QImage& mask) {
        return gauss_seidel_solver::solve(target, source, mask);
    }
}
