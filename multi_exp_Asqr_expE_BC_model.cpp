#include "multi_exp_Asqr_expE_BC_model.h"

multi_exp_Asqr_expE_BC_model::multi_exp_Asqr_expE_BC_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, string _T_str, int n_exp)
{
  E_str=_E_str;
  dE_str=_dE_str;
  A_str=_A_str;
  B_str=_B_str;
  t_str=_t_str;
  T_str=_T_str;
  model=new multi_exp_Asqr_expE_model(E_str, dE_str, A_str, B_str, t_str, n_exp);
}


multi_exp_Asqr_expE_BC_model::~multi_exp_Asqr_expE_BC_model()
{
  delete model;
}


int multi_exp_Asqr_expE_BC_model::get_n_functions()
{
  return model->get_n_functions();
}


int multi_exp_Asqr_expE_BC_model::get_n_variables()
{
  return model->get_n_variables();
}


int multi_exp_Asqr_expE_BC_model::get_n_parameters()
{
  return model->get_n_parameters();
}

int multi_exp_Asqr_expE_BC_model::get_n_constants()
{
  return 1;
}


void multi_exp_Asqr_expE_BC_model::set_parameters(const vector< double >& parameters)
{
  model->set_parameters(parameters);
}


void multi_exp_Asqr_expE_BC_model::set_constants(const vector< double >& constants)
{
  T=constants[0];
}


double multi_exp_Asqr_expE_BC_model::eval_function(int function, const vector< double >& arguments)
{
  vector< double > BC_arguments(arguments);
  BC_arguments[0]=T-BC_arguments[0];
  return model->eval_function(function, arguments)+model->eval_function(function, BC_arguments);
}


double multi_exp_Asqr_expE_BC_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  vector< double > BC_arguments(arguments);
  BC_arguments[0]=T-BC_arguments[0];
  return model->eval_derivative(function, parameter, arguments)+model->eval_derivative(function, parameter, BC_arguments);
}


int multi_exp_Asqr_expE_BC_model::no_of_errors()
{
  return model->no_of_errors();
}


string multi_exp_Asqr_expE_BC_model::get_function_name(int function)
{
  string st=model->get_function_name(function, model->get_variable_name(0));
  string BC_st=model->get_function_name(function, "("+T_str+"-"+model->get_variable_name(0)+")");
  return st+"+"+BC_st;
}


string multi_exp_Asqr_expE_BC_model::get_parameter_name(int parameter)
{
  return model->get_parameter_name(parameter);
}


string multi_exp_Asqr_expE_BC_model::get_variable_name(int variable)
{
  return model->get_variable_name(variable);
}

string multi_exp_Asqr_expE_BC_model::get_constant_name(int constant)
{
  if(constant==0)
  {
    return T_str;
  }
  return "";
}
