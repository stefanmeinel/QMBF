#include "chisqr_extra_term.h"


bool __in(const std::string& sub, const std::string& s)
{
  if(s.find(sub)!=std::string::npos)
  {
    return true;
  }
  else
  {
    return false;
  }
}



chisqr_extra_term::chisqr_extra_term(int n_parameters, int n_constants, string function)
{
  active=false;
  errors=0;

  n_params=n_parameters;
  func=function;
  _parser = new parser(function);

  num_diff_step=1e-08;

  params.resize(n_parameters, 0.0);
  parameter_names.resize(n_parameters, "");
  function_contains_parameter.resize(n_parameters);
  constant_names.resize(n_constants, "");
}


void chisqr_extra_term::set_parameters(const vector< double >& parameters)
{
  params=parameters;
}


void chisqr_extra_term::set_parameter_name(int parameter, string name)
{
  parameter_names[parameter]=name;
}


void chisqr_extra_term::set_constants(const vector< double >& constants)
{
  for(int c=0; c<constant_names.size(); ++c)
  {
    parameter_values[constant_names[c]]=constants[c];
  }
}


void chisqr_extra_term::set_constant_name(int constant, string name)
{
  constant_names[constant]=name;
}


void chisqr_extra_term::update_params()
{
  for(int p=0; p<n_params; ++p)
  {
    parameter_values[parameter_names[p]]=params[p];
  }
}


void chisqr_extra_term::set_num_diff_step(double d)
{
  num_diff_step=d;
}


void chisqr_extra_term::init_parameter_list()
{
  for(int p=0; p<n_params; ++p)
  {
    function_contains_parameter[p]=__in(parameter_names[p], func);
  }
}


double chisqr_extra_term::chi_sqr()
{
  if(!active)
  {
    return 0.0;
  }
  update_params();
  double d=_parser->parse(parameter_values);
  errors=_parser->get_no_of_errors();
  return d;
}


double chisqr_extra_term::beta(int parameter)
{
  if(!active)
  {
    return 0.0;
  }
  if(!function_contains_parameter[parameter])
  {
    return 0.0;
  }
  update_params();
  tmp_parameter_values=parameter_values;
  double derivative;
  tmp_parameter_values[parameter_names[parameter]]=parameter_values[parameter_names[parameter]]+num_diff_step;
  derivative=_parser->parse(tmp_parameter_values);
  tmp_parameter_values[parameter_names[parameter]]=parameter_values[parameter_names[parameter]]-num_diff_step;
  derivative-=_parser->parse(tmp_parameter_values);
  derivative/=(2.0*num_diff_step);
  return (-0.5)*derivative;
}


double chisqr_extra_term::alpha(int parameter1, int parameter2)
{
  if(!active)
  {
    return 0.0;
  }
  if(!function_contains_parameter[parameter1])
  {
    return 0.0;
  }
  if(!function_contains_parameter[parameter2])
  {
    return 0.0;
  }
  update_params();
  tmp_parameter_values=parameter_values;
  double sqrt_num_diff_step=sqrt(num_diff_step);
  double derivative;

  if(parameter1!=parameter2)
  {
    tmp_parameter_values[parameter_names[parameter1]]=parameter_values[parameter_names[parameter1]]+sqrt_num_diff_step;  // forward step
    tmp_parameter_values[parameter_names[parameter2]]=parameter_values[parameter_names[parameter2]]+sqrt_num_diff_step;  // forward step
    derivative=_parser->parse(tmp_parameter_values);

    tmp_parameter_values[parameter_names[parameter1]]=parameter_values[parameter_names[parameter1]]+sqrt_num_diff_step;  // forward step
    tmp_parameter_values[parameter_names[parameter2]]=parameter_values[parameter_names[parameter2]]-sqrt_num_diff_step;  // backward step
    derivative-=_parser->parse(tmp_parameter_values);

    tmp_parameter_values[parameter_names[parameter1]]=parameter_values[parameter_names[parameter1]]-sqrt_num_diff_step;  // backward step
    tmp_parameter_values[parameter_names[parameter2]]=parameter_values[parameter_names[parameter2]]+sqrt_num_diff_step;  // forward step
    derivative-=_parser->parse(tmp_parameter_values);

    tmp_parameter_values[parameter_names[parameter1]]=parameter_values[parameter_names[parameter1]]-sqrt_num_diff_step;  // backward step
    tmp_parameter_values[parameter_names[parameter2]]=parameter_values[parameter_names[parameter2]]-sqrt_num_diff_step;  // backward step
    derivative+=_parser->parse(tmp_parameter_values);
  }
  else
  {
    tmp_parameter_values[parameter_names[parameter1]]=parameter_values[parameter_names[parameter1]]+2.0*sqrt_num_diff_step;  // forward step
    derivative=_parser->parse(tmp_parameter_values);

    tmp_parameter_values[parameter_names[parameter1]]=parameter_values[parameter_names[parameter1]]-2.0*sqrt_num_diff_step;  // backward step
    derivative+=_parser->parse(tmp_parameter_values);

    tmp_parameter_values[parameter_names[parameter1]]=parameter_values[parameter_names[parameter1]];                // middle
    derivative-=2.0*(_parser->parse(tmp_parameter_values));
  }
  // divide by (2*sqrt_num_diff_step)^2 = 4*num_diff_step
  derivative/=(4.0*num_diff_step);

  return 0.5*derivative;
}


int chisqr_extra_term::no_of_errors()
{
  return errors;
}

void chisqr_extra_term::set_enabled(bool enabled)
{
  active=enabled;
}
