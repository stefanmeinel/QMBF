#include "multifit_thread.h"

multifit_thread::multifit_thread(QObject* parent)
{
  ec = ok;
  max_steps = 100;
  steps_needed = 100;
  boot_prior = true;
  stop = false;
  n_parameters_dof=0;
}

multifit_thread::~multifit_thread()
{
}


void multifit_thread::set_fitter(fitter* f)
{
  _fitter = f;
}


void multifit_thread::set_max_steps(int steps)
{
  max_steps = steps;
}


int multifit_thread::get_steps_needed()
{
  return steps_needed;
}


void multifit_thread::set_gaussian_prior(gaussian_prior* gp)
{
  _gaussian_prior = gp;
}


void multifit_thread::set_n_functions(int n)
{
  n_functions = n;
}

void multifit_thread::set_n_parameters(int n)
{
  n_parameters = n;
}

void multifit_thread::set_parameter_names(const vector< string >& names)
{
  parameter_names = names;
}


void multifit_thread::set_n_parameters_dof(int n)
{
  n_parameters_dof = n;
}


void multifit_thread::set_boot_prior(bool pr)
{
  boot_prior = pr;
}


exit_code multifit_thread::get_exit_code()
{
  return ec;
}


void multifit_thread::set_fit_arguments(const vector< vector< double > >& fa)
{
  fit_arguments = fa;
}


void multifit_thread::set_fit_data(const vector< vector< vector< double > > >& fd)
{
  fit_data = fd;
}


void multifit_thread::set_priors(const vector< double >& prs)
{
  priors = prs;
}


void multifit_thread::set_sigmas(const vector< double >& sgs)
{
  sigmas = sgs;
}


void multifit_thread::set_file_name(const QString& f)
{
  curFile = f;
}

void multifit_thread::set_output_dir(const string& d)
{
  dir = d;
  if(*(dir.end()-1)!='/')
  {
    dir.append("/");
  }
}



QString multifit_thread::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}


double multifit_thread::get_multifit_sigma(int p)
{
  return multifit_sigma[p];
}



void multifit_thread::run()
{
  ec = ok;
  string fit_message;
  steps_needed=_fitter->fit(max_steps, fit_message);
  if(stop)
  {
    ec = aborted;
    return;
  }

  emit fit_finished(2);

  msleep(200);

  if(steps_needed==max_steps+1)
  {
    ec = fit_not_converge;
    return;
  }

  if(fit_message!="")
  {
    emit message_signal(fit_message.c_str());
    return;
  }

  vector< double > multifit_results_temp(n_parameters, 0.0);
  vector< vector< double > > multifit_results(fit_data.size(), multifit_results_temp);

  vector< ofstream* > all_output;
  vector< string > all_filenames;
  all_output.resize(n_parameters);
  all_filenames.resize(n_parameters);

  for(int p=0; p<n_parameters; ++p)
  {
    string filename;
    if(curFile.isEmpty())
    {
      filename=dir+"untitled.mbf_multifit_"+parameter_names[p]+".dat";
    }
    else
    {
      filename=dir+strippedName(curFile).toStdString()+"_multifit_"+parameter_names[p]+".dat";
    }
    all_filenames[p]=filename;

    ofstream* output = new ofstream(filename.c_str());

    if(!(*output))
    {
      ec = output_error;
      return;
    }

    output -> precision(15);
    all_output[p]=output;
  }

  gsl_rng_default_seed=0;
  gsl_rng_env_setup();
  const gsl_rng_type* Trng=gsl_rng_mt19937;
  gsl_rng* rng=gsl_rng_alloc(Trng);


  for(int n=0; n<fit_data.size(); ++n)
  {

    _fitter->set_average_data(fit_data[n]);

    if(boot_prior)
    {
      for(int p=0; p<n_parameters; ++p)
      {
        _gaussian_prior->set_prior(p, priors[p]+gsl_ran_gaussian(rng, sigmas[p]));
      }
    }

    steps_needed=_fitter->fit(max_steps, fit_message);
    if(fit_message!="")
    {
      emit message_signal(fit_message.c_str());
    }

    if(stop)
    {
      gsl_rng_free(rng);
      ec = aborted;
      for(int p=0; p<n_parameters; ++p)
      {
        all_output[p] -> close();
        delete all_output[p];
      }
      return;
    }

    if(steps_needed==max_steps+1)
    {
      stringstream message;
      message << "Multifit warning: Fit #" << n+1 << " did not converge after " << max_steps << " iterations." << endl;
      emit message_signal((message.str()).c_str());
    }


    for(int p=0; p<n_parameters; ++p)
    {
      multifit_results[n][p]=_fitter->get_parameter(p);
      *all_output[p] << multifit_results[n][p] << endl;
    }
    emit step(n);
  }

  for(int p=0; p<n_parameters; ++p)
  {
    all_output[p] -> close();
    delete all_output[p];
  }

  gsl_rng_free(rng);

  stringstream message;
  message << "Multifit with " << fit_data.size() << " ensembles completed.";
  emit message_signal((message.str()).c_str());

  multifit_av.resize(n_parameters, 0.0);
  multifit_sigma.resize(n_parameters, 0.0);

  for(int p=0; p<n_parameters; ++p)
  {
    for(int n=0; n<fit_data.size(); ++n)
    {
      multifit_av[p]+=multifit_results[n][p];
    }
    multifit_av[p]/=fit_data.size();
  }

  for(int p=0; p<n_parameters; ++p)
  {
    vector< double > range_temp(fit_data.size(), 0.0);
    for(int n=0; n<fit_data.size(); ++n)
    {
      range_temp[n]=multifit_results[n][p];
    }

    stable_sort(range_temp.begin(), range_temp.end());
    multifit_sigma[p]=(range_temp[static_cast<int>(0.841345*fit_data.size())]-range_temp[static_cast<int>(0.158655*fit_data.size())])/2.0;

    stringstream message1;
    message1 << parameter_names[p] << ":   average = " << multifit_av[p] << "   sigma = " << multifit_sigma[p] << endl << "Output file = " << all_filenames[p];
    emit message_signal((message1.str()).c_str());
  }

  emit message_signal("\n");
}
