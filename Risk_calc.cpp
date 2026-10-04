#include "Risk_calc.hpp"
#include <algorithm>

RiskMetrics calculate_risk_metrics(std::vector<double>& portfolio_values, double confidence_level){
    double mean=0;
    double alpha = 1.0 - confidence_level;
    double variance=0;
    double skew=0;
    std::size_t n=portfolio_values.size();
    for (std::size_t i=0;i<n;++i){
        mean+=portfolio_values[i];
    }
    mean=mean/static_cast<double>(n);
    for (std::size_t j=0;j<n;++j){
        variance+=(portfolio_values[j]-mean)*(portfolio_values[j]-mean);
    }
    variance=variance/static_cast<double>(n-1);
    for (std::size_t k=0;k<n;++k){
        skew+=((portfolio_values[k]-mean)/std::sqrt(variance))*((portfolio_values[k]-mean)/std::sqrt(variance))*((portfolio_values[k]-mean)/std::sqrt(variance));
    }
    skew=skew/static_cast<double>(n);
    ///VaR and CVaR
    std::sort(portfolio_values.begin(), portfolio_values.end());
    std::size_t var_index = static_cast<std::size_t>(alpha * n);
    double var=portfolio_values[var_index];
    double cvar=0.0;
    for (std::size_t u=0;u<=var_index;++u){
        cvar+=portfolio_values[u];
    }
    cvar=cvar/(1.0+static_cast<double>(var_index));
    RiskMetrics risk={var,cvar,mean,variance,skew};
    return risk;
}