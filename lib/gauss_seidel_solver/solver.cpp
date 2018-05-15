#include <vector>

#include <boost/tuple/tuple.hpp>

#include "utility.h"

using namespace std;

namespace gauss_seidel_solver {


	const size_t TOO_MANY_PIXELS = 5 * 5;
	const uint MAX_OPERATIONS = 2 << 28;
	const uint RESIZE_FACTOR = 2;


	using namespace std::chrono;
	auto log_start = high_resolution_clock::now();

	auto qDebugWithTs() {
		auto delta = duration_cast<microseconds>(high_resolution_clock::now() - log_start).count();
		return qDebug() << "[" << (delta / 1000.) << "\tms] ";
	}


	template <typename SOLUTION_TYPE>
	void gauss_seidel_solve(
			const std::vector<std::array<int, 4>>& mat,
			const std::vector<int>& rhs,
			std::vector<SOLUTION_TYPE>& sol,
			int iterations
	) {
		vector<SOLUTION_TYPE> tmp_sol(sol);

		for (int it = 0; it < iterations; it++) {
			for (size_t p = 0; p < mat.size(); p++) {
				SOLUTION_TYPE row_res = rhs[p];

				for (auto n : mat[p]) {
					row_res -= sol[n];
				}

				tmp_sol[p] = row_res / -4;
			}
			swap(sol, tmp_sol);
		}
	}

	QImage poisson(
			const QImage& target,
			const QImage& source,
			const QImage& mask,
			bool reset_log
	) {
		if (reset_log)
			log_start = high_resolution_clock::now();

		std::vector<std::pair<int, int>> pixels;
		std::vector<float> sol_r;
		std::vector<float> sol_g;
		std::vector<float> sol_b;

		std::vector<std::array<int, 4>> mat;

		std::vector<int> rhs_r;
		std::vector<int> rhs_g;
		std::vector<int> rhs_b;
		qDebug() << "$2";
		utility::generateFixed4Matrix(target, source, mask, mat, sol_r, sol_g, sol_b, rhs_r, rhs_g, rhs_b,
											pixels);
		qDebug() << "$3";
		qDebugWithTs() << "Matrix ready";

		if (pixels.size() >= TOO_MANY_PIXELS) {
			// Precompute results on a smaller image
			auto tmp_target = target.scaled(target.width() / RESIZE_FACTOR, target.height() / RESIZE_FACTOR);
			auto tmp_source = source.scaled(source.width() / RESIZE_FACTOR, source.height() / RESIZE_FACTOR);
			auto tmp_mask = mask.scaled(mask.width() / RESIZE_FACTOR, mask.height() / RESIZE_FACTOR);

			auto tmp_result = poisson(tmp_target, tmp_source, tmp_mask, false).scaled(target.width(), target.height());

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


		int iterations = MAX_OPERATIONS / int(pixels.size());
		gauss_seidel_solve(mat, rhs_r, sol_r, iterations);
		gauss_seidel_solve(mat, rhs_g, sol_g, iterations);
		gauss_seidel_solve(mat, rhs_b, sol_b, iterations);

		qDebugWithTs() << "Solved in" << iterations << "iterations";

		QImage result(target);
		int x, y;
		for (size_t i = 0; i < mat.size(); i++) {
			boost::tie(x, y) = pixels[i];
			int r = utility::bound(int(sol_r[i]));
			int g = utility::bound(int(sol_g[i]));
			int b = utility::bound(int(sol_b[i]));

			QColor c(r, g, b);
			result.setPixel(x, y, c.rgb());
		}

		return result;
	}

	QImage poisson(
			const QImage& target,
			const QImage& source,
			const QImage& mask) {
		qDebug() << "$1";
		return poisson(target, source, mask, true);
	}


}
