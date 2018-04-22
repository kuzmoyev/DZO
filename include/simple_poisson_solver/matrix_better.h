#pragma once

#include "util.h"

namespace matrix_better {
    // Assembles matrix for Poisson's equation with boundary condition on the masks boundary.
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
    );
}