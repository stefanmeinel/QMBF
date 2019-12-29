#include "multi_alt_exp_expE_vec_model.h"

multi_alt_exp_expE_vec_model::multi_alt_exp_expE_vec_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, int n_exp, int m_exp, int n_vec)
{
  E_str=_E_str;
  dE_str=_dE_str;
  A_str=_A_str;
  B_str=_B_str;
  t_str=_t_str;
  nexp=n_exp;
  mexp=m_exp;
  nvec=n_vec;
  vector<double> params_temp(2*nexp+2*mexp, 0.0);
  params.resize(nvec, params_temp);
  multi_alt_exp_expE_models.resize(nvec);
  for(int v=0; v<nvec; ++v)
  {
    multi_alt_exp_expE_models[v]=new multi_alt_exp_expE_model(E_str, dE_str, A_str, B_str, t_str, nexp, mexp);
  }
}


multi_alt_exp_expE_vec_model::~multi_alt_exp_expE_vec_model()
{
  for(int v=0; v<nvec; ++v)
  {
    delete multi_alt_exp_expE_models[v];
  }
}


int multi_alt_exp_expE_vec_model::get_n_functions()
{
  return nvec;
}


int multi_alt_exp_expE_vec_model::get_n_variables()
{
  return 1;
}


int multi_alt_exp_expE_vec_model::get_n_parameters()
{
  return nexp+mexp+nvec*(nexp+mexp);
}

int multi_alt_exp_expE_vec_model::get_n_constants()
{
  return 0;
}


void multi_alt_exp_expE_vec_model::set_parameters(const vector< double >& parameters)
{
  // all vector components share the same energy parameters
  for(int v=0; v<nvec; ++v)
  {
    for(int n=0; n<nexp+mexp; ++n)
    {
      params[v][n]=parameters[n];
    }
  }
  // but have individual amplitudes
  for(int v=0; v<nvec; ++v)
  {
    for(int n=0; n<nexp+mexp; ++n)
    {
      params[v][nexp+mexp+n]=parameters[nexp+mexp+v*(nexp+mexp)+n];
    }
  }

  for(int v=0; v<nvec; ++v)
  {
    multi_alt_exp_expE_models[v]->set_parameters(params[v]);
  }
}


void multi_alt_exp_expE_vec_model::set_constants(const vector< double >& constants)
{
}


double multi_alt_exp_expE_vec_model::eval_function(int function, const vector< double >& arguments)
{
  return multi_alt_exp_expE_models[function]->eval_function(0, arguments);
}


double multi_alt_exp_expE_vec_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  if( (parameter>=0) && (parameter<nexp+mexp) )
  {
    return multi_alt_exp_expE_models[function]->eval_derivative(0, parameter, arguments);
  }
  else if( (parameter>=nexp+mexp) && (parameter<nexp+mexp+nvec*(nexp+mexp)) )
  {
    if( (parameter-nexp-mexp)/(nexp+mexp) == function )
    {
      return multi_alt_exp_expE_models[function]->eval_derivative(0, nexp+mexp+(parameter-nexp-mexp)%(nexp+mexp), arguments);
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


int multi_alt_exp_expE_vec_model::no_of_errors()
{
  return 0;
}


string multi_alt_exp_expE_vec_model::get_function_name(int function, string variable)
{
  stringstream function_st;
  function_st << A_str << "__" << function+1 << "*(exp(-exp(" << E_str << ")*" << variable << ")";
  for(int n=1; n<nexp; ++n)
  {
    function_st << "+" << B_str << "_" << n << "__" << function+1 << "*exp(-(exp(" << E_str << ")";
    for(int k=1; k<=n; ++k)
    {
      function_st << "+exp(" << dE_str << "_" << k << ")";
    }
    function_st << ")*" << variable << ")";
  }
  function_st << ")";

  stringstream function_st_o;
  function_st_o << A_str+"o" << "__" << function+1 << "*(exp(-exp(" << E_str+"o" << ")*" << variable << ")";
  for(int n=1; n<nexp; ++n)
  {
    function_st_o << "+" << B_str+"o" << "_" << n << "__" << function+1 << "*exp(-(exp(" << E_str+"o" << ")";
    for(int k=1; k<=n; ++k)
    {
      function_st_o << "+exp(" << dE_str+"o" << "_" << k << ")";
    }
    function_st_o << ")*" << variable << ")";
  }
  function_st_o << ")";

  return function_st.str()+"+alt(" + variable + "+1)*"+function_st_o.str();
}


string multi_alt_exp_expE_vec_model::get_function_name(int function)
{
  return get_function_name(function, t_str);
}


string multi_alt_exp_expE_vec_model::get_parameter_name(int parameter)
{
  vector<string> names(nexp+mexp+nvec*(nexp+mexp), "");
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
    param_st << dE_str+"o_" << m;
    names[nexp+m]=param_st.str();
  }
  for(int v=0; v<nvec; ++v)
  {
    stringstream a1st;
    a1st << A_str << "__" << v+1;
    names[nexp+mexp+v*(nexp+mexp)]=a1st.str();
    for(int n=1; n<nexp; ++n)
    {
      stringstream b_st;
      b_st << B_str << "_" << n << "__" << v+1;
      names[nexp+mexp+v*(nexp+mexp)+n]=b_st.str();
    }
    stringstream ao1st;
    ao1st << A_str+"o" << "__" << v+1;
    names[nexp+mexp+v*(nexp+mexp)+nexp]=ao1st.str();
    for(int m=1; m<mexp; ++m)
    {
      stringstream bo_st;
      bo_st << B_str+"o" << "_" << m << "__" << v+1;
      names[nexp+mexp+v*(nexp+mexp)+nexp+m]=bo_st.str();
    }
  }
  return names[parameter];
}


string multi_alt_exp_expE_vec_model::get_variable_name(int variable)
{
  if(variable!=0)
  {
    return "";
  }
  return t_str;
}

string multi_alt_exp_expE_vec_model::get_constant_name(int constant)
{
  return "";
}
