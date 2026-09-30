#include "Nelder_Meadsolver.hpp"


std::vector<double> NelderMeadSolver::compute_centroid(const std::vector<std::vector<double>>& simplex){
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
std::vector<double> NelderMeadSolver::reflect(const std::vector<double>& centroid, const std::vector<double>& worst, double alpha = 1.0){
    std::size_t m=centroid.size();
    std::vector<double> vec(m);
    for (std::size_t j=0; j<m;++j){
        vec[j]=centroid[j]+alpha*(centroid[j]-worst[j]);
    }
    return vec;
}
std::vector<double> NelderMeadSolver::expand(const std::vector<double>& centroid, const std::vector<double>& reflected, double gamma = 2.0){
    std::size_t m=centroid.size();
    std::vector<double> vec(m);
    for (std::size_t j=0; j<m;++j){
        vec[j]=centroid[j]+gamma*(reflected[j]-centroid[j]);
    }
    return vec;  
}
std::vector<double> NelderMeadSolver::contract(const std::vector<double>& centroid, const std::vector<double>& point, double beta = 0.5){
    std::size_t m=centroid.size();
    std::vector<double> vec(m);
    for (std::size_t j=0; j<m;++j){
        vec[j]=centroid[j]+beta*(point[j]-centroid[j]);
    }
    return vec; 
}
void NelderMeadSolver::shrink(std::vector<std::vector<double>>& simplex,const CalibrationObjective& cost_fn, double delta = 0.5){
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


/////SOLVER/////

std::vector<double> NelderMeadSolver::solve(const CalibrationObjective& cost_fn,const std::vector<double>& initial_params,double step = 0.05){
    std::vector<std::vector<double>> simplex;
    std::size_t n=initial_params.size();
    std::vector<double> vec_ini=initial_params;
    vec_ini.push_back(cost_fn(vec_ini));
    simplex[0]=vec_ini;
    for (std::size_t i=0; i<=n;++i){
        std::vector<double> x=initial_params;
        x[i]=(std::abs(x[i]) > 1e-8) ? x[i] * (1.0 + step) : step;
        x.push_back(cost_fn(x));
        simplex[i+1 ]=x;   
    }
    ///to continue
}
