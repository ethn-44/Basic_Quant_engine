#ifndef NELDER_MEADSOLVER_HPP
#define NELDER_MEADSOLVER_HPP
#include <vector>
#include <cstddef>
#include "Calib_cost.hpp"

class NelderMeadSolver {
private:
    double alpha_;
    double gamma_;
    double beta_;
    double delta_;
    double tolerance_;
    std::size_t max_iterations_;

public:
    NelderMeadSolver(double alpha = 1.0, double gamma = 2.0, double beta = 0.5, double delta = 0.5, double tolerance = 1e-6, std::size_t max_iterations = 500)
    : alpha_(alpha),gamma_(gamma), beta_(beta), delta_(delta), tolerance_(tolerance), max_iterations_(max_iterations) {}

    std::vector<double> compute_centroid(const std::vector<std::vector<double>>& simplex) const;
    std::vector<double> reflect(const std::vector<double>& centroid, const std::vector<double>& worst, double alpha = 1.0) const;
    std::vector<double> expand(const std::vector<double>& centroid, const std::vector<double>& reflected, double gamma = 2.0) const;
    std::vector<double> contract(const std::vector<double>& centroid, const std::vector<double>& point, double beta = 0.5) const;
    void shrink(std::vector<std::vector<double>>& simplex, const CalibrationObjective& cost_fn, double delta = 0.5) const;
    void sort_simplex(std::vector<std::vector<double>>& simplex) const;
    std::vector<double> solve(const CalibrationObjective& cost_fn,const std::vector<double>& initial_params,double step = 0.05) const;
};
#endif