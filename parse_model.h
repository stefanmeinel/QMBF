#ifndef PARSE_MODEL_H
#define PARSE_MODEL_H

#include "abstract_model.h"

#include <vector>
#include <map>
#include <string>
#include "parser.h"

using namespace std;

class parse_model : public abstract_model
{
  public:

    parse_model(const vector< string >& functions,
                const vector< string >& variables,
                const vector< string >& parameters,
                const vector< string >& constants,
                const vector< vector< string > >& derivatives);

// derived functions

    ~parse_model();

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

    int n_functions;
    int n_variables;
    int n_parameters;
    int n_constants;

    vector< string > var_names;
    vector< string > param_names;
    vector< string > const_names;
    vector< string > func_names;

    vector< parser* > funcs_parsers;
    vector< vector< parser* > > derivs_parsers;

    map< string, double > table;

    void set_args(const vector< double >& args);
    int errors;
};

#endif
