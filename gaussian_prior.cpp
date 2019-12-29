#include "gaussian_prior.h"

gaussian_prior::gaussian_prior(int n_parameters)
{
  active=true;
  n_params=n_parameters;

  params.resize(n_parameters, 0.0);
  priors.resize(n_parameters, 0.0);
  sigmas.resize(n_parameters, 1.0);
}


void gaussian_prior::set_parameters(const vector< double >& parameters)
{
  params=parameters;
}


void gaussian_prior::set_prior(int parameter, double prior)
{
  priors[parameter]=prior;
}


void gaussian_prior::set_sigma(int parameter, double sigma)
{
  sigmas[parameter]=sigma;
}


double gaussian_prior::chi_sqr()
{
  if(!active)
  {
    return 0.0;
  }
  double sum=0.0;
  for(int p=0; p<n_params; ++p)
  {
    sum+=(params[p]-priors[p])*(params[p]-priors[p])/(sigmas[p]*sigmas[p]);
  }
  return sum;
}


double gaussian_prior::beta(int parameter)
{
  if(!active)
  {
    return 0.0;
  }
  return (-1.0)*(params[parameter]-priors[parameter])/(sigmas[parameter]*sigmas[parameter]);
}


double gaussian_prior::alpha(int parameter1, int parameter2)
{
  if(!active)
  {
    return 0.0;
  }
  if(parameter1!=parameter2)
  {
    return 0.0;
  }
  else
  {
    return 1.0/(sigmas[parameter1]*sigmas[parameter2]);
  }
}


int gaussian_prior::no_of_errors()
{
  return 0;
}

void gaussian_prior::set_enabled(bool enabled)
{
  active=enabled;
}
