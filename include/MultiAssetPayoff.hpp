#ifndef MULTIASSETPAYOFF_HPP
#define MULTIASSETPAYOFF_HPP
#include "MultiAssetModel.hpp"
#include <cmath>
#include <vector>
#include <random>
class MultiAssetsPayoff{
    private :
    std::vector<bool> is_asian_;
    
    public :
    explicit MultiAssetsPayoff(,std::vector<bool>> is_asian):is_asian_(is_asian){}

    double evaluate_basket(const std::vector<std::vector<double>> price,const std::vector<double>& weight,const double strike){
        double c=0;
        std::size_t n=weight.size();
        std::size_t m=price.size();
        for (std::size_t i=0; i<n; ++i){
            if (is_asian[i]==false){
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
    double 
    
};




#endif