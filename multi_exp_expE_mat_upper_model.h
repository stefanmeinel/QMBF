#ifndef MULTI_EXP_EXPE_MAT_UPPER_MODEL_H
#define MULTI_EXP_EXPE_MAT_UPPER_MODEL_H

#include "abstract_model.h"

#include <cmath>
#include <string>
#include <sstream>

using namespace std;

class multi_exp_expE_mat_upper_model : public abstract_model
{
  public:

    multi_exp_expE_mat_upper_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, int n_exp, int _dim);

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

    string E_str, dE_str, A_str, B_str, t_str;
    int nexp;
    int dim;
    vector< double > params;
    vector< int > f_to_v1;
    vector< int > f_to_v2;
};

#endif
