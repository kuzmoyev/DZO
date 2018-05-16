#include <vector>

#include <boost/tuple/tuple.hpp>
#include <QtConcurrent/QtConcurrent>
#include <include/gauss_seidel_solver.h>

#include "cuda_gauss_seidel_solver.h"
#include "utility.h"

using namespace std;

namespace cuda_gauss_seidel_solver {
	using namespace std::chrono;

	const size_t TOO_MANY_PIXELS = 5 * 5;
	const size_t CPU_THRESHOLD = 16 * 16;
	const uint RESIZE_FACTOR = 2;

	auto log_start = high_resolution_clock::now();

	auto qDebugWithTs() {
		auto delta = duration_cast<microseconds>(high_resolution_clock::now() - log_start).count();
		return qDebug() << "[" << (delta / 1000.) << "\tms] ";
	}

	QImage poisson_impl(
			const QImage& target,
			const QImage& source,
			const QImage& mask,
			unsigned int iterations) {

		std::vector<std::pair<int, int>> pixels;
		std::vector<float> sol_r;
		std::vector<float> sol_g;
		std::vector<float> sol_b;

		std::vector<std::array<int, 4>> mat;

		std::vector<int> rhs_r;
		std::vector<int> rhs_g;
		std::vector<int> rhs_b;
		utility::generateFixed4Matrix(target, source, mask, mat, sol_r, sol_g, sol_b, rhs_r, rhs_g, rhs_b,
									  pixels);

		if (pixels.size() >= TOO_MANY_PIXELS) {
			// Precompute results on a smaller image
			auto tmp_target = target.scaled(target.width() / RESIZE_FACTOR, target.height() / RESIZE_FACTOR);
			auto tmp_source = source.scaled(source.width() / RESIZE_FACTOR, source.height() / RESIZE_FACTOR);
			auto tmp_mask = mask.scaled(mask.width() / RESIZE_FACTOR, mask.height() / RESIZE_FACTOR);

			QImage tmp_result;
			if (pixels.size() >= CPU_THRESHOLD) {
				tmp_result = poisson_impl(tmp_target, tmp_source, tmp_mask, iterations * RESIZE_FACTOR);
			} else {
				tmp_result = gauss_seidel_solver::poisson_impl(
						tmp_target, tmp_source, tmp_mask,
						iterations * RESIZE_FACTOR);
			}
			tmp_result = tmp_result.scaled(target.width(), target.height());

			int x, y;
			for (size_t i = 0; i < mat.size(); i++) {
				boost::tie(x, y) = pixels[i];
				auto color = tmp_result.pixelColor(x, y);

				sol_r[i] = color.red();
				sol_g[i] = color.green();
				sol_b[i] = color.blue();
			}
			qDebugWithTs() << "Precomputed on:"
						   << target.width() / RESIZE_FACTOR << "x"
						   << target.height() / RESIZE_FACTOR;
		}

		std::array<const std::vector<int>, 3> rhs{std::move(rhs_r),
												  std::move(rhs_g),
												  std::move(rhs_b)};
		std::array<std::vector<real_t>, 3> sol{
				std::move(sol_r),
				std::move(sol_g),
				std::move(sol_b)
		};

		gauss_seidel_solve(mat, rhs, sol, iterations);

		qDebugWithTs() << "Solved in" << iterations << "iterations [GPU]";

		QImage result(target);
		int x, y;
		for (size_t i = 0; i < mat.size(); i++) {
			boost::tie(x, y) = pixels[i];
			int r = utility::bound(int(sol[0][i]));
			int g = utility::bound(int(sol[1][i]));
			int b = utility::bound(int(sol[2][i]));

			QColor c(r, g, b);
			result.setPixel(x, y, c.rgb());
		}

		return result;
	}

	QImage poisson(
			const QImage& target,
			const QImage& source,
			const QImage& mask) {
		const uint max_iterations = 2 << 28;
		const int img_size = target.size().height() * target.size().width();
		try {
			log_start = high_resolution_clock::now();
			return poisson_impl(target, source, mask, max_iterations / img_size);
		} catch (std::runtime_error&) {
			return {};
		}
	}
}
