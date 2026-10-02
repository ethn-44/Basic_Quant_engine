#ifndef MULTIASSETMODEL_HPP
#define MULTIASSETMODEL_HPP
#include "MatrixCrout.hpp"
#include <cmath>
#include <vector>
#include <random>
struct specific_params {
    std::vector<double> kappa;
    std::vector<double> theta;
    std::vector<double> xi;
    std::vector<double> rho;
    std::vector<double> lambda;
    std::vector<double> mu;
    std::vector<double> delta;
};

class MultiAsset{
    private : 
    std::vector<std::vector<double>> cov_var_;
    std::vector<double> spots_;
    std::vector<double> vol_;
    double rate_;
    specific_params spec_params_;
    decomposition M_;
    std::vector<std::vector<double>> D_;
    std::vector<std::vector<double>> L_;
    public :
    enum Type { BS, HESTON, MERTON };
   explicit MultiAsset(const std::vector<double>& spots,const std::vector<double>& vol,double rate,const std::vector<std::vector<double>>& cov_var,const specific_params spec_params)
        : cov_var_(cov_var),spots_(spots),vol_(vol),rate_(rate),spec_params_(spec_params){
    decomposition M = Crout_decompo(cov_var_);
    L_ = M.L;
    D_ = M.D;
    }
    inline double rate() const {
    return rate_;
    }
    template <typename Matrix>
    void update_covariance_and_crout(const Matrix& new_cov) {
        this->cov_var_ = new_cov;
        decomposition M = Crout_decompo(new_cov);
        this->L_ = M.L;
        this->D_ = M.D;
    }

    void set_volatilities(const std::vector<double>& sigmas) {
    this->vol_ = sigmas; 
    }
    std::vector<std::vector<double>> generate_path(double maturity, std::size_t num_steps, std::mt19937_64& rng) const {
    
    std::size_t n = spots_.size();
    std::vector<double> default_lambda(n, 0.1);
    std::vector<double> default_mu(n, 0.0);   
    std::vector<double> default_delta(n, 0.2); 

    return price_Assets_Merton(maturity, num_steps, rng, default_lambda, default_mu, default_delta);
    }
    std::vector<std::vector<double>> price_Assets_bs(double maturity, std::size_t num_steps, std::mt19937_64& rng) const {
        double dt = maturity / static_cast<double>(num_steps);
        std::normal_distribution<double> dist(0.0, 1.0);
        std::size_t n=spots_.size();
        std::vector<std::vector<double>> price(num_steps + 1, std::vector<double>(n));
        price[0]=spots_;
        std::vector<double> X(n);
        std::vector<double> Z(n);
        for (std::size_t i=1; i<=num_steps;++i){
            for (std::size_t j=0; j<n;++j){
                Z[j]=std::sqrt(D_[j][j])*dist(rng);
                double c=0;
                for (std::size_t k=0; k<j;++k){
                    c+=L_[j][k]*Z[k];
                }
                X[j]=c+Z[j];
                price[i][j]=price[i-1][j]*std::exp((rate_-vol_[j]*vol_[j]/2.0)*dt + vol_[j]*std::sqrt(dt)*X[j]);
            }
        }
    return price;
    }
    std::vector<std::vector<double>> price_Assets_Heston(double maturity, std::size_t num_steps, std::mt19937_64& rng) const {
        std::vector<double> kappa=spec_params_.kappa;
        std::vector<double> theta=spec_params_.theta;
        std::vector<double> xi=spec_params_.xi;
        std::vector<double> rho=spec_params_.rho;
        double dt = maturity / static_cast<double>(num_steps);
        double sqdt= std::sqrt(dt);
        std::normal_distribution<double> dist(0.0, 1.0);
        std::size_t n=spots_.size();
        std::vector<std::vector<double>> price(num_steps + 1, std::vector<double>(n));
        price[0]=spots_;
        std::vector<std::vector<double>> vol(num_steps + 1, std::vector<double>(n));
        vol[0]=vol_;
        std::vector<double> X(n);
        std::vector<double> Z(n);
        for (std::size_t i=1; i<=num_steps;++i){
            for (std::size_t j=0; j<n;++j){
                Z[j]=std::sqrt(D_[j][j])*dist(rng);
                double c=0;
                for (std::size_t k=0; k<j;++k){
                    c+=L_[j][k]*Z[k];
                }
                X[j]=c+Z[j];
                double v_plus = std::max(vol[i-1][j], 0.0);
                double Z_2=dist(rng);
                double Z_v=rho[j]*X[j]+std::sqrt(1.0-rho[j]*rho[j])*Z_2;
                vol[i][j]=v_plus + kappa[j]*(theta[j]-v_plus)*dt + xi[j]*std::sqrt(v_plus)*sqdt*Z_v;
                price[i][j]=price[i-1][j]*std::exp((rate_-v_plus/2.0)*dt+std::sqrt(v_plus)*sqdt*X[j]);
            }
        }
    return price;
    }
    std::vector<std::vector<double>> price_Assets_Merton(double maturity, std::size_t num_steps, std::mt19937_64& rng) const {
        std::vector<double> lambda=spec_params_.lambda;
        std::vector<double> mu=spec_params_.mu;
        std::vector<double> delta=spec_params_.delta;
        double dt = maturity / static_cast<double>(num_steps);
        double sqdt= std::sqrt(dt);
        std::normal_distribution<double> dist(0.0, 1.0);
        std::size_t n=spots_.size();
        std::vector<std::vector<double>> price(num_steps + 1, std::vector<double>(n));
        price[0]=spots_;
        std::vector<double> vol;
        vol=vol_;
        std::vector<double> drift(n);
        std::vector<double> k(n);
        std::vector<double> X(n);
        std::vector<double> Z(n);
        for (std::size_t l=0; l<n;++l){
            k[l]=std::exp(mu[l]+delta[l]*delta[l]/2.0)-1;
            drift[l]=(rate_-vol[l]*vol[l]/2-lambda[l]*k[l])*dt; 
        }
        for (std::size_t i=1; i<=num_steps;++i){
            for (std::size_t j=0; j<n;++j){
                Z[j]=std::sqrt(D_[j][j])*dist(rng);
                double c=0;
                for (std::size_t k=0; k<j;++k){
                    c+=L_[j][k]*Z[k];
                }
                X[j]=c+Z[j];
                std::poisson_distribution<int> poisson_dist(lambda[j] * dt);
                std::size_t num=poisson_dist(rng);
                double d=1;
                for (std::size_t m=0;m<num;++m){
                    d*=std::exp(mu[j] + delta[j] * dist(rng));
                }
                price[i][j]=price[i-1][j]*d*std::exp(drift[j]+vol[j]*sqdt*X[j]);
            }
        }
    return price;
    }
    std::vector<std::vector<double>> simulate_paths(double maturity,std::size_t num_steps, std::mt19937_64& rng) const {
    switch (model_type_) {
            case BS:     return price_Assets_bs(maturity, num_steps, rng);
            case HESTON: return price_Assets_Heston(maturity, num_steps, rng);
            case MERTON: return price_Assets_Merton(maturity, num_steps, rng);
        }
    return {};
}
};


#endif
