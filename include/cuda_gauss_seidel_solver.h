#pragma once

#include <QtGui/QImage>

namespace cuda_gauss_seidel_solver {
	using real_t = float;

	QImage poisson(
			const QImage& target,
			const QImage& source,
			const QImage& mask);

	QImage poisson_impl(
			const QImage& target,
			const QImage& source,
			const QImage& mask,
			unsigned int iterations);

	void gauss_seidel_solve(
			const std::vector<std::array<int, 4>>& mat,
			std::array<const std::vector<int>, 3>& rhs,
			std::array<std::vector<real_t>, 3>& sol,
			unsigned int iterations);
}
