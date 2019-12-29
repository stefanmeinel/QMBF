#include "multi_alt_exp_nonsym_mat_model.h"

multi_alt_exp_nonsym_mat_model::multi_alt_exp_nonsym_mat_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, int n_exp, int m_exp, int dim_1, int dim_2)
{
  E_str=_E_str;
  dE_str=_dE_str;
  A_str=_A_str;
  B_str=_B_str;
  t_str=_t_str;
  nexp=n_exp;
  mexp=m_exp;
  dim1=dim_1;
  if(dim1<2)
  {
    dim1=2;
  }
  dim2=dim_2;
  standard_exp_mat_model=new multi_exp_nonsym_mat_model(E_str, dE_str, A_str, B_str, t_str, nexp, dim1, dim2);
  alternating_exp_mat_model=new multi_exp_nonsym_mat_model(E_str+"o", dE_str+"o", A_str+"o", B_str+"o", t_str, mexp, dim1, dim2);
  standard_params.resize(standard_exp_mat_model->get_n_parameters());
  alternating_params.resize(alternating_exp_mat_model->get_n_parameters());
}


multi_alt_exp_nonsym_mat_model::~multi_alt_exp_nonsym_mat_model()
{
  delete standard_exp_mat_model;
  delete alternating_exp_mat_model;
}


int multi_alt_exp_nonsym_mat_model::get_n_functions()
{
  return standard_exp_mat_model->get_n_functions();
}


int multi_alt_exp_nonsym_mat_model::get_n_variables()
{
  return standard_exp_mat_model->get_n_variables();
}


int multi_alt_exp_nonsym_mat_model::get_n_parameters()
{
  return standard_exp_mat_model->get_n_parameters()+alternating_exp_mat_model->get_n_parameters();
}

int multi_alt_exp_nonsym_mat_model::get_n_constants()
{
  return 0;
}


void multi_alt_exp_nonsym_mat_model::set_parameters(const vector< double >& parameters)
{
  for(int n=0; n<nexp; ++n) // standard energies
  {
    standard_params[n]=parameters[n];
  }
  for(int m=0; m<mexp; ++m) // oscillating energies
  {
    alternating_params[m]=parameters[nexp+m];
  }
  for(int p=0; p<(dim1+dim2-1)*nexp; ++p) // standard amplitudes
  {
    standard_params[nexp+p]=parameters[nexp+mexp+p];
  }
  for(int p=0; p<(dim1+dim2-1)*mexp; ++p) // oscillating amplitudes
  {
    alternating_params[mexp+p]=parameters[nexp+mexp+(dim1+dim2-1)*nexp+p];
  }
  standard_exp_mat_model->set_parameters(standard_params);
  alternating_exp_mat_model->set_parameters(alternating_params);
}


void multi_alt_exp_nonsym_mat_model::set_constants(const vector< double >& constants)
{
}


double multi_alt_exp_nonsym_mat_model::alt(double x)
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


double multi_alt_exp_nonsym_mat_model::eval_function(int function, const vector< double >& arguments)
{
  return standard_exp_mat_model->eval_function(function, arguments)+alt(arguments[0]+1.0)*alternating_exp_mat_model->eval_function(function, arguments);
}


double multi_alt_exp_nonsym_mat_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  if( (parameter >= 0) && (parameter < nexp) )
  {
    return standard_exp_mat_model->eval_derivative(function, parameter, arguments);
  }
  else if( (parameter >= nexp) && (parameter < nexp+mexp) )
  {
    return alt(arguments[0]+1.0)*alternating_exp_mat_model->eval_derivative(function, parameter-nexp, arguments);
  }
  else if( (parameter >= nexp+mexp) && (parameter < nexp+mexp+(dim1+dim2-1)*nexp) )
  {
    return standard_exp_mat_model->eval_derivative(function, nexp+parameter-(nexp+mexp), arguments);
  }
  else if( (parameter >= nexp+mexp+(dim1+dim2-1)*nexp) && (parameter < nexp+mexp+(dim1+dim2-1)*nexp+(dim1+dim2-1)*mexp) )
  {
    return alt(arguments[0]+1.0)*alternating_exp_mat_model->eval_derivative(function, mexp+parameter-(nexp+mexp+(dim1+dim2-1)*nexp), arguments);
  }
  else
  {
    return 0.0;
  }
}


int multi_alt_exp_nonsym_mat_model::no_of_errors()
{
  return standard_exp_mat_model->no_of_errors()+alternating_exp_mat_model->no_of_errors();
}



string multi_alt_exp_nonsym_mat_model::get_function_name(int function, string variable)
{
  return                                 standard_exp_mat_model    -> get_function_name(function, variable)
         + "+alt(" + variable + "+1)*" + alternating_exp_mat_model -> get_function_name(function, variable);
}


string multi_alt_exp_nonsym_mat_model::get_function_name(int function)
{
  return get_function_name(function, t_str);
}


string multi_alt_exp_nonsym_mat_model::get_parameter_name(int parameter)
{
  if( (parameter >= 0) && (parameter < nexp) )
  {
    return standard_exp_mat_model->get_parameter_name(parameter);
  }
  else if( (parameter >= nexp) && (parameter < nexp+mexp) )
  {
    return alternating_exp_mat_model->get_parameter_name(parameter-nexp);
  }
  else if( (parameter >= nexp+mexp) && (parameter < nexp+mexp+(dim1+dim2-1)*nexp) )
  {
    return standard_exp_mat_model->get_parameter_name(nexp+parameter-(nexp+mexp));
  }
  else if( (parameter >= nexp+mexp+(dim1+dim2-1)*nexp) && (parameter < nexp+mexp+(dim1+dim2-1)*nexp+(dim1+dim2-1)*mexp) )
  {
    return alternating_exp_mat_model->get_parameter_name(mexp+parameter-(nexp+mexp+(dim1+dim2-1)*nexp));
  }
  else
  {
    return "";
  }
}


string multi_alt_exp_nonsym_mat_model::get_variable_name(int variable)
{
  if(variable!=0)
  {
    return "";
  }
  return t_str;
}

string multi_alt_exp_nonsym_mat_model::get_constant_name(int constant)
{
  return "";
}
