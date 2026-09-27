#ifndef GREEKS_HPP
#define GREEKS_HPP
#include <random>
#include "MonteCarlo.hpp"

struct GreeksResult{
    double price;
    double delta;
    double gamma;
    double vega;
    double rho;
    
};

class GreeksEngine{
    private:
    MonteCarloEngine& engine_;
    double h_ratio_;
    std::size_t num_steps_;
    ///
    public :
    explicit GreeksEngine(MonteCarloEngine& engine, double h_ratio = 0.025, std::size_t num_steps = 50): engine_(engine), h_ratio_(h_ratio), num_steps_(num_steps) {}
    template <typename ModelType, typename PayoffType>
    GreeksResult calculate(const ModelType& model,const PayoffType& payoff,double maturity,std::size_t num_sims){
        GreeksResult greek;
        const double S0 = model.spot();
        const double h = S0 * h_ratio_;
        const double h_vol = 0.01;
        const double h_r = 0.005;
        ///V(S_0), V(S_0-h),V(S_0+h) : Delta & Gamma
        ModelType model_up = model.with_spot(S0 + h);
        ModelType model_down = model.with_spot(S0 - h);
        ///
        engine_.reset_seed();
        double v_base = engine_.price_path_dependant(model,payoff, maturity, num_sims, num_steps_);
        engine_.reset_seed();
        double v_up = engine_.price_path_dependant(model_up,payoff, maturity, num_sims, num_steps_);
        engine_.reset_seed();
        double v_down = engine_.price_path_dependant(model_down,payoff, maturity, num_sims, num_steps_);
        greek.price=v_base;
        ///Delta & Gamma
        greek.delta = (v_up-v_down)/(2*h);
        greek.gamma = (v_up+v_down-2*v_base)/(h*h);
        //Vega
        double vol=model.vol();
        ModelType model_up_vol = model.with_vol(vol + h_vol);
        ModelType model_down_vol = model.with_vol(vol - h_vol);
        engine_.reset_seed();
        double v_up_vol = engine_.price_path_dependant(model_up_vol,payoff, maturity, num_sims, num_steps_);
        engine_.reset_seed();
        double v_down_vol = engine_.price_path_dependant(model_down_vol,payoff, maturity, num_sims, num_steps_);
        greek.vega=(v_up_vol-v_down_vol)/(2*h_vol);
        //rho
        ModelType model_r_up   = model.with_rate(model.rate() + h_r);
        ModelType model_r_down = model.with_rate(model.rate() - h_r);
        engine_.reset_seed();
        double v_r_up = engine_.price_path_dependant(model_r_up,payoff, maturity, num_sims, num_steps_);
        engine_.reset_seed();
        double v_r_down = engine_.price_path_dependant(model_r_down,payoff, maturity, num_sims, num_steps_);
        greek.rho=(v_r_up - v_r_down) / (2.0 * h_r);
        return greek;
    }
};
















#endif