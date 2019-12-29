#include "threept_multi_exp_expE_model.h"

threept_multi_exp_expE_model::threept_multi_exp_expE_model(string _E_str, string _dE_str, string _F_str, string _dF_str, string _A_str, string _B_str, string _t_str, string _T_str, int n_exp_initial, int n_exp_final)
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
  params.resize(nexp_initial*nexp_final+nexp_initial+nexp_final);
}


int threept_multi_exp_expE_model::get_n_functions()
{
  return 1;
}


int threept_multi_exp_expE_model::get_n_variables()
{
  return 2;
}


int threept_multi_exp_expE_model::get_n_parameters()
{
  return params.size();
}

int threept_multi_exp_expE_model::get_n_constants()
{
  return 0;
}


void threept_multi_exp_expE_model::set_parameters(const vector< double >& parameters)
{
  params=parameters;
}


void threept_multi_exp_expE_model::set_constants(const vector< double >& constants)
{
}


double threept_multi_exp_expE_model::eval_function(int function, const vector< double >& arguments)
{
  double t=arguments[0];
  double T=arguments[1];
  double sum=0.0;
  double F=exp(params[0]);
  double E=exp(params[nexp_final]);
  sum+=exp(-F*t-E*(T-t));
  for(int k=0; k<nexp_final; ++k)
  {
    if(k>0)
    {
      F+=exp(params[k]);
    }
    E=exp(params[nexp_final]);
    for(int l=0; l<nexp_initial; ++l)
    {
      if(l>0)
      {
        E+=exp(params[nexp_final+l]);
      }
      if( (l>0) || (k>0) )
      {
        sum+=params[nexp_final+nexp_initial+k*nexp_initial+l]*exp(-F*t-E*(T-t));
      }
    }
  }
  sum*=params[nexp_final+nexp_initial];
  return sum;
}


double threept_multi_exp_expE_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  double t=arguments[0];
  double T=arguments[1];
  if(parameter==0)
  {
    return (-exp(params[parameter])*t)*eval_function(0, arguments);
  }
  else if( (parameter>0) && (parameter<nexp_final) )
  {
    double sum=0.0;
    double F=exp(params[0]);
    for(int k=1; k<parameter; ++k)
    {
      F+=exp(params[k]);
    }
    for(int k=parameter; k<nexp_final; ++k)
    {
      F+=exp(params[k]);
      double E=exp(params[nexp_final]);
      for(int l=0; l<nexp_initial; ++l)
      {
        if(l>0)
        {
          E+=exp(params[nexp_final+l]);
        }
        sum+=params[nexp_final+nexp_initial+k*nexp_initial+l]*exp(-F*t-E*(T-t));
      }
    }
    sum*=(-exp(params[parameter])*t)*params[nexp_final+nexp_initial];
    return sum;
  }
  else if(parameter==nexp_final)
  {
    return (-exp(params[parameter])*(T-t))*eval_function(0, arguments);
  }
  else if( (parameter>nexp_final) && (parameter<nexp_final+nexp_initial) )
  {
    double sum=0.0;
    double F=exp(params[0]);
    for(int k=0; k<nexp_final; ++k)
    {
      if(k>0)
      {
        F+=exp(params[k]);
      }
      double E=exp(params[nexp_final]);
      for(int l=1; l<parameter-nexp_final; ++l)
      {
        E+=exp(params[nexp_final+l]);
      }
      for(int l=parameter-nexp_final; l<nexp_initial; ++l)
      {
        E+=exp(params[nexp_final+l]);
        sum+=params[nexp_final+nexp_initial+k*nexp_initial+l]*exp(-F*t-E*(T-t));
      }
    }
    sum*=(-exp(params[parameter])*(T-t))*params[nexp_final+nexp_initial];
    return sum;
  }
  else if (parameter==nexp_final+nexp_initial)
  {
    double sum=0.0;
    double F=exp(params[0]);
    double E=exp(params[nexp_final]);
    sum+=exp(-F*t-E*(T-t));
    for(int k=0; k<nexp_final; ++k)
    {
      if(k>0)
      {
        F+=exp(params[k]);
      }
      E=exp(params[nexp_final]);
      for(int l=0; l<nexp_initial; ++l)
      {
        if(l>0)
        {
          E+=exp(params[nexp_final+l]);
        }
        if( (l>0) || (k>0) )
        {
          sum+=params[nexp_final+nexp_initial+k*nexp_initial+l]*exp(-F*t-E*(T-t));
        }
      }
    }
    return sum;
  }
  else if (parameter>nexp_final+nexp_initial)
  {
    int K=(parameter-(nexp_final+nexp_initial))/nexp_initial;
    int L=(parameter-(nexp_final+nexp_initial))%nexp_initial;
    double F=exp(params[0]);
    for(int k=0; k<=K; ++k)
    {
      if(k>0)
      {
        F+=exp(params[k]);
      }
    }
    double E=exp(params[nexp_final]);
    for(int l=0; l<=L; ++l)
    {
      if(l>0)
      {
        E+=exp(params[nexp_final+l]);
      }
    }
    return params[nexp_final+nexp_initial]*exp(-F*t-E*(T-t));
  }
  else
  {
    return 0.0;
  }
}


int threept_multi_exp_expE_model::no_of_errors()
{
  return 0;
}


string threept_multi_exp_expE_model::get_function_name(int function, string variable)
{
  if(function!=0)
  {
    return "";
  }
  stringstream function_st;
  function_st << A_str <<"*(exp(-exp(" << F_str << ")*" << variable << ")*exp(-exp(" << E_str << ")*(" << T_str << "-" << variable << "))";
  for(int K=0; K<nexp_final; ++K)
  {
    for(int L=0; L<nexp_initial; ++L)
    {
      if( (L>0) || (K>0) )
      {
        function_st << "+" << B_str << "_" << K << "_" << L << "*exp(-(exp(" << F_str << ")";
        for(int k=0; k<=K; ++k)
        {
          if(k>0)
          {
            function_st << "+exp(" << dF_str << "_" << k << ")";
          }
        }
        function_st << ")*" << variable << ")*exp(-(exp(" << E_str << ")";
        for(int l=0; l<=L; ++l)
        {
          if(l>0)
          {
            function_st << "+exp(" << dE_str << "_" << l << ")";
          }
        }
        function_st << ")*(" << T_str << "-" << variable << "))";
      }
    }
  }
  function_st << ")";
  return function_st.str();
}


string threept_multi_exp_expE_model::get_function_name(int function)
{
  return get_function_name(function, t_str);
}


string threept_multi_exp_expE_model::get_parameter_name(int parameter)
{
  if(parameter==0)
  {
    return F_str;
  }
  else if( (parameter>0) && (parameter<nexp_final) )
  {
    stringstream param_st;
    param_st << dF_str << "_" << parameter;
    return param_st.str();
  }
  else if(parameter==nexp_final)
  {
    return E_str;
  }
  else if( (parameter>nexp_final) && (parameter<nexp_final+nexp_initial) )
  {
    stringstream param_st;
    param_st << dE_str << "_" << parameter-nexp_final;
    return param_st.str();
  }
  else if (parameter==nexp_final+nexp_initial)
  {
    return A_str;
  }
  else if (parameter>nexp_final+nexp_initial)
  {
    int K=(parameter-(nexp_final+nexp_initial))/nexp_initial;
    int L=(parameter-(nexp_final+nexp_initial))%nexp_initial;
    stringstream param_st;
    param_st << B_str << "_" << K << "_" << L;
    return param_st.str();
  }
  else
  {
    return "";
  }
}


string threept_multi_exp_expE_model::get_variable_name(int variable)
{
  if(variable==0)
  {
    return t_str;
  }
  else if(variable==1)
  {
    return T_str;
  }
  else
  {
    return "";
  }
}

string threept_multi_exp_expE_model::get_constant_name(int constant)
{
  return "";
}
