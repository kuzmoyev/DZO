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
	__global__ void _kernel_gauss_seidel(const MatrixRow* mat, unsigned int mat_size,
										 const int* rhs, SolutionRow<RealT>* solution,
										 unsigned int iterations,
										 unsigned int items_per_thread) {
		int swap_idx = 0;
		#define IDX_OLD (swap_idx)
		#define IDX_NEW (swap_idx ^ 1)
		#define SWAP_BUFFERS swap_idx ^= 1

		auto index_from = threadIdx.x * items_per_thread;
		auto index_to = index_from + items_per_thread;
		if (index_to > mat_size)
			index_to = mat_size;

		for (unsigned int it = 0; it < iterations; it++) {
			for (size_t row = index_from; row < index_to; row++) {
				RealT row_res = rhs[row];
				#pragma unroll 4
				for (int i = 0; i < 4; i++) {
					row_res -= solution[mat[row].indices[i]].values[IDX_OLD];
				}

				solution[row].values[IDX_NEW] = row_res / -4;
			}
			SWAP_BUFFERS;
			__syncthreads();
		}
	}

	template <typename T>
	T ceil_div(T a, T d) {
		return (a + d - 1) / d;
	}

	__host__ void gauss_seidel_solve(
			const std::vector<std::array<int, 4>>& mat,
			const std::vector<int>& rhs,
			std::vector<real_t>& sol,
			unsigned int iterations) {
		constexpr unsigned max_threads_per_block = 1024;

		MatrixRow* d_mat;
		int* d_rhs;
		SolutionRow<real_t>* d_sol;
		unsigned mat_size = (unsigned) mat.size();

		unsigned items_per_thread = ceil_div(mat_size, max_threads_per_block);

		static_assert(sizeof(*d_mat) == sizeof(mat[0]), "Device matrix use different datatype");
		HANDLE_ERROR(cudaMalloc((void**) &d_mat, mat.size() * sizeof(*d_mat)));
		HANDLE_ERROR(cudaMalloc((void**) &d_rhs, rhs.size() * sizeof(*d_rhs)));
		HANDLE_ERROR(cudaMalloc((void**) &d_sol, sol.size() * sizeof(*d_sol)));

		HANDLE_ERROR(cudaMemcpy(d_mat, mat.data(),
								mat_size * sizeof(*d_mat),
								cudaMemcpyHostToDevice));

		HANDLE_ERROR(cudaMemcpy(d_rhs, rhs.data(),
								rhs.size() * sizeof(*d_rhs),
								cudaMemcpyHostToDevice));

		std::vector<SolutionRow<real_t>> doubled_solution(sol.size());
		for (size_t i = 0; i < sol.size(); i++)
			doubled_solution[i].values[0] = sol[i];

		HANDLE_ERROR(cudaMemcpy(d_sol, doubled_solution.data(),
								doubled_solution.size() * sizeof(*d_sol),
								cudaMemcpyHostToDevice));

		_kernel_gauss_seidel<<<1, 1024>>>(d_mat, mat_size, d_rhs, d_sol,
				iterations, items_per_thread);

		HANDLE_ERROR(cudaDeviceSynchronize());

		HANDLE_ERROR(cudaMemcpy(doubled_solution.data(), d_sol,
								doubled_solution.size() * sizeof(*d_sol),
								cudaMemcpyDeviceToHost));

		if (iterations % 2 == 0) {
			for (size_t i = 0; i < sol.size(); i++)
				sol[i] = doubled_solution[i].values[0];
		} else {
			for (size_t i = 0; i < sol.size(); i++)
				sol[i] = doubled_solution[i].values[1];
		}

		HANDLE_ERROR(cudaFree(d_mat));
		HANDLE_ERROR(cudaFree(d_rhs));
		HANDLE_ERROR(cudaFree(d_sol));
	}
}
