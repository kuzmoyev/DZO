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
			const std::vector<int>& rhs,
			std::vector<real_t>& sol,
			unsigned int iterations);
}
