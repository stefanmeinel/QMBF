#include "multi_exp_expE_const_model.h"

multi_exp_expE_const_model::multi_exp_expE_const_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, string _C_str, int n_exp)
{
  E_str=_E_str;
  dE_str=_dE_str;
  A_str=_A_str;
  B_str=_B_str;
  t_str=_t_str;
  C_str=_C_str;
  model=new multi_exp_expE_model(E_str, dE_str, A_str, B_str, t_str, n_exp);
}


multi_exp_expE_const_model::~multi_exp_expE_const_model()
{
  delete model;
}


int multi_exp_expE_const_model::get_n_functions()
{
  return model->get_n_functions();
}


int multi_exp_expE_const_model::get_n_variables()
{
  return model->get_n_variables();
}


int multi_exp_expE_const_model::get_n_parameters()
{
  return model->get_n_parameters()+1;
}

int multi_exp_expE_const_model::get_n_constants()
{
  return model->get_n_constants();
}


void multi_exp_expE_const_model::set_parameters(const vector< double >& parameters)
{
  C=parameters[model->get_n_parameters()];
  vector<double> tmp_params=parameters;
  tmp_params.pop_back();
  model->set_parameters(tmp_params);
}


void multi_exp_expE_const_model::set_constants(const vector< double >& constants)
{
  model->set_constants(constants);
}


double multi_exp_expE_const_model::eval_function(int function, const vector< double >& arguments)
{
  return model->eval_function(function, arguments)+C;
}


double multi_exp_expE_const_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  if(parameter < model->get_n_parameters())
  {
    return model->eval_derivative(function, parameter, arguments);
  }
  else if(parameter == model->get_n_parameters())
  {
    return 1;
  }
  else
  {
    return 0; 
  }
}


int multi_exp_expE_const_model::no_of_errors()
{
  return model->no_of_errors();
}


string multi_exp_expE_const_model::get_function_name(int function)
{
  string st=model->get_function_name(function, t_str);
  return st+"+"+C_str;
}


string multi_exp_expE_const_model::get_parameter_name(int parameter)
{
  if(parameter < model->get_n_parameters())
  {
    return model->get_parameter_name(parameter);
  }
  else if(parameter == model->get_n_parameters())
  {
    return C_str;
  }
  else
  {
    return ""; 
  }
}


string multi_exp_expE_const_model::get_variable_name(int variable)
{
  return model->get_variable_name(variable);
}

string multi_exp_expE_const_model::get_constant_name(int constant)
{
  return model->get_constant_name(constant);
}
