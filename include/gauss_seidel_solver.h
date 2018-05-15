#include <vector>

#include "utility.h"

using namespace std;

namespace gauss_seidel_solver {
	QImage poisson(
			const QImage& target,
			const QImage& source,
			const QImage& mask
	);

	QImage poisson_impl(
			const QImage& target,
			const QImage& source,
			const QImage& mask,
			unsigned int iterations
	);
}
