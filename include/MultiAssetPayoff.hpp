#ifndef MULTIASSETPAYOFF_HPP
#define MULTIASSETPAYOFF_HPP
#include "MultiAssetModel.hpp"
#include <cmath>
#include <vector>
#include <random>
#include "Payoff.hpp"
class MultiAssetsPayoff{
    private :
    std::vector<bool> is_asian_;
    bool is_american_ ;
    
    public :
    explicit MultiAssetsPayoff(std::vector<bool> is_asian, bool is_american): is_asian_(is_asian), is_american_(is_american) {}
    
    double evaluate_basket(const std::vector<std::vector<double>> price,const std::vector<double>& weight,const double strike){
        double c=0;
        std::size_t n=weight.size();
        std::size_t m=price.size();
        for (std::size_t i=0; i<n; ++i){
            if (is_asian_[i]==false){
                c+=(price[m-1][i]/price[0][i])*weight[i];
            }else{
                double d=0;
                for (std::size_t j=1; j<m; ++j){
                    d+=price[j][i];
                }
                d=(d/price[0][i])*weight[i]/static_cast<double>(m-1);
                c+=d;
            }
        }
        return std::max(c-strike, 0.0);
    }
    double evaluate_rainbow(const std::vector<std::vector<double>> price,const double barrier,double strike, bool put)const{
        std::size_t n=price[0].size();
        std::size_t m=price.size();
        if (is_american_==false){
            if (put==true){
                double min=price[m-1][0]/price[0][0];
                double d=0;
                //int k_min=0;
                for (std::size_t i=1; i<n;++i){
                    d=price[m-1][i]/price[0][i];
                    if (d<min){
                        min=d;
                        //k_min=i;
                    }
                }
                PayoffPut calc(strike);
                return calc(min);   
            }else{
                double max=price[m-1][0]/price[0][0];
                double d=0;
                //int k_max=0;
                for (std::size_t i=1; i<n;++i){
                    d=price[m-1][i]/price[0][i];
                    if(d>max){
                    max=d;
                    //k_max=i;
                    }
                }
                PayoffCall calc(strike);
                return calc(max);
            }
        }else{
            for (std::size_t j = 1; j < m; ++j) {
                if (put == true) {
                    double min = price[j][0] / price[0][0];
                    for (std::size_t k = 1; k < n; ++k) {
                        double d = price[j][k] / price[0][k];
                        if (d < min) { min = d; }
                    }
                    if (min < barrier || j == m - 1) {
                        PayoffPut calc(strike);
                        return calc(min);
                    }
                } else {
                    double max = price[j][0] / price[0][0];
                    for (std::size_t k = 1; k < n; ++k) {
                        double d = price[j][k] / price[0][k];
                        if (d > max) { max = d; }
                    }
                    if (max > barrier || j == m - 1) {
                        PayoffCall calc(strike);
                        return calc(max);
                    }
                }
            }
        }
        return 0.0;
    }
};




#endif