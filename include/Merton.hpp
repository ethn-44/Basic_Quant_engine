#ifndef MERTON_HPP
#define MERTON_HPP
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

class MertonModel{
    private:
    double spot_;
    double rate_;
    double vol_;
    double lambda_;
    double mu_j_;
    double delta_j_;

    public:
    explicit MertonModel(double spot,double rate,double vol,double lambda, double mu_j, double delta_j) : spot_(spot),rate_(rate),vol_(vol),lambda_(lambda),mu_j_(mu_j),delta_j_(delta_j){}
    double spot() const { return spot_; }
    double rate() const { return rate_; }
    double vol() const { return vol_; }
    double lambda() const { return lambda_; }
    double mu_j() const { return mu_j_; }
    double delta_j() const { return delta_j_; }
    MertonModel with_spot(double new_spot) const {
        return MertonModel(new_spot, rate_, vol_, lambda_, mu_j_, delta_j_);
    }
    MertonModel with_vol(double new_vol) const {
        return MertonModel(spot_, rate_, new_vol, lambda_, mu_j_, delta_j_);
    }
    MertonModel with_rate(double new_rate) const {
        return MertonModel(spot_, new_rate, vol_, lambda_, mu_j_, delta_j_);
    }

    template <typename Generator>
    std::vector<double> generate_path(double maturity, std::size_t num_steps, Generator& gen) const{
        const double k=std::exp(mu_j_+delta_j_*delta_j_/2.0)-1;
        const double dt = maturity/static_cast<double>(num_steps);
        const double sqdt=std::sqrt(dt); 
        const double drift=(rate_-vol_*vol_/2-lambda_*k)*dt;
        std::normal_distribution<double> dist(0.0, 1.0);
        std::poisson_distribution<int> poisson_dist(lambda_ * dt);
        std::vector<double> S(num_steps+1);
        S[0]=spot_;
        double c=1;
        for (std::size_t i=0; i<num_steps; ++i){
            double Z=dist(gen);
            c=std::exp(drift+vol_*sqdt*Z);
            int num_jumps = poisson_dist(gen);
            for (int j = 0; j < num_jumps; ++j) {
                double J =dist(gen);
                c*=std::exp(mu_j_ + delta_j_ * J);
            }
            S[i+1]=S[i]*c;
        }
        return S;
    }
};
#endif