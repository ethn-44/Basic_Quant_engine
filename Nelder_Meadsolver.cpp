#include "Nelder_Meadsolver.hpp"
#include <iostream>

std::vector<double> NelderMeadSolver::compute_centroid(const std::vector<std::vector<double>>& simplex)const{
    std::size_t n=simplex[0].size();
    std::size_t m=simplex.size();
    std::vector<double> x_centroid(n-1);
    for (std::size_t j=0;j<(n-1);++j){
        double moy=0;
        for (std::size_t i=0; i<(m-1); ++i){
            moy+=simplex[i][j];
        }
        x_centroid[j]=moy/static_cast<double>(m-1);
    }
    return x_centroid;
}
std::vector<double> NelderMeadSolver::reflect(const std::vector<double>& centroid, const std::vector<double>& worst, double alpha)const{
    std::size_t m=centroid.size();
    std::vector<double> vec(m);
    for (std::size_t j=0; j<m;++j){
        vec[j]=centroid[j]+alpha*(centroid[j]-worst[j]);
    }
    return vec;
}
std::vector<double> NelderMeadSolver::expand(const std::vector<double>& centroid, const std::vector<double>& reflected, double gamma )const{
    std::size_t m=centroid.size();
    std::vector<double> vec(m);
    for (std::size_t j=0; j<m;++j){
        vec[j]=centroid[j]+gamma*(reflected[j]-centroid[j]);
    }
    return vec;  
}
std::vector<double> NelderMeadSolver::contract(const std::vector<double>& centroid, const std::vector<double>& point, double beta)const{
    std::size_t m=centroid.size();
    std::vector<double> vec(m);
    for (std::size_t j=0; j<m;++j){
        vec[j]=centroid[j]+beta*(point[j]-centroid[j]);
    }
    return vec; 
}
void NelderMeadSolver::shrink(std::vector<std::vector<double>>& simplex,const CalibrationObjective& cost_fn, double delta)const{
    std::size_t n=simplex[0].size(); //nb_parameters
    std::size_t m=simplex.size(); // nb x_i
    for (std::size_t i=1;i<(m);++i){
        for (std::size_t j=0; j<(n-1);++j){
            simplex[i][j]=simplex[0][j]+ delta*(simplex[i][j]-simplex[0][j]);
        }
        std::vector<double> params_i(simplex[i].begin(), simplex[i].begin() + (n - 1));
        simplex[i][n-1]=cost_fn(params_i);
    }
}
void NelderMeadSolver::sort_simplex(std::vector<std::vector<double>>& simplex)const{
std::sort(simplex.begin(), simplex.end(), [](const std::vector<double>& a, const std::vector<double>& b) {
        return a.back() < b.back();
    });
}

/////SOLVER/////

std::vector<double> NelderMeadSolver::solve(const CalibrationObjective& cost_fn,const std::vector<double>& initial_params,double step)const{
    std::size_t n = initial_params.size();
    std::size_t m = n + 1;
    std::vector<std::vector<double>> simplex(m);
    std::vector<double> vec_ini=initial_params;
    vec_ini.push_back(cost_fn(vec_ini));
    simplex[0]=vec_ini;
    for (std::size_t i=0; i<(n);++i){
        std::vector<double> x=initial_params;
        x[i]=(std::abs(x[i]) > 1e-8) ? x[i] * (1.0 + step) : step;
        x.push_back(cost_fn(x));
        simplex[i+1]=x;   
    }
    std::size_t iter=0;
    sort_simplex(simplex);

    while (iter<max_iterations_ && (std::abs(simplex[0].back()-simplex.back().back())>tolerance_)){
        std::vector<double> centroid=compute_centroid(simplex);
        std::vector<double> worst_params(simplex.back().begin(), simplex.back().begin() + n);
        std::vector<double> x_r= reflect(centroid,worst_params,alpha_);
        double f_r = cost_fn(x_r);
        if (f_r<simplex[0][n]){
            std::vector<double> x_e=expand(centroid,x_r,gamma_);
            double f_e=cost_fn(x_e);
            if (f_e<f_r){
                x_e.push_back(f_e);
                simplex.back()=(x_e);
            }else{
                x_r.push_back(f_r);
                simplex.back()=(x_r);
            }
        }else{
            if (f_r<simplex[m-2][n]){
                x_r.push_back(f_r);
                simplex.back()=x_r;
            }else{
                if (f_r < simplex[m-2][n]){
                    std::vector<double> x_c=contract(centroid,x_r,beta_);
                    double f_c=cost_fn(x_c);
                    if (f_c>simplex.back().back()){
                        shrink(simplex,cost_fn);
                    }else{
                        x_c.push_back(f_c);
                        simplex.back()=(x_c);
                    }
                }else{
                    std::vector<double> x_c=contract(centroid,worst_params,beta_);
                    double f_c=cost_fn(x_c);
                    if (f_c>simplex.back().back()){
                        shrink(simplex,cost_fn);
                    }else{
                        x_c.push_back(f_c);
                        simplex.back()=(x_c);
                    }
                }

            }
        }
        iter+=1;
        sort_simplex(simplex);
    }
    return std::vector<double>(simplex[0].begin(), simplex[0].begin() + n);
}
