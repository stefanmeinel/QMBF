#ifndef MULTI_EXP_ASQR_EXPE_VEC_BC_MODEL_H
#define MULTI_EXP_ASQR_EXPE_VEC_BC_MODEL_H

#include "abstract_model.h"
#include "multi_exp_Asqr_expE_vec_model.h"

#include <cmath>
#include <string>
#include <sstream>

using namespace std;

class multi_exp_Asqr_expE_vec_BC_model : public abstract_model
{
  public:

    multi_exp_Asqr_expE_vec_BC_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, string _T_str, int n_exp, int n_vec);
    ~multi_exp_Asqr_expE_vec_BC_model();

// derived functions

    int get_n_functions();
    int get_n_variables();
    int get_n_parameters();
    int get_n_constants();

    void set_parameters(const vector< double >& parameters);
    void set_constants(const vector< double >& constants);

    double eval_function(int function, const vector< double >& arguments);
    double eval_derivative(int function, int parameter, const vector< double >& arguments);

    int no_of_errors();

    string get_function_name(int function);
    string get_parameter_name(int parameter);
    string get_variable_name(int variable);
    string get_constant_name(int constant);

  private:

    string E_str, dE_str, A_str, B_str, t_str, T_str;

    multi_exp_Asqr_expE_vec_model* model;

    double T;

};

#endif
