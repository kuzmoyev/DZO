#pragma once

#include <vector>
#include <utility>

#include <QDebug>
#include <QtGui/QBitmap>
#include <QtGui/QImage>
#include <QtCore/QRect>
#include <QtGui/QColor>

namespace utility {
	const QRgb WHITE = 0xffffffff;

	template <typename T>
	T bound(T x, T min = 0, T max = 255) {
		return std::max(std::min(x, max), min);
	}


	QRect rectFrom2Points(const QPoint& a, const QPoint& b);

	QColor colorFromDirection(float delta_x, float delta_y);

	std::vector<std::vector<double>> getGaussianKernel(size_t size);

	QImage gaussBlur(const QImage& img, int kernel = 15);

	// Assembles matrix for Poisson's equation with boundary condition on the masks boundary.
	// The matrix is returned in the CRS components ptr, col, and val.
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
	);

	void generateFixed4Matrix(
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

