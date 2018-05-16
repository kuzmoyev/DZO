#pragma once

#include <QtGui/QImage>

namespace amgcl_solver {
	QImage poisson(const QImage& target, const QImage& source, const QImage& mask, unsigned iterations);
}
