#include "bootstrap_thread.h"

bootstrap_thread::bootstrap_thread(QObject* parent)
{
  ec = ok;
  max_steps = 100;
  steps_needed = 100;
  bssamples = 500;
  boot_prior = true;
  usebse = true;
  stop = false;
  n_parameters_dof=0;
  bootstrap_normalization=false;
}

bootstrap_thread::~bootstrap_thread()
{
}


void bootstrap_thread::set_bootstrap_normalization(bool bsn)
{
  bootstrap_normalization=bsn;
}



void bootstrap_thread::set_fitter(fitter* f)
{
  _fitter = f;
}


void bootstrap_thread::set_max_steps(int steps)
{
  max_steps = steps;
}


int bootstrap_thread::get_steps_needed()
{
  return steps_needed;
}


void bootstrap_thread::set_gaussian_prior(gaussian_prior* gp)
{
  _gaussian_prior = gp;
}


void bootstrap_thread::set_n_functions(int n)
{
  n_functions = n;
}

void bootstrap_thread::set_n_parameters(int n)
{
  n_parameters = n;
}

void bootstrap_thread::set_parameter_names(const vector< string >& names)
{
  parameter_names = names;
}


void bootstrap_thread::set_bssamples(int bs)
{
  bssamples = bs;
}

void bootstrap_thread::set_n_parameters_dof(int n)
{
  n_parameters_dof = n;
}

void bootstrap_thread::set_bse_file(string s)
{
  bse_file = s;
}


void bootstrap_thread::set_boot_prior(bool pr)
{
  boot_prior = pr;
}


void bootstrap_thread::set_use_bse(bool use_bse)
{
  usebse = use_bse;
}


exit_code bootstrap_thread::get_exit_code()
{
  return ec;
}


void bootstrap_thread::set_fit_arguments(const vector< vector< double > >& fa)
{
  fit_arguments = fa;
}


void bootstrap_thread::set_fit_data(const vector< vector< vector< double > > >& fd)
{
  fit_data = fd;
}


void bootstrap_thread::set_priors(const vector< double >& prs)
{
  priors = prs;
}


void bootstrap_thread::set_sigmas(const vector< double >& sgs)
{
  sigmas = sgs;
}


void bootstrap_thread::set_file_name(const QString& f)
{
  curFile = f;
}

void bootstrap_thread::set_output_dir(const string& d)
{
  dir = d;
  if(*(dir.end()-1)!='/')
  {
    dir.append("/");
  }
}



QString strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}


double bootstrap_thread::get_bs_sigma(int p)
{
  return boot_sigma[p];
}



void bootstrap_thread::run()
{
  ec = ok;
  string fit_message;
  steps_needed=_fitter->fit(max_steps, fit_message);
  if(stop)
  {
    ec = aborted;
    return;
  }

  emit fit_finished(1);

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

  vector< double > boot_data_temp_1(n_functions, 0.0);
  vector< vector< double > > boot_data_temp_2(fit_arguments.size(), boot_data_temp_1);
  vector< vector< vector< double > > > boot_data(fit_data.size(), boot_data_temp_2);

  gsl_rng_default_seed=0;
  gsl_rng_env_setup();
  const gsl_rng_type* Trng=gsl_rng_mt19937;
  gsl_rng* rng=gsl_rng_alloc(Trng);


  vector< int > bootconfig_temp(fit_data.size(), 0);
  vector< vector< int > > bootconfig(bssamples, bootconfig_temp);

  if(usebse)
  {
    ifstream bse(bse_file.c_str());
    int buf;
    bse >> buf;
    bse >> buf;
    for(int boot=0; boot<bssamples; ++boot)
    {
      for(unsigned int b=0; b<fit_data.size(); ++b)
      {
        bse >> bootconfig[boot][b];
        bootconfig[boot][b]-=1;
      }
    }
    bse.close();
  }
  else
  {
    for(int boot=0; boot<bssamples; ++boot)
    {
      for(unsigned int b=0; b<fit_data.size(); ++b)
      {
        bootconfig[boot][b]=static_cast<int>(gsl_rng_uniform(rng)*fit_data.size());
      }
    }
  }

  vector< double > boot_results_temp(n_parameters, 0.0);
  vector< vector< double > > boot_results(bssamples, boot_results_temp);


  vector< ofstream* > all_output;
  vector< string > all_filenames;
  all_output.resize(n_parameters);
  all_filenames.resize(n_parameters);

  for(int p=0; p<n_parameters; ++p)
  {
    string filename;
    if(curFile.isEmpty())
    {
      filename=dir+"untitled.mbf_bootstrap_"+parameter_names[p]+".dat";
    }
    else
    {
      filename=dir+strippedName(curFile).toStdString()+"_bootstrap_"+parameter_names[p]+".dat";
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



  for(int boot=0; boot<bssamples; ++boot)
  {
    for(unsigned int b=0; b<fit_data.size(); ++b)
    {
      int r=bootconfig[boot][b];
      boot_data[b]=fit_data[r];
    }

//    emit message_signal(int_to_QString(boot));

    string set_data_message;
    _fitter->set_data(fit_arguments, boot_data, set_data_message);

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

/*
    double chisqr=_fitter->get_chi_sqr();
    double dof=_fitter->get_dof()-_fitter->get_cut()-n_parameters_dof;
    if(dof<0.0)
    {
      dof=0.0;
    }
    if(steps_needed==max_steps+1)
    {
      stringstream message;
      message << "Bootstrap warning: Fit #" << boot+1 << " did not converge after " << max_steps << " iterations. " << "chi^2/dof = " << chisqr/dof;
      emit message_signal((message.str()).c_str());
    }
    else
    {
      stringstream message;
      message << "Bootstrap fit #" << boot+1 << " converged after " << steps_needed << " iterations. " << "chi^2/dof = " << chisqr/dof;
      emit message_signal((message.str()).c_str());
    }
*/

    if(steps_needed==max_steps+1)
    {
      stringstream message;
      message << "Bootstrap warning: Fit #" << boot+1 << " did not converge after " << max_steps << " iterations." << endl;
      emit message_signal((message.str()).c_str());
    }


    for(int p=0; p<n_parameters; ++p)
    {
      boot_results[boot][p]=_fitter->get_parameter(p);
      *all_output[p] << boot_results[boot][p] << endl;
    }
    emit step(boot);
  }

  for(int p=0; p<n_parameters; ++p)
  {
    all_output[p] -> close();
    delete all_output[p];
  }

  gsl_rng_free(rng);

  stringstream message;
  message << "Bootstrap with " << bssamples << " ensembles completed.";
  emit message_signal((message.str()).c_str());

  boot_av.resize(n_parameters, 0.0);
  boot_sigma.resize(n_parameters, 0.0);

  for(int p=0; p<n_parameters; ++p)
  {
    for(int boot=0; boot<bssamples; ++boot)
    {
      boot_av[p]+=boot_results[boot][p];
    }
    boot_av[p]/=bssamples;
  }

  double normalization=1.0;
  if(bootstrap_normalization)
  {
    normalization=sqrt(double(fit_data.size()));
  }

  for(int p=0; p<n_parameters; ++p)
  {
    vector< double > range_temp(bssamples, 0.0);
    for(int boot=0; boot<bssamples; ++boot)
    {
      range_temp[boot]=boot_results[boot][p];
    }

    stable_sort(range_temp.begin(), range_temp.end());
    boot_sigma[p]=(range_temp[static_cast<int>(0.841345*bssamples)]-range_temp[static_cast<int>(0.158655*bssamples)])/2.0;

    stringstream message1;
    message1 << parameter_names[p] << ":   average = " << boot_av[p] << "   sigma = " << normalization*boot_sigma[p] << endl << "Output file = " << all_filenames[p];
    emit message_signal((message1.str()).c_str());
  }

  emit message_signal("\n");
}
