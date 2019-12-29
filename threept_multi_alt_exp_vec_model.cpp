#include "threept_multi_alt_exp_vec_model.h"

threept_multi_alt_exp_vec_model::threept_multi_alt_exp_vec_model(string _E_str, string _dE_str, string _F_str, string _dF_str, string _A_str, string _B_str, string _t_str, string _T_str, int n_exp_initial, int m_exp_initial, int n_exp_final, int m_exp_final, int n_vec)
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
  nvec=n_vec;

  vector<double> params_temp(nexp_initial+mexp_initial+nexp_final+mexp_final+(nexp_initial+mexp_initial)*(nexp_final+mexp_final), 0.0);
  params.resize(nvec, params_temp);

  threept_multi_alt_exp_models.resize(nvec);
  for(int v=0; v<nvec; ++v)
  {
    threept_multi_alt_exp_models[v]=new threept_multi_alt_exp_model(E_str, dE_str, F_str, dF_str, A_str, B_str, t_str, T_str, n_exp_initial, m_exp_initial, n_exp_final, m_exp_final);
  }
}


threept_multi_alt_exp_vec_model::~threept_multi_alt_exp_vec_model()
{
  for(int v=0; v<nvec; ++v)
  {
    delete threept_multi_alt_exp_models[v];
  }
}


int threept_multi_alt_exp_vec_model::get_n_functions()
{
  return nvec;
}


int threept_multi_alt_exp_vec_model::get_n_variables()
{
  return 2;
}


int threept_multi_alt_exp_vec_model::get_n_parameters()
{
  return nexp_initial+mexp_initial+nexp_final+mexp_final+nvec*(nexp_initial+mexp_initial)*(nexp_final+mexp_final);
}

int threept_multi_alt_exp_vec_model::get_n_constants()
{
  return 0;
}


void threept_multi_alt_exp_vec_model::set_parameters(const vector< double >& parameters)
{
  // all vector components share the same energy parameters
  for(int v=0; v<nvec; ++v)
  {
    for(int n=0; n<nexp_initial+mexp_initial+nexp_final+mexp_final; ++n)
    {
      params[v][n]=parameters[n];
    }
  }
  // but have individual amplitudes
  for(int v=0; v<nvec; ++v)
  {
    for(int n=0; n<(nexp_initial+mexp_initial)*(nexp_final+mexp_final); ++n)
    {
      params[v][nexp_initial+mexp_initial+nexp_final+mexp_final+n]=parameters[nexp_initial+mexp_initial+nexp_final+mexp_final+v*(nexp_initial+mexp_initial)*(nexp_final+mexp_final)+n];
    }
  }

  for(int v=0; v<nvec; ++v)
  {
    threept_multi_alt_exp_models[v]->set_parameters(params[v]);
  }
}


void threept_multi_alt_exp_vec_model::set_constants(const vector< double >& constants)
{
}


double threept_multi_alt_exp_vec_model::eval_function(int function, const vector< double >& arguments)
{
  return threept_multi_alt_exp_models[function]->eval_function(0, arguments);
}


double threept_multi_alt_exp_vec_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  if( (parameter>=0) && (parameter<nexp_initial+mexp_initial+nexp_final+mexp_final) )
  {
    return threept_multi_alt_exp_models[function]->eval_derivative(0, parameter, arguments);
  }
  else if( (parameter>=nexp_initial+mexp_initial+nexp_final+mexp_final) && (parameter<nexp_initial+mexp_initial+nexp_final+mexp_final+nvec*(nexp_initial+mexp_initial)*(nexp_final+mexp_final) ) )
  {
    if( (parameter-(nexp_initial+mexp_initial+nexp_final+mexp_final))/((nexp_initial+mexp_initial)*(nexp_final+mexp_final)) == function )
    {
      return threept_multi_alt_exp_models[function]->eval_derivative(0,nexp_initial+mexp_initial+nexp_final+mexp_final+(parameter-(nexp_initial+mexp_initial+nexp_final+mexp_final))%((nexp_initial+mexp_initial)*(nexp_final+mexp_final)), arguments);
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


int threept_multi_alt_exp_vec_model::no_of_errors()
{
  return 0;
}



string threept_multi_alt_exp_vec_model::get_single_function_name(int function, int nexpinitial, int nexpfinal, string _E_str, string _dE_str, string _F_str, string _dF_str, string _A_str, string _B_str, string variable)
{
  stringstream function_st;
  function_st << _A_str << "__" << function+1 << "*(exp(-" << _F_str << "*" << variable << ")*exp(-" << _E_str << "*(" << T_str << "-" << variable << "))";
  for(int K=0; K<nexpfinal; ++K)
  {
    for(int L=0; L<nexpinitial; ++L)
    {
      if( (L>0) || (K>0) )
      {
        function_st << "+" << _B_str << "_" << K << "_" << L << "__" << function+1 << "*exp(-(" << _F_str;
        for(int k=0; k<=K; ++k)
        {
          if(k>0)
          {
            function_st << "+" << _dF_str << "_" << k;
          }
        }
        function_st << ")*" << variable << ")*exp(-(" << _E_str;
        for(int l=0; l<=L; ++l)
        {
          if(l>0)
          {
            function_st << "+" << _dE_str << "_" << l;
          }
        }
        function_st << ")*(" << T_str << "-" << variable << "))";
      }
    }
  }
  function_st << ")";
  return function_st.str();
}


string threept_multi_alt_exp_vec_model::get_function_name(int function, string variable)
{
  string ee_string;
  string oe_string;
  string eo_string;
  string oo_string;
  string function_string;
  ee_string=get_single_function_name(function, nexp_initial, nexp_final,  E_str, dE_str, F_str, dF_str, A_str+"ee", B_str+"ee", variable);
  function_string+=ee_string;
  if(mexp_final>0)
  {
    oe_string=get_single_function_name(function, nexp_initial, nexp_final,  E_str, dE_str, F_str+"o", dF_str+"o", A_str+"oe", B_str+"oe", variable);
    function_string+="+alt(t)*"+oe_string;
  }
  if(mexp_initial>0)
  {
    eo_string=get_single_function_name(function, nexp_initial, nexp_final,  E_str+"o", dE_str+"o", F_str, dF_str, A_str+"eo", B_str+"eo", variable);
    function_string+="+alt(T-t)*"+eo_string;
  }
  if( (mexp_initial>0) && (mexp_final>0) )
  {
    oo_string=get_single_function_name(function, nexp_initial, nexp_final,  E_str+"o", dE_str+"o", F_str+"o", dF_str+"o", A_str+"oo", B_str+"oo", variable);
    function_string+="+alt(t)*alt(T-t)*"+oo_string;
  }
  return function_string;
}


string threept_multi_alt_exp_vec_model::get_function_name(int function)
{
  return get_function_name(function, t_str);
}


string threept_multi_alt_exp_vec_model::get_parameter_name(int parameter)
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
      param_st << dF_str+"o" << "_" << m;
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
      param_st << dE_str+"o" << "_" << m;
      parameters.push_back(param_st.str());
    }
  }
  for(int function=0; function<nvec; ++function)
  {
    for(int j=0; j<nexp_final*nexp_initial; ++j)
    {
      if(j==0)
      {
        stringstream param_st;
        param_st << A_str+"ee" << "__" << function+1;
        parameters.push_back(param_st.str());
      }
      else
      {
        int K=j/nexp_initial;
        int L=j%nexp_initial;
        stringstream param_st;
        param_st << B_str+"ee" << "_" << K << "_" << L << "__" << function+1;
        parameters.push_back(param_st.str());
      }
    }
    for(int j=0; j<mexp_final*nexp_initial; ++j)
    {
      if(j==0)
      {
        stringstream param_st;
        param_st << A_str+"oe" << "__" << function+1;
        parameters.push_back(param_st.str());
      }
      else
      {
        int K=j/nexp_initial;
        int L=j%nexp_initial;
        stringstream param_st;
        param_st << B_str+"oe" << "_" << K << "_" << L << "__" << function+1;
        parameters.push_back(param_st.str());
      }
    }
    for(int j=0; j<nexp_final*mexp_initial; ++j)
    {
      if(j==0)
      {
        stringstream param_st;
        param_st << A_str+"eo" << "__" << function+1;
        parameters.push_back(param_st.str());
      }
      else
      {
        int K=j/mexp_initial;
        int L=j%mexp_initial;
        stringstream param_st;
        param_st << B_str+"eo" << "_" << K << "_" << L << "__" << function+1;
        parameters.push_back(param_st.str());
      }
    }
    for(int j=0; j<mexp_final*mexp_initial; ++j)
    {
      if(j==0)
      {
        stringstream param_st;
        param_st << A_str+"oo" << "__" << function+1;
        parameters.push_back(param_st.str());
      }
      else
      {
        int K=j/mexp_initial;
        int L=j%mexp_initial;
        stringstream param_st;
        param_st << B_str+"oo" << "_" << K << "_" << L << "__" << function+1;
        parameters.push_back(param_st.str());
      }
    }
  }
  return parameters[parameter];
}


string threept_multi_alt_exp_vec_model::get_variable_name(int variable)
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

string threept_multi_alt_exp_vec_model::get_constant_name(int constant)
{
  return "";
}
