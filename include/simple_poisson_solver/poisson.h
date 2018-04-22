#pragma once


#include <QtGui/QBitmap>

#include "simple_poisson_solver/poisson.h"
#include "simple_poisson_solver/gaussian_blur.h"
#include "simple_poisson_solver/amgcl_solver.h"
#include "simple_poisson_solver/gauss_seidel_solver_better.h"
#include "simple_poisson_solver/util.h"
#include "simple_poisson_solver/matrix.h"


namespace simple_solver {
    QImage poisson(QImage target, QImage source, const QImage& mask);

}