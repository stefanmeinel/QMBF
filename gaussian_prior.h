#ifndef GAUSSIAN_PRIOR_H
#define GAUSSIAN_PRIOR_H

#include "abstract_prior.h"

#include <vector>

using namespace std;

class gaussian_prior : public abstract_prior
{
  public:

    gaussian_prior(int n_parameters);

// derived functions

    void set_parameters(const vector< double >& parameters);

    double chi_sqr();
    double beta(int parameter);
    double alpha(int parameter1, int parameter2);

    int no_of_errors();

// additional functions

    void set_prior(int parameter, double prior);
    void set_sigma(int parameter, double sigma);

    void set_enabled(bool enabled);

  private:

    bool active;

    int n_params;

    vector< double > params;
    vector< double > priors;
    vector< double > sigmas;

};

#endif
