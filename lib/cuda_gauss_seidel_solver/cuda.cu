#include <cstddef>
#include <array>
#include <vector>
#include <driver_types.h>
#include <cuda_runtime_api.h>

#include "clion_cuda_stub.h"
#include "cuda_gauss_seidel_solver.h"


namespace cuda_gauss_seidel_solver {
	struct MatrixRow {
		unsigned int indices[4];
	};

	template <typename RealT>
	struct SolutionRow {
		RealT values[2];
	};

#define HANDLE_ERROR(err) (cuda_handle_error((err), __FILE__, __LINE__ ))

	__host__ void cuda_handle_error(cudaError_t err, const char* file, int line) {
		if (err != cudaSuccess) {
			printf("%s in %s at line %d\n", cudaGetErrorString(err), file, line);
			throw std::runtime_error("CUDA error");
		}
	}

	template <typename RealT>
	__global__ void _kernel_gauss_seidel(const MatrixRow* mat,
										 unsigned int mat_size,
										 const int* rhs,
										 RealT* old_solution,
										 RealT* solution) {
		auto thread_idx = blockIdx.x * blockDim.x + threadIdx.x;
		if (thread_idx >= mat_size)
			return;

		RealT row_res = rhs[thread_idx];
		#pragma unroll 4
		for (int i = 0; i < 4; i++) {
			row_res -= old_solution[mat[thread_idx].indices[i]];
		}

		solution[thread_idx] = row_res / -4;
	}

	template <typename T>
	T ceil_div(T a, T d) {
		return (a + d - 1) / d;
	}

	__host__ void gauss_seidel_solve(
			const std::vector<std::array<int, 4>>& mat,
			std::array<const std::vector<int>, 3>& rhs,
			std::array<std::vector<real_t>, 3>& sol,
			unsigned int iterations) {
		constexpr unsigned max_threads_per_block = 1024;

		MatrixRow* d_mat;
		std::array<int*, 3> d_rhs;
		std::array<std::array<real_t*, 2>, 3> d_sol;
		unsigned mat_size = (unsigned) mat.size();

		unsigned block_size = 128;
		unsigned block_count = ceil_div(mat_size, block_size);

		static_assert(sizeof(*d_mat) == sizeof(mat[0]), "Device matrix use different datatype");
		HANDLE_ERROR(cudaMalloc((void**) &d_mat, mat.size() * sizeof(*d_mat)));
		HANDLE_ERROR(cudaMemcpy(d_mat, mat.data(),
								mat_size * sizeof(*d_mat),
								cudaMemcpyHostToDevice));

		for (int i = 0; i < 3; i++) {
			HANDLE_ERROR(cudaMalloc((void**) &(d_rhs[i]), rhs[i].size() * sizeof(int)));
			HANDLE_ERROR(cudaMalloc((void**) &(d_sol[i][0]), sol[i].size() * sizeof(real_t)));
			HANDLE_ERROR(cudaMalloc((void**) &(d_sol[i][1]), sol[i].size() * sizeof(real_t)));

			HANDLE_ERROR(cudaMemcpy(d_rhs[i], rhs[i].data(),
									rhs[i].size() * sizeof(int),
									cudaMemcpyHostToDevice));

			HANDLE_ERROR(cudaMemcpy(d_sol[i][0], sol[i].data(),
									sol[i].size() * sizeof(real_t),
									cudaMemcpyHostToDevice));
		}

		std::array<cudaStream_t, 3> streams;
		for (int i = 0; i < 3; i++) {
			HANDLE_ERROR(cudaStreamCreate(&streams[i]));
		}

		int buf_idx = 0;
		for (int it = 0; it < iterations; it++) {
			for (int i = 0; i < 3; i++)
				_kernel_gauss_seidel<<<block_count, block_size, 0, streams[i]>>> (
						d_mat, mat_size, d_rhs[i], d_sol[i][buf_idx], d_sol[i][buf_idx ^ 1]);
			buf_idx ^= 1;
		}

		HANDLE_ERROR(cudaDeviceSynchronize());

		for (int i = 0; i < 3; i++) {
			HANDLE_ERROR(cudaStreamDestroy(streams[i]));
		}

		for (int i = 0; i < 3; i++) {
			HANDLE_ERROR(cudaMemcpy(sol[i].data(), d_sol[i][buf_idx],
									sol[i].size() * sizeof(real_t),
									cudaMemcpyDeviceToHost));

			HANDLE_ERROR(cudaFree(d_sol[i][1]));
			HANDLE_ERROR(cudaFree(d_sol[i][0]));
			HANDLE_ERROR(cudaFree(d_rhs[i]));
		}
		HANDLE_ERROR(cudaFree(d_mat));
	}
}
