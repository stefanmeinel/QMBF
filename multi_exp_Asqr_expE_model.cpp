#include "multi_exp_Asqr_expE_model.h"

multi_exp_Asqr_expE_model::multi_exp_Asqr_expE_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, int n_exp)
{
  E_str=_E_str;
  dE_str=_dE_str;
  A_str=_A_str;
  B_str=_B_str;
  t_str=_t_str;
  nexp=n_exp;
  params.resize(2*nexp);
}


int multi_exp_Asqr_expE_model::get_n_functions()
{
  return 1;
}


int multi_exp_Asqr_expE_model::get_n_variables()
{
  return 1;
}


int multi_exp_Asqr_expE_model::get_n_parameters()
{
  return 2*nexp;
}

int multi_exp_Asqr_expE_model::get_n_constants()
{
  return 0;
}


void multi_exp_Asqr_expE_model::set_parameters(const vector< double >& parameters)
{
  params=parameters;
}


void multi_exp_Asqr_expE_model::set_constants(const vector< double >& constants)
{
}


double multi_exp_Asqr_expE_model::eval_function(int function, const vector< double >& arguments)
{
  double t=arguments[0];
  double sum=0.0;
  double E=exp(params[0]);
  sum+=exp(-E*t);
  for(int n=1; n<nexp; ++n)
  {
    E+=exp(params[n]);
    sum+=params[nexp+n]*params[nexp+n]*exp(-E*t);
  }
  sum*=params[nexp]*params[nexp];
  return sum;
}


double multi_exp_Asqr_expE_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  double t=arguments[0];
  if(parameter==0)
  {
    return (-exp(params[0])*t)*eval_function(0, arguments);
  }
  else if( (parameter>0) && (parameter<nexp) )
  {
    double sum=0.0;
    double E=exp(params[0]);
    for(int n=1; n<parameter; ++n)
    {
      E+=exp(params[n]);
    }
    for(int n=parameter; n<nexp; ++n)
    {
      E+=exp(params[n]);
      sum+=params[nexp+n]*params[nexp+n]*exp(-E*t);
    }
    sum*=(-exp(params[parameter])*t)*params[nexp]*params[nexp];
    return sum;
  }
  else if(parameter==nexp)
  {
    double sum=0.0;
    double E=exp(params[0]);
    sum+=exp(-E*t);
    for(int n=1; n<nexp; ++n)
    {
      E+=exp(params[n]);
      sum+=params[nexp+n]*params[nexp+n]*exp(-E*t);
    }
    sum*=2.0*params[nexp];
    return sum;
  }
  else if(parameter>nexp)
  {
    double E=exp(params[0]);
    for(int n=1; n<=parameter-nexp; ++n)
    {
      E+=exp(params[n]);
    }
    return 2.0*params[parameter]*params[nexp]*params[nexp]*exp(-E*t);
  }
  else
  {
    return 0.0;
  }
}


int multi_exp_Asqr_expE_model::no_of_errors()
{
  return 0;
}


string multi_exp_Asqr_expE_model::get_function_name(int function, string variable)
{
  if(function!=0)
  {
    return "";
  }
  stringstream function_st;
  function_st << A_str << "*" << A_str << "*(exp(-exp(" << E_str << ")*" << variable << ")";
  for(int n=1; n<nexp; ++n)
  {
    function_st << "+" << B_str << "_" << n << "*" << B_str << "_" << n << "*exp(-(exp(" << E_str << ")";
    for(int m=1; m<=n; ++m)
    {
      function_st << "+exp(" << dE_str << "_" << m <<")";
    }
    function_st << ")*" << variable << ")";
  }
  function_st << ")";
  return function_st.str();
}


string multi_exp_Asqr_expE_model::get_function_name(int function)
{
  return get_function_name(function, t_str);
}


string multi_exp_Asqr_expE_model::get_parameter_name(int parameter)
{
  vector<string> names(2*nexp, "");
  names[0]=E_str;
  for(int n=1; n<nexp; ++n)
  {
    stringstream param_st;
    param_st << dE_str << "_" << n;
    names[n]=param_st.str();
  }
  names[nexp]=A_str;
  for(int n=1; n<nexp; ++n)
  {
    stringstream param_st;
    param_st << B_str << "_" << n;
    names[nexp+n]=param_st.str();
  }
  return names[parameter];
}


string multi_exp_Asqr_expE_model::get_variable_name(int variable)
{
  if(variable!=0)
  {
    return "";
  }
  return t_str;
}

string multi_exp_Asqr_expE_model::get_constant_name(int constant)
{
  return "";
}
