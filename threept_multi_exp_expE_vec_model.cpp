#include "threept_multi_exp_expE_vec_model.h"

threept_multi_exp_expE_vec_model::threept_multi_exp_expE_vec_model(string _E_str, string _dE_str, string _F_str, string _dF_str, string _A_str, string _B_str, string _t_str, string _T_str, int n_exp_initial, int n_exp_final, int n_vec)
{
  E_str=_E_str;
  dE_str=_dE_str;
  F_str=_F_str;
  dF_str=_dF_str;
  A_str=_A_str;
  B_str=_B_str;
  t_str=_t_str;
  T_str=_T_str;
  nexp_initial= n_exp_initial;
  nexp_final=n_exp_final;
  nvec=n_vec;
  vector<double> params_temp(nexp_initial+nexp_final+nexp_initial*nexp_final, 0.0);
  params.resize(nvec, params_temp);
  threept_multi_exp_expE_models.resize(nvec);
  for(int v=0; v<nvec; ++v)
  {
    threept_multi_exp_expE_models[v]=new threept_multi_exp_expE_model(E_str, dE_str, F_str, dF_str, A_str, B_str, t_str, T_str, n_exp_initial, n_exp_final);
  }
}


threept_multi_exp_expE_vec_model::~threept_multi_exp_expE_vec_model()
{
  for(int v=0; v<nvec; ++v)
  {
    delete threept_multi_exp_expE_models[v];
  }
}


int threept_multi_exp_expE_vec_model::get_n_functions()
{
  return nvec;
}


int threept_multi_exp_expE_vec_model::get_n_variables()
{
  return 2;
}


int threept_multi_exp_expE_vec_model::get_n_parameters()
{
  return nexp_initial+nexp_final+nvec*nexp_initial*nexp_final;
}

int threept_multi_exp_expE_vec_model::get_n_constants()
{
  return 0;
}


void threept_multi_exp_expE_vec_model::set_parameters(const vector< double >& parameters)
{
  // all vector components share the same energy parameters
  for(int v=0; v<nvec; ++v)
  {
    for(int n=0; n<nexp_initial+nexp_final; ++n)
    {
      params[v][n]=parameters[n];
    }
  }
  // but have individual amplitudes
  for(int v=0; v<nvec; ++v)
  {
    for(int n=0; n<nexp_initial*nexp_final; ++n)
    {
      params[v][nexp_initial+nexp_final+n]=parameters[nexp_initial+nexp_final+v*nexp_initial*nexp_final+n];
    }
  }

  for(int v=0; v<nvec; ++v)
  {
    threept_multi_exp_expE_models[v]->set_parameters(params[v]);
  }
}


void threept_multi_exp_expE_vec_model::set_constants(const vector< double >& constants)
{
}


double threept_multi_exp_expE_vec_model::eval_function(int function, const vector< double >& arguments)
{
  return threept_multi_exp_expE_models[function]->eval_function(0, arguments);
}


double threept_multi_exp_expE_vec_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  if( (parameter>=0) && (parameter<nexp_initial+nexp_final) )
  {
    return threept_multi_exp_expE_models[function]->eval_derivative(0, parameter, arguments);
  }
  else if( (parameter>=nexp_initial+nexp_final) && (parameter<nexp_initial+nexp_final+nvec*nexp_initial*nexp_final) )
  {
    if( (parameter-(nexp_initial+nexp_final))/(nexp_initial*nexp_final) == function )
    {
      return threept_multi_exp_expE_models[function]->eval_derivative(0,nexp_initial+nexp_final+(parameter-(nexp_initial+nexp_final))%(nexp_initial*nexp_final), arguments);
    }
    else
    {
      return 0.0;
    }
  }
  else
  {
    return 0.0;
  }
}


int threept_multi_exp_expE_vec_model::no_of_errors()
{
  return 0;
}


string threept_multi_exp_expE_vec_model::get_function_name(int function, string variable)
{
  stringstream function_st;
  function_st << A_str << "__" << function+1 << "*(exp(-exp(" << F_str << ")*" << variable << ")*exp(-exp(" << E_str << ")*(" << T_str << "-" << variable << "))";
  for(int K=0; K<nexp_final; ++K)
  {
    for(int L=0; L<nexp_initial; ++L)
    {
      if( (L>0) || (K>0) )
      {
        function_st << "+" << B_str << "_" << K << "_" << L << "__" << function+1 << "*exp(-(exp(" << F_str << ")";
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


string threept_multi_exp_expE_vec_model::get_function_name(int function)
{
  return get_function_name(function, t_str);
}


string threept_multi_exp_expE_vec_model::get_parameter_name(int parameter)
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
  else
  {
    int function=(parameter-(nexp_initial+nexp_final))/(nexp_initial*nexp_final);
    parameter-=function*nexp_initial*nexp_final;
    if (parameter==nexp_final+nexp_initial)
    {
      stringstream param_st;
      param_st << A_str << "__" << function+1;
      return param_st.str();
    }
    else if (parameter>nexp_final+nexp_initial)
    {
      int K=(parameter-(nexp_final+nexp_initial))/nexp_initial;
      int L=(parameter-(nexp_final+nexp_initial))%nexp_initial;
      stringstream param_st;
      param_st << B_str << "_" << K << "_" << L << "__" << function+1;
      return param_st.str();
    }
    else
    {
      return "";
    }
  }
}


string threept_multi_exp_expE_vec_model::get_variable_name(int variable)
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

string threept_multi_exp_expE_vec_model::get_constant_name(int constant)
{
  return "";
}
