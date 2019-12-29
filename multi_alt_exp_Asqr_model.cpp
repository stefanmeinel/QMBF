#include "multi_alt_exp_Asqr_model.h"

multi_alt_exp_Asqr_model::multi_alt_exp_Asqr_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, int n_exp, int m_exp)
{
  E_str=_E_str;
  dE_str=_dE_str;
  A_str=_A_str;
  B_str=_B_str;
  nexp=n_exp;
  mexp=m_exp;
  t_str=_t_str;
  nexp=n_exp;
  mexp=m_exp;
  standard_params.resize(2*nexp);
  alternating_params.resize(2*mexp);
  standard_exp_model=new multi_exp_Asqr_model(E_str, dE_str, A_str, B_str, t_str, nexp);
  alternating_exp_model=new multi_exp_Asqr_model(E_str+"o", dE_str+"o", A_str+"o", B_str+"o", t_str, mexp);
}


multi_alt_exp_Asqr_model::~multi_alt_exp_Asqr_model()
{
  delete standard_exp_model;
  delete alternating_exp_model;
}


int multi_alt_exp_Asqr_model::get_n_functions()
{
  return 1;
}


int multi_alt_exp_Asqr_model::get_n_variables()
{
  return 1;
}


int multi_alt_exp_Asqr_model::get_n_parameters()
{
  return 2*(nexp+mexp);
}

int multi_alt_exp_Asqr_model::get_n_constants()
{
  return 0;
}


void multi_alt_exp_Asqr_model::set_parameters(const vector< double >& parameters)
{
  for(int n=0; n<nexp; ++n)
  {
    standard_params[n]=parameters[n];
  }
  for(int m=0; m<mexp; ++m)
  {
    alternating_params[m]=parameters[nexp+m];
  }
  for(int n=0; n<nexp; ++n)
  {
    standard_params[nexp+n]=parameters[nexp+mexp+n];
  }
  for(int m=0; m<mexp; ++m)
  {
    alternating_params[mexp+m]=parameters[2*nexp+mexp+m];
  }
  standard_exp_model->set_parameters(standard_params);
  alternating_exp_model->set_parameters(alternating_params);
}


void multi_alt_exp_Asqr_model::set_constants(const vector< double >& constants)
{
}


double multi_alt_exp_Asqr_model::alt(double x)
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


double multi_alt_exp_Asqr_model::eval_function(int function, const vector< double >& arguments)
{
  return standard_exp_model->eval_function(0, arguments)+alt(arguments[0]+1)*alternating_exp_model->eval_function(function, arguments);
}


double multi_alt_exp_Asqr_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  if( (parameter >= 0) && (parameter < nexp) )
  {
    return standard_exp_model->eval_derivative(0, parameter, arguments);
  }
  else if( (parameter >= nexp) && (parameter < nexp+mexp) )
  {
    return alt(arguments[0]+1)*alternating_exp_model->eval_derivative(0, parameter-nexp, arguments);
  }
  else if( (parameter >= nexp+mexp) && (parameter < 2*nexp+mexp) )
  {
    return standard_exp_model->eval_derivative(0, parameter-mexp, arguments);
  }
  else if( (parameter >= 2*nexp+mexp) && (parameter < 2*nexp+2*mexp) )
  {
    return alt(arguments[0]+1)*alternating_exp_model->eval_derivative(0, parameter-2*nexp, arguments);
  }
  else
  {
    return 0.0;
  }
}


int multi_alt_exp_Asqr_model::no_of_errors()
{
  return standard_exp_model->no_of_errors()+alternating_exp_model->no_of_errors();
}


string multi_alt_exp_Asqr_model::get_function_name(int function, string variable)
{
  return                                 standard_exp_model    -> get_function_name(function, variable)
         + "+alt(" + variable + "+1)*" + alternating_exp_model -> get_function_name(function, variable);
}


string multi_alt_exp_Asqr_model::get_function_name(int function)
{
  return get_function_name(function, t_str);
}


string multi_alt_exp_Asqr_model::get_parameter_name(int parameter)
{
  vector<string> names(2*(nexp+mexp), "");
  names[0]=E_str;
  for(int n=1; n<nexp; ++n)
  {
    stringstream param_st;
    param_st << dE_str << "_" << n;
    names[n]=param_st.str();
  }
  names[nexp]=E_str+"o";
  for(int m=1; m<mexp; ++m)
  {
    stringstream param_st;
    param_st << dE_str << "o_" << m;
    names[nexp+m]=param_st.str();
  }
  names[nexp+mexp]=A_str;
  for(int n=1; n<nexp; ++n)
  {
    stringstream param_st;
    param_st << B_str << "_" << n;
    names[nexp+mexp+n]=param_st.str();
  }
  names[2*nexp+mexp]=A_str+"o";
  for(int m=1; m<mexp; ++m)
  {
    stringstream param_st;
    param_st << B_str << "o_" << m;
    names[2*nexp+mexp+m]=param_st.str();
  }
  return names[parameter];
}


string multi_alt_exp_Asqr_model::get_variable_name(int variable)
{
  if(variable!=0)
  {
    return "";
  }
  return t_str;
}

string multi_alt_exp_Asqr_model::get_constant_name(int constant)
{
  return "";
}
