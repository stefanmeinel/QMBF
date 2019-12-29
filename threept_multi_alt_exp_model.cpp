#include "threept_multi_alt_exp_model.h"

threept_multi_alt_exp_model::threept_multi_alt_exp_model(string _E_str, string _dE_str, string _F_str, string _dF_str, string _A_str, string _B_str, string _t_str, string _T_str, int n_exp_initial, int m_exp_initial, int n_exp_final, int m_exp_final)
{
  E_str=_E_str;
  dE_str=_dE_str;
  F_str=_F_str;
  dF_str=_dF_str;
  A_str=_A_str;
  B_str=_B_str;
  t_str=_t_str;
  T_str=_T_str;

  nexp_initial=n_exp_initial;
  nexp_final=n_exp_final;
  mexp_initial=m_exp_initial;
  mexp_final=m_exp_final;

  ee_model=new threept_multi_exp_model(E_str, dE_str, F_str, dF_str, A_str+"ee", B_str+"ee", t_str, T_str, nexp_initial, nexp_final);
  ee_params.resize(ee_model->get_n_parameters());

  if( mexp_final>0 )
  {
    oe_model=new threept_multi_exp_model(E_str, dE_str, F_str+"o", dF_str+"o", A_str+"oe", B_str+"oe", t_str, T_str, nexp_initial, mexp_final);
    oe_params.resize(oe_model->get_n_parameters());
  }

  if( mexp_initial>0 )
  {
    eo_model=new threept_multi_exp_model(E_str+"o", dE_str+"o", F_str, dF_str, A_str+"eo", B_str+"eo", t_str, T_str, mexp_initial, nexp_final);
    eo_params.resize(eo_model->get_n_parameters());
  }

  if( (mexp_initial>0) && (mexp_final>0) )
  {
    oo_model=new threept_multi_exp_model(E_str+"o", dE_str+"o", F_str+"o", dF_str+"o", A_str+"oo", B_str+"oo", t_str, T_str, mexp_initial, mexp_final);
    oo_params.resize(oo_model->get_n_parameters());
  }
}


int threept_multi_alt_exp_model::get_n_functions()
{
  return 1;
}


int threept_multi_alt_exp_model::get_n_variables()
{
  return 2;
}


int threept_multi_alt_exp_model::get_n_parameters()
{
  return nexp_initial+mexp_initial+nexp_final+mexp_final+(nexp_initial+mexp_initial)*(nexp_final+mexp_final);
}


int threept_multi_alt_exp_model::get_n_constants()
{
  return 0;
}


void threept_multi_alt_exp_model::set_parameters(const vector< double >& parameters)
{
  for(int n=0; n<nexp_final; ++n)
  {
    ee_params[n]=parameters[n];
    if(mexp_initial>0)
    {
      eo_params[n]=parameters[n];
    }
  }
  for(int m=0; m<mexp_final; ++m)
  {
    oe_params[m]=parameters[nexp_final+m];
    if(mexp_initial>0)
    {
      oo_params[m]=parameters[nexp_final+m];
    }
  }
  for(int n=0; n<nexp_initial; ++n)
  {
    ee_params[nexp_final+n]=parameters[nexp_final+mexp_final+n];
    if(mexp_final>0)
    {
      oe_params[mexp_final+n]=parameters[nexp_final+mexp_final+n];
    }
  }
  for(int m=0; m<mexp_initial; ++m)
  {
    eo_params[nexp_final+m]=parameters[nexp_final+mexp_final+nexp_initial+m];
    if(mexp_final>0)
    {
      oo_params[mexp_final+m]=parameters[nexp_final+mexp_final+nexp_initial+m];
    }
  }
  for(int j=0; j<nexp_final*nexp_initial; ++j)
  {
    ee_params[nexp_final+nexp_initial+j]=parameters[nexp_final+mexp_final+nexp_initial+mexp_initial+j];
  }
  for(int j=0; j<mexp_final*nexp_initial; ++j)
  {
    oe_params[mexp_final+nexp_initial+j]=parameters[nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_final*nexp_initial+j];
  }
  for(int j=0; j<nexp_final*mexp_initial; ++j)
  {
    eo_params[nexp_final+mexp_initial+j]=parameters[nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_final*nexp_initial+mexp_final*nexp_initial+j];
  }
  for(int j=0; j<mexp_final*mexp_initial; ++j)
  {
    oo_params[mexp_final+mexp_initial+j]=parameters[nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_final*nexp_initial+mexp_final*nexp_initial+nexp_final*mexp_initial+j];
  }
  ee_model->set_parameters(ee_params);
  if(mexp_final>0)
  {
    oe_model->set_parameters(oe_params);
  }
  if(mexp_initial>0)
  {
    eo_model->set_parameters(eo_params);
  }
  if( (mexp_initial>0) && (mexp_final>0) )
  {
    oo_model->set_parameters(oo_params);
  }
}


void threept_multi_alt_exp_model::set_constants(const vector< double >& constants)
{
}


double threept_multi_alt_exp_model::alt(double x)
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


double threept_multi_alt_exp_model::eval_function(int function, const vector< double >& arguments)
{
  double t=arguments[0];
  double T=arguments[1];
  double result=0.0;
  result+=ee_model->eval_function(0, arguments);
  if(mexp_final>0)
  {
    result+=alt(t)*oe_model->eval_function(0, arguments);
  }
  if(mexp_initial>0)
  {
    result+=alt(T-t)*eo_model->eval_function(0, arguments);
  }
  if( (mexp_initial>0) && (mexp_final>0) )
  {
    result+=alt(t)*alt(T-t)*oo_model->eval_function(0, arguments);
  }
  return result;
}


double threept_multi_alt_exp_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  double t=arguments[0];
  double T=arguments[1];
  if(    (parameter>=0)
      && (parameter<nexp_final) )
  {
    double result=ee_model->eval_derivative(function, parameter, arguments);
    if(mexp_initial>0)
    {
      result+=alt(T-t)*eo_model->eval_derivative(function, parameter, arguments);
    }
    return result;
  }
  else if(    (parameter>=nexp_final)
           && (parameter<nexp_final+mexp_final) )
  {
    parameter-=nexp_final;
    double result=alt(t)*oe_model->eval_derivative(function, parameter, arguments);
    if(mexp_initial>0)
    {
      result+=alt(t)*alt(T-t)*oo_model->eval_derivative(function, parameter, arguments);
    }
    return result;
  }
  else if(    (parameter>=nexp_final+mexp_final)
           && (parameter<nexp_final+mexp_final+nexp_initial) )
  {
    parameter-=nexp_final+mexp_final;
    double result=ee_model->eval_derivative(function, nexp_final+parameter, arguments);
    if(mexp_final>0)
    {
      result+=alt(t)*oe_model->eval_derivative(function, mexp_final+parameter, arguments);
    }
    return result;
  }
  else if(    (parameter>=nexp_final+mexp_final+nexp_initial)
           && (parameter<nexp_final+mexp_final+nexp_initial+mexp_initial) )
  {
    parameter-=nexp_final+mexp_final+nexp_initial;
    double result=alt(T-t)*eo_model->eval_derivative(function, nexp_final+parameter, arguments);
    if(mexp_final>0)
    {
      result+=alt(t)*alt(T-t)*oo_model->eval_derivative(function, mexp_final+parameter, arguments);
    }
    return result;
  }
  else if(    (parameter>=nexp_final+mexp_final+nexp_initial+mexp_initial)
           && (parameter<nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final) )
  {
    parameter-=nexp_final+mexp_final+nexp_initial+mexp_initial;
    return ee_model->eval_derivative(function, nexp_final+nexp_initial+parameter, arguments);
  }
  else if(    (parameter>=nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final)
           && (parameter<nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final+nexp_initial*mexp_final) )
  {
    parameter-=nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final;
    return alt(t)*oe_model->eval_derivative(function, mexp_final+nexp_initial+parameter, arguments);
  }
  else if(    (parameter>=nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final+nexp_initial*mexp_final)
           && (parameter<nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final+nexp_initial*mexp_final+mexp_initial*nexp_final) )
  {
    parameter-=nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final+nexp_initial*mexp_final;
    return alt(T-t)*eo_model->eval_derivative(function, nexp_final+mexp_initial+parameter, arguments);
  }
  else if(    (parameter>=nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final+nexp_initial*mexp_final+mexp_initial*nexp_final)
           && (parameter<nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final+nexp_initial*mexp_final+mexp_initial*nexp_final+mexp_initial*mexp_final) )
  {
    parameter-=nexp_final+mexp_final+nexp_initial+mexp_initial+nexp_initial*nexp_final+nexp_initial*mexp_final+mexp_initial*nexp_final;
    return alt(t)*alt(T-t)*oo_model->eval_derivative(function, mexp_final+mexp_initial+parameter, arguments);
  }
  else
  {
    return 0.0;
  }
}


int threept_multi_alt_exp_model::no_of_errors()
{
  return 0;
}


string threept_multi_alt_exp_model::get_function_name(int function, string variable)
{
  string name=ee_model->get_function_name(function, variable);
  if( mexp_final>0 )
  {
    name=name+"+alt(" + variable + ")*"          + oe_model->get_function_name(function, variable);
  }
  if( mexp_initial>0 )
  {
    name=name+"+alt(" + T_str + "-" + variable + ")*"        + eo_model->get_function_name(function, variable);
  }
  if( (mexp_initial>0) && (mexp_final>0) )
  {
    name=name+"+alt(" + variable + ")*alt(" + T_str + "-" + variable + ")*" + oo_model->get_function_name(function, variable);
  }
  return name;
}


string threept_multi_alt_exp_model::get_function_name(int function)
{
  return get_function_name(function, t_str);
}


string threept_multi_alt_exp_model::get_parameter_name(int parameter)
{
  vector< string > parameters;
  for(int n=0; n<nexp_final; ++n)
  {
    if(n==0)
    {
      parameters.push_back(F_str);
    }
    else
    {
      stringstream param_st;
      param_st << dF_str << "_" << n;
      parameters.push_back(param_st.str());
    }
  }
  for(int m=0; m<mexp_final; ++m)
  {
    if(m==0)
    {
      parameters.push_back(F_str+"o");
    }
    else
    {
      stringstream param_st;
      param_st << dF_str+"o_" << m;
      parameters.push_back(param_st.str());
    }
  }
  for(int n=0; n<nexp_initial; ++n)
  {
    if(n==0)
    {
      parameters.push_back(E_str);
    }
    else
    {
      stringstream param_st;
      param_st << dE_str << "_" << n;
      parameters.push_back(param_st.str());
    }
  }
  for(int m=0; m<mexp_initial; ++m)
  {
    if(m==0)
    {
      parameters.push_back(E_str+"o");
    }
    else
    {
      stringstream param_st;
      param_st << dE_str+"o_" << m;
      parameters.push_back(param_st.str());
    }
  }
  for(int j=0; j<nexp_final*nexp_initial; ++j)
  {
    if(j==0)
    {
      parameters.push_back(A_str+"ee");
    }
    else
    {
      int K=j/nexp_initial;
      int L=j%nexp_initial;
      stringstream param_st;
      param_st << B_str+"ee_" << K << "_" << L;
      parameters.push_back(param_st.str());
    }
  }
  for(int j=0; j<mexp_final*nexp_initial; ++j)
  {
    if(j==0)
    {
      parameters.push_back(A_str+"oe");
    }
    else
    {
      int K=j/nexp_initial;
      int L=j%nexp_initial;
      stringstream param_st;
      param_st << B_str+"oe_" << K << "_" << L;
      parameters.push_back(param_st.str());
    }
  }
  for(int j=0; j<nexp_final*mexp_initial; ++j)
  {
    if(j==0)
    {
      parameters.push_back(A_str+"eo");
    }
    else
    {
      int K=j/mexp_initial;
      int L=j%mexp_initial;
      stringstream param_st;
      param_st << B_str+"eo_" << K << "_" << L;
      parameters.push_back(param_st.str());
    }
  }
  for(int j=0; j<mexp_final*mexp_initial; ++j)
  {
    if(j==0)
    {
      parameters.push_back(A_str+"oo");
    }
    else
    {
      int K=j/mexp_initial;
      int L=j%mexp_initial;
      stringstream param_st;
      param_st << B_str+"oo_" << K << "_" << L;
      parameters.push_back(param_st.str());
    }
  }
  return parameters[parameter];
}


string threept_multi_alt_exp_model::get_variable_name(int variable)
{
  return ee_model->get_variable_name(variable);
}


string threept_multi_alt_exp_model::get_constant_name(int constant)
{
  return ee_model->get_constant_name(constant);
}
