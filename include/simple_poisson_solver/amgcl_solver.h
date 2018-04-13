#pragma once

#include <amgcl/make_solver.hpp>
#include <amgcl/solver/bicgstab.hpp>
#include <amgcl/amg.hpp>
#include <amgcl/coarsening/smoothed_aggregation.hpp>
#include <amgcl/relaxation/spai0.hpp>
#include <amgcl/adapter/crs_tuple.hpp>

#include <vector>

#include "simple_poisson_solver/util.h"
#include "simple_poisson_solver/matrix.h"

using namespace std;

namespace amgcl_solver {

    typedef amgcl::backend::builtin<double> Backend;
    typedef amgcl::make_solver<
            // Use AMG as preconditioner:
            amgcl::amg<
                    Backend,
                    amgcl::coarsening::smoothed_aggregation,
                    amgcl::relaxation::spai0
            >,
            // And BiCGStab as iterative solver:
            amgcl::solver::bicgstab<Backend>
    > PoissonSolver;


    template <typename SOLUTION_TYPE>
    void solve(
            const QImage& target,
            const QImage& source,
            const QImage& mask,
            std::vector<std::pair<int, int>>& pixels,
            std::vector<SOLUTION_TYPE>& result_r,
            std::vector<SOLUTION_TYPE>& result_g,
            std::vector<SOLUTION_TYPE>& result_b
    ) {

        using namespace std::chrono;
        auto time_start = high_resolution_clock::now();

        auto qDebugWithTs = [&] {
            auto delta = duration_cast<microseconds>(high_resolution_clock::now() - time_start).count();
            return qDebug() << "[" << (delta / 1000.) << " ms] ";
        };


        std::vector<int> ptr;
        std::vector<int> col;
        std::vector<int> matrix_values;

        std::vector<int> rhs_r;
        std::vector<int> rhs_g;
        std::vector<int> rhs_b;

        ulong n = generateMatrix(target, source, mask, ptr, col, matrix_values,
                                 rhs_r, rhs_g, rhs_b, pixels);
        qDebugWithTs() << "Matrix ready";


        result_r.resize(n, 0);
        result_g.resize(n, 0);
        result_b.resize(n, 0);

        PoissonSolver::params prm;
        prm.solver.maxiter = 2;
        PoissonSolver solver(boost::tie(n, ptr, col, matrix_values), prm);
        qDebugWithTs() << "Solver ready";

        int iters;
        double error;
        boost::tie(iters, error) = solver(rhs_r, result_r);
        qDebugWithTs() << "R: error=" << error << ", iters=" << iters;
        boost::tie(iters, error) = solver(rhs_g, result_g);
        qDebugWithTs() << "G: error=" << error << ", iters=" << iters;
        boost::tie(iters, error) = solver(rhs_b, result_b);
        qDebugWithTs() << "B: error=" << error << ", iters=" << iters;
    }


}