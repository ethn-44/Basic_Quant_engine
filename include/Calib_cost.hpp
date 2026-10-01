#ifndef CALIB_COST_HPP
#define CALIB_COST_HPP

#include <vector>
#include <cstddef>

#include "MultiAssetPayoff.hpp"
#include "MultiAssetModel.hpp"
#include "MonteCarlo.hpp"

class CalibrationObjective {
private:
    MultiAsset& model_;
    MonteCarloEngine& mc_engine_;
    const MultiAssetsPayoff& payoff_;
    const std::vector<double>& market_prices_;
    const std::vector<double>& spot_prices_;
    std::size_t num_simulations_;
    std::size_t num_steps_;
    std::vector<double> strike_list_;
    std::vector<double> barrier_list_;
    std::vector<bool> is_put_list_;
    std::vector<double> maturity_list_;
    double risk_;
    std::vector<std::vector<double>> build_cov_matrix(const std::vector<double>& sigmas, double rho) const;
    bool check_constraints(const std::vector<double>& params) const;
public:
    virtual ~CalibrationObjective() = default;

    explicit CalibrationObjective(MultiAsset& model,MonteCarloEngine& mc_engine,const MultiAssetsPayoff& payoff,const std::vector<double>& market_prices,const std::vector<double>& spot_prices,std::size_t num_simulations,std::size_t num_steps,const std::vector<double>& strike_list,const std::vector<double>& barrier_list,const std::vector<bool>& is_put_list,const std::vector<double>& maturity_list,double risk);
    virtual double operator()(const std::vector<double>& params) const;

protected:

    CalibrationObjective() = default;
};

#endif