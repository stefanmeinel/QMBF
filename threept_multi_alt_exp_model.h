#ifndef THREEPT_MULTI_ALT_EXP_MODEL_H
#define THREEPT_MULTI_ALT_EXP_MODEL_H

#include "abstract_model.h"
#include "threept_multi_exp_model.h"

#include <cmath>
#include <string>
#include <sstream>

using namespace std;

class threept_multi_alt_exp_model : public abstract_model
{
  public:

    threept_multi_alt_exp_model(string _E_str, string _dE_str, string _F_str, string _dF_str, string _A_str, string _B_str, string _t_str, string _T_str, int n_exp_initial, int m_exp_initial, int n_exp_final, int m_exp_final);

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
    string get_function_name(int function, string variable);
    string get_parameter_name(int parameter);
    string get_variable_name(int variable);
    string get_constant_name(int constant);

  private:

    double alt(double x);

    string E_str, dE_str, F_str, dF_str, A_str, B_str, t_str, T_str;

    int nexp_initial;
    int nexp_final;
    int mexp_initial;
    int mexp_final;

    threept_multi_exp_model* ee_model;
    threept_multi_exp_model* oe_model;
    threept_multi_exp_model* eo_model;
    threept_multi_exp_model* oo_model;

    vector< double > ee_params;
    vector< double > oe_params;
    vector< double > eo_params;
    vector< double > oo_params;

};

#endif
