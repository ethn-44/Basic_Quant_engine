#ifndef RISK_CALC_HPP
#define RISK_CALC_HPP
#include "Portfolio.hpp"
struct RiskMetrics {
    double var;
    double cvar;
    double mean;
    double variance;
    double skewness;
};
RiskMetrics calculate_risk_metrics(std::vector<double>& portfolio_values, double confidence_level = 0.99);




#endif