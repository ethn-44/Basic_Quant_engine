#ifndef BLACKSCHOLES_HPP
#define BLACKSCHOLES_HPP
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

class BlackScholesModel{
    private:
    double spot_;
    double rate_;
    double vol_;
    
    public :
    explicit BlackScholesModel(double spot,double rate,double vol) : spot_(spot),rate_(rate),vol_(vol){}
    double spot() const { return spot_; }
    double rate() const { return rate_; }
    double vol() const { return vol_; }
    inline double spot_at_maturity(double drift, double diffusion, double normal) const {
        return spot_ * std::exp(drift + diffusion * normal);
    }
    BlackScholesModel with_spot(double new_spot) const {
        return BlackScholesModel(new_spot, rate_, vol_);
    }
    BlackScholesModel with_vol(double new_vol) const {
        return BlackScholesModel(spot_, rate_, new_vol);
    }
    BlackScholesModel with_rate(double new_rate) const {
        return BlackScholesModel(spot_, new_rate, vol_);
    }
    std::vector<double> generate_path(double maturity,std::size_t num_steps, std::mt19937_64& rng)const {
        std::vector<double> path(num_steps+1);
        path[0]=spot_;
        const double dt = maturity / static_cast<double>(num_steps);
        const double drift = (rate_ - 0.5 * vol_ * vol_) * dt;
        const double diffusion = vol_ * std::sqrt(dt);
        std::normal_distribution<double> dist(0.0, 1.0);
        for( std::size_t i=0;i<num_steps;++i){
            path[i+1]=path[i]*std::exp(drift+diffusion* dist(rng));
        }
        return path;

    }
};
#endif