#ifndef CHISQR_EXTRA_TERM_H
#define CHISQR_EXTRA_TERM_H

#include <vector>
#include <string>
#include <map>

#include "parser.h"

using namespace std;


class chisqr_extra_term
{
  public:

    chisqr_extra_term(int n_parameters, int n_constants, string function);

    void set_parameters(const vector< double >& parameters);
    void set_parameter_name(int parameter, string name);  // call first

    void set_constants(const vector< double >& constants);
    void set_constant_name(int constant, string name);  // call first

    void set_num_diff_step(double d);

    void init_parameter_list();

    double chi_sqr();
    double beta(int parameter);
    double alpha(int parameter1, int parameter2);

    int no_of_errors();

    void set_enabled(bool enabled);

  private:

    void update_params();

    string func;

    bool active;

    int n_params;

    vector< double > params;
    vector< string > parameter_names;

    vector< string > constant_names;

    map< string, double > parameter_values;
    map< string, double > tmp_parameter_values;

    vector< bool > function_contains_parameter;

    double num_diff_step;

    parser* _parser;
    int errors;
};

#endif
