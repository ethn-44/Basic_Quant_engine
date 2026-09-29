#include "CalibrationObjective.hpp"
#include <cmath>
CalibrationObjective::CalibrationObjective(MultiAsset& model,MonteCarloEngine& mc_engine,const MultiAssetsPayoff& payoff_engine,const std::vector<double>& market_prices,const std::vector<double>& spot_prices,std::size_t num_simulations,std::size_t num_steps,const std::vector<double>& strike_list,const std::vector<double>& barrier_list,const std::vector<bool>& is_put_list,const std::vector<double>& maturity_list,
double risk)
    : model_(model),mc_engine_(mc_engine),payoff_engine_(payoff_engine),market_prices_(market_prices),spot_prices_(spot_prices),num_simulations_(num_simulations),num_steps_(num_steps),strike_list_(strike_list),barrier_list_(barrier_list),is_put_list_(is_put_list),maturity_list_(maturity_list),risk_(risk) {}

bool CalibrationObjective::check_constraints(const std::vector<double>& params) const {
    std::size_t m=params.size();
    std::size_t n=spot_prices_.size();
    if (m!=(n+1)){
        return false;
    }else{
        if (std::abs(params[n])>1){
            return false;
        }
        for (std::size_t k=0; k<n;++k){
            if (params[k]<=0){
                return false;
            }
        }
    }
    return true;

}

std::vector<std::vector<double>> CalibrationObjective::build_cov_matrix(const std::vector<double>& sigmas, double rho) const{
    std::size_t n=spot_prices_.size();
    std::vector<std::vector<double>>  cov_mat(n,std::vector<double>(n));
    for (std::size_t i=0; i<n; ++i){
        for(std::size_t j=0; j<=i; ++j){
            if (i==j){
                cov_mat[i][i]=sigmas[i]*sigmas[i];
            }else{
                double tmp=sigmas[i]*sigmas[j]*rho;
                cov_mat[i][j]=tmp;
                cov_mat[j][i]=tmp;
            }
        }
    }
    return cov_mat;
}

double CalibrationObjective::operator()(const std::vector<double>& params) const {
    if (!check_constraints(params)){
        return 1e10;
    }
    std::size_t n=spot_prices_.size();
    std::vector<double> sigmas(params.begin(), params.begin() + n);
    double rho=params.back();
    model_.set_volatilities(sigmas);
    std::vector<std::vector<double>> new_cov = build_cov_matrix(sigmas, rho);
    model_.update_covariance_and_crout(new_cov);
    std::mt19937_64 rng(42);
    double total_squared_error = 0.0;
    for (std::size_t i = 0; i < market_prices_.size(); ++i) {
        mc_engine_.reset_seed();
        auto option_i_payoff = [this, i](const std::vector<std::vector<double>>& path) {
            return payoff_.evaluate_rainbow(path, barrier_list_[i], strike_list_[i], is_put_list_[i]);
        };
        double model_price = mc_engine_.price_path_dependant(model_, option_i_payoff, maturity_list_[i], num_simulations_, num_steps_);
        double diff = model_price - market_prices_[i];
        total_squared_error += diff * diff;
    }

return total_squared_error;

}
