#pragma once

#include "util.h"


// Assembles matrix for Poisson's equation with boundary condition on the masks boundary.
// The matrix is returned in the CRS components ptr, col, and val.
ulong generateMatrix(
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
);