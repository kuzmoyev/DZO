#include <vector>

#include "utility.h"

using namespace std;
/**
 * @brief Parallel CPU implementation of gauss-seidel iterative solver
 */
namespace gauss_seidel_solver {
	QImage poisson(
			const QImage& target,
			const QImage& source,
			const QImage& mask,
			unsigned iterations
	);

	QImage poisson_impl(
			const QImage& target,
			const QImage& source,
			const QImage& mask,
			unsigned int iterations
	);
}
