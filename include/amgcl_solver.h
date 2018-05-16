#pragma once

#include <QtGui/QImage>

/**
 * @brief Adapter to amgcl library, that uses iterative BiCGSTUB solver
 */
namespace amgcl_solver {
	QImage poisson(const QImage& target, const QImage& source, const QImage& mask, unsigned iterations);
}
