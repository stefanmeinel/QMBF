#include "multi_alt_exp_Asqr_expE_vec_BC_const_model.h"

multi_alt_exp_Asqr_expE_vec_BC_const_model::multi_alt_exp_Asqr_expE_vec_BC_const_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, string _T_str, string _C_str, int n_exp, int m_exp, int n_vec)
{
  E_str=_E_str;
  dE_str=_dE_str;
  A_str=_A_str;
  B_str=_B_str;
  t_str=_t_str; T_str=_T_str;
  C_str=_C_str;
  nvec=n_vec;
  C.resize(nvec);
  Co.resize(nvec);
  model=new multi_alt_exp_Asqr_expE_vec_BC_model(E_str, dE_str, A_str, B_str, t_str, T_str, n_exp, m_exp, n_vec);
}


multi_alt_exp_Asqr_expE_vec_BC_const_model::~multi_alt_exp_Asqr_expE_vec_BC_const_model()
{
  delete model;
}


int multi_alt_exp_Asqr_expE_vec_BC_const_model::get_n_functions()
{
  return model->get_n_functions();
}


int multi_alt_exp_Asqr_expE_vec_BC_const_model::get_n_variables()
{
  return model->get_n_variables();
}


int multi_alt_exp_Asqr_expE_vec_BC_const_model::get_n_parameters()
{
  return model->get_n_parameters()+2*nvec;
}

int multi_alt_exp_Asqr_expE_vec_BC_const_model::get_n_constants()
{
  return model->get_n_constants();
}


void multi_alt_exp_Asqr_expE_vec_BC_const_model::set_parameters(const vector< double >& parameters)
{
  for(int n=0; n<nvec; ++n)
  {    
    C[n]=parameters[model->get_n_parameters()+n];
  }
  for(int n=0; n<nvec; ++n)
  {    
    Co[n]=parameters[model->get_n_parameters()+nvec+n];
  }
  vector<double> tmp_params=parameters;
  tmp_params.erase(tmp_params.end()-2*nvec, tmp_params.end());
  model->set_parameters(tmp_params);
}


void multi_alt_exp_Asqr_expE_vec_BC_const_model::set_constants(const vector< double >& constants)
{
  model->set_constants(constants);
}


double multi_alt_exp_Asqr_expE_vec_BC_const_model::eval_function(int function, const vector< double >& arguments)
{
  return model->eval_function(function, arguments)+C[function]+alt(arguments[0]+1)*Co[function];
}


double multi_alt_exp_Asqr_expE_vec_BC_const_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  if(parameter < model->get_n_parameters())
  {
    return model->eval_derivative(function, parameter, arguments);
  }
  else if( (parameter - model->get_n_parameters())==function )
  {
    return 1;
  }
  else if( (parameter - model->get_n_parameters() - nvec)==function )
  {
    return alt(arguments[0]+1);
  }
  else
  {
    return 0; 
  }
}


int multi_alt_exp_Asqr_expE_vec_BC_const_model::no_of_errors()
{
  return model->no_of_errors();
}


string multi_alt_exp_Asqr_expE_vec_BC_const_model::get_function_name(int function)
{
  stringstream function_st;
  function_st << model->get_function_name(function) << "+" << C_str << "__" << function+1 << "+alt(" << t_str << "+1)*" << C_str << "o__" << function+1;
  return function_st.str();
}


string multi_alt_exp_Asqr_expE_vec_BC_const_model::get_parameter_name(int parameter)
{
  if(parameter < model->get_n_parameters())
  {
    return model->get_parameter_name(parameter);
  }
  else if( (parameter >= model->get_n_parameters()) && (parameter < model->get_n_parameters()+nvec) )
  {
    stringstream param_st;
    param_st << C_str << "__" << parameter - model->get_n_parameters() + 1;
    return param_st.str();
  }
  else if( (parameter >= model->get_n_parameters()+nvec) && (parameter < model->get_n_parameters()+2*nvec) )
  {
    stringstream param_st;
    param_st << C_str << "o__" << parameter - model->get_n_parameters() - nvec + 1;
    return param_st.str();
  }
  else
  {
    return "";
  }
}


string multi_alt_exp_Asqr_expE_vec_BC_const_model::get_variable_name(int variable)
{
  return model->get_variable_name(variable);
}

string multi_alt_exp_Asqr_expE_vec_BC_const_model::get_constant_name(int constant)
{
  return model->get_constant_name(constant);
}

double multi_alt_exp_Asqr_expE_vec_BC_const_model::alt(double x)
{
  int i=static_cast<int>(x);
  if(i%2==0)
  {
    return 1.0;
  }
  else
  {
    return -1.0;
  }
}
