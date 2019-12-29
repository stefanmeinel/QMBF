#include "parse_model.h"

parse_model::parse_model(const vector< string >& functions,
                         const vector< string >& variables,
                         const vector< string >& parameters,
                         const vector< string >& constants,
                         const vector< vector< string > >& derivatives)
{
  errors=0;

  n_functions=functions.size();
  n_variables=variables.size();
  n_parameters=parameters.size();
  n_constants=constants.size();

  var_names=variables;
  param_names=parameters;
  const_names=constants;
  func_names=functions;

  for(vector< string >::const_iterator it=functions.begin(); it!=functions.end(); ++it)
  {
    funcs_parsers.push_back(new parser(*it));
  }

  for(vector< vector< string > >::const_iterator it1=derivatives.begin(); it1!=derivatives.end(); ++it1)
  {
    vector< parser* > derivs;
    for(vector< string >::const_iterator it2=(*it1).begin(); it2!=(*it1).end(); ++it2)
    {
      derivs.push_back(new parser(*it2));
    }
    derivs_parsers.push_back(derivs);
  }
}


parse_model::~parse_model()
{
  for(vector< parser* >::iterator it=funcs_parsers.begin(); it!=funcs_parsers.end(); ++it)
  {
    delete *it;
  }
  for(vector< vector< parser* > >::iterator it1=derivs_parsers.begin(); it1!=derivs_parsers.end(); ++it1)
  {
    for(vector< parser* >::iterator it2=(*it1).begin(); it2!=(*it1).end(); ++it2)
    {
      delete *it2;
    }
  }
}


int parse_model::get_n_functions()
{
  return n_functions;
}


int parse_model::get_n_variables()
{
  return n_variables;
}


int parse_model::get_n_parameters()
{
  return n_parameters;
}


int parse_model::get_n_constants()
{
  return n_constants;
}


void parse_model::set_parameters(const vector< double >& params)
{
  for(int p=0; p<n_parameters; ++p)
  {
    table[param_names[p]]=params[p];
  }
}


void parse_model::set_constants(const vector< double >& constants)
{
  for(int c=0; c<n_constants; ++c)
  {
    table[const_names[c]]=constants[c];
  }
}


void parse_model::set_args(const vector< double >& args)
{
  for(int v=0; v<n_variables; ++v)
  {
    table[var_names[v]]=args[v];
  }
}


double parse_model::eval_function(int function, const vector< double >& arguments)
{
  set_args(arguments);
  double d=funcs_parsers[function]->parse(table);
  errors=funcs_parsers[function]->get_no_of_errors();
  return d;
}


double parse_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  set_args(arguments);
  double d=derivs_parsers[function][parameter]->parse(table);
  errors=derivs_parsers[function][parameter]->get_no_of_errors();
  return d;
}

string parse_model::get_function_name(int function)
{
  return func_names[function];
}

string parse_model::get_parameter_name(int parameter)
{
  return param_names[parameter];
}

string parse_model::get_variable_name(int variable)
{
  return var_names[variable];
}

string parse_model::get_constant_name(int constant)
{
  return const_names[constant];
}


int parse_model::no_of_errors()
{
  return errors;
}
