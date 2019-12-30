void MainWindow::loadFile(const QString &fileName)
{
  settingsmap m;

  QApplication::setOverrideCursor(Qt::WaitCursor);
  if(!m.load_file(fileName.toStdString()))
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this, tr("QMBF"),
                         tr("Cannot read file %1:\n")
                         .arg(fileName));
    return;
  }

  if(m.exists("version"))
  {
    double v=m.get_double("version");
    if(v>version)
    {
      QApplication::restoreOverrideCursor();
      int ret=QMessageBox::warning(this, tr("QMBF"),
                                   tr("Warning: %1 was saved with a newer QMBF version: %2\n This QMBF version is %3\n Do you want to continue loading the file?").arg(strippedName(fileName)).arg(v).arg(version),
                                   QMessageBox::Yes,
                                   QMessageBox::No  | QMessageBox::Default | QMessageBox::Escape);
      if(ret==QMessageBox::No)
      {
        return;
      }
      QApplication::setOverrideCursor(Qt::WaitCursor);
    }
  }

  if(!m.exists("nfunctions"))
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this, tr("QMBF"),
                         tr("%1 does not contain number of functions")
                         .arg(fileName));
    return;
  }

  if(!m.exists("nvariables"))
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this, tr("QMBF"),
                         tr("%1 does not contain number of variables")
                         .arg(fileName));
    return;
  }

  if(!m.exists("nparameters"))
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this, tr("QMBF"),
                         tr("%1 does not contain number of parameters")
                         .arg(fileName));
    return;
  }

  if(!m.exists("nconstants"))
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this, tr("QMBF"),
                         tr("%1 does not contain number of constants")
                         .arg(fileName));
    return;
  }

  int n_functions=m.get_int("nfunctions");
  int n_variables=m.get_int("nvariables");
  int n_parameters=m.get_int("nparameters");
  int n_constants=m.get_int("nconstants");

  funcWidget->set_n_functions(n_functions);
  varsWidget->set_n_variables(n_variables);
  parderWidget->set_n_parameters(n_parameters);
  constsWidget->set_n_constants(n_constants);

  if(m.exists("currentmodel"))
  {
    current_model=static_cast<model>(m.get_int("currentmodel"));
    check_current_model();
    need_to_reset_fitter=true;
  }
  else
  {
    current_model=PARSE;
    check_current_model();
    need_to_reset_fitter=true;
  }

  if(current_model==PARSE)
  {
    for(int f=0; f<n_functions; ++f)
    {
       funcWidget->set_function(f, m.get_string("function", f));
    }
  }

  for(int v=0; v<n_variables; ++v)
  {
    if(current_model==PARSE) varsWidget->set_variable_name(v, m.get_string("variable", v));
    varsWidget->set_fit_min(v, m.get_string("fitmin", v));
    varsWidget->set_fit_max(v, m.get_string("fitmax", v));
  }

  if(current_model==PARSE)
  {
    for(int p=0; p<n_parameters; ++p)
    {
       parderWidget->set_parameter(p, m.get_string("parameter", p));
    }
  }

  for(int c=0; c<n_constants; ++c)
  {
    if(current_model==PARSE) constsWidget->set_constant_name(c, m.get_string("constant", c));
    if(m.exists("constantvalue", c))
    {
      double val=m.get_double("constantvalue", c);
      if(val!=empty_double)
      {
        constsWidget->set_constant_value(c, val);
      }
    }
  }

  if(current_model==PARSE)
  {
    for(int f=0; f<n_functions; ++f)
    {
      for(int p=0; p<n_parameters; ++p)
      {
        parderWidget->set_derivative(f, p, m.get_string("derivative", f, p));
      }
    }
  }

  if(m.exists("bayesian")) fitsetWidget->set_bayes(m.get_bool("bayesian"));

  if(m.exists("numdiff")) fitsetWidget->set_num_diff(m.get_bool("numdiff"));
  if(m.exists("secondderivminimization")) fitsetWidget->set_second_deriv_minimization(m.get_bool("secondderivminimization"));
  if(m.exists("secondderivcovariance")) fitsetWidget->set_second_deriv_covariance(m.get_bool("secondderivcovariance"));

  if(m.exists("numdiffstep"))
  {
    double num_diff_step=m.get_double("numdiffstep");
    if(num_diff_step!=empty_double)
    {
      fitsetWidget->set_num_diff_step(num_diff_step);
    }
  }

  if(m.exists("usebse"))
  {
    fitsetWidget->set_use_bse(m.get_bool("usebse"));
  }
  else
  {
    if(m.exists("bsefile"))
    {
      fitsetWidget->set_use_bse(true);
    }
    else
    {
      fitsetWidget->set_use_bse(false);
    }
  }
  if(m.exists("restrictdatarange")) fitsetWidget->set_restrict_data(m.get_bool("restrictdatarange"));
  if(m.exists("datarangemin")) fitsetWidget->set_data_start(m.get_int("datarangemin"));
  if(m.exists("datarangemax")) fitsetWidget->set_data_stop(m.get_int("datarangemax"));

  if(m.exists("startlambda"))
  {
    double startlambda=m.get_double("startlambda");
    if(startlambda!=empty_double)
    {
      fitsetWidget->set_startlambda(startlambda);
    }
  }

  if(m.exists("lambdafactor"))
  {
    double lambdafactor=m.get_double("lambdafactor");
    if(lambdafactor!=empty_double)
    {
      fitsetWidget->set_lambdafac(lambdafactor);
    }
  }

  if(m.exists("tolerance"))
  {
    double tolerance=m.get_double("tolerance");
    if(tolerance!=empty_double)
    {
      fitsetWidget->set_tolerance(tolerance);
    }
  }

  if(m.exists("svdcut"))
  {
    int svdcut=m.get_int("svdcut");
    if(svdcut!=empty_int)
    {
      fitsetWidget->set_svd(svdcut);
    }
  }

  if(m.exists("svdratio"))
  {
    double ratio=m.get_double("svdratio");
    if(ratio!=empty_double)
    {
      fitsetWidget->set_svd_ratio(ratio);
    }
  }

  if(m.exists("svdvalue"))
  {
    double value=m.get_double("svdvalue");
    if(value!=empty_double)
    {
      fitsetWidget->set_svd_value(value);
    }
  }

  if(m.exists("rescalevalue"))
  {
    double value=m.get_double("rescalevalue");
    if(value!=empty_double)
    {
      fitsetWidget->set_rescale_value(value);
    }
  }

  if(m.exists("inversionmethod"))
  {
    fitsetWidget->set_inversion_method(static_cast<inversion_method>(m.get_int("inversionmethod")));
  }
  else
  {
    if(fitsetWidget->get_svd()>0)
    {
      fitsetWidget->set_inversion_method(simple_cut);
    }
    else
    {
      fitsetWidget->set_inversion_method(LU_inversion);
    }
  }


  if(m.exists("maxsteps"))
  {
    int maxsteps=m.get_int("maxsteps");
    if(maxsteps!=empty_int)
    {
      fitsetWidget->set_steps(maxsteps);
    }
  }

  if(m.exists("binsize"))
  {
    int binsize=m.get_int("binsize");
    if(binsize!=empty_int)
    {
      fitsetWidget->set_bin(binsize);
    }
  }
  if(m.exists("bootstrapnormalization"))
  {
    bool bsn=m.get_bool("bootstrapnormalization");
    fitsetWidget->set_bootstrap_normalization(bsn);
    if(bsn)
    {
      fitsetWidget->set_bin(1);
    }
  }

  if(m.exists("bssamples"))
  {
    int bssamples=m.get_int("bssamples");
    if(bssamples!=empty_int)
    {
      fitsetWidget->set_bssamples(bssamples);
    }
  }

  if(m.exists("bsefile")) fitsetWidget->set_boot_file(m.get_string("bsefile"));

  if(m.exists("bootprior")) fitsetWidget->set_boot_prior(m.get_bool("bootprior"));
  if(m.exists("datafiletype"))
  {
    mainWidget->set_data_file_type(m.get_int("datafiletype"));
  }
  mainWidget->set_data_file(m.get_string("datafile"));
  mainWidget->set_output_dir(m.get_string("outputdir"));

  if(m.exists("multiexpdialog:nexp")) m_exp_dialog->set_n_exp(m.get_int("multiexpdialog:nexp"));
  if(m.exists("multiexpdialog:BC")) m_exp_dialog->set_BC(m.get_bool("multiexpdialog:BC"));
  if(m.exists("multiexpdialog:constant")) m_exp_dialog->set_constant(m.get_bool("multiexpdialog:constant"));

  if(m.exists("multiexpvecdialog:nexp"))
  {
    m_exp_vec_dialog->set_n_exp(m.get_int("multiexpvecdialog:nexp"));
    m_exp_mat_dialog->set_n_exp(m.get_int("multiexpvecdialog:nexp"));  // for compatibility, overwritten later
  }
  if(m.exists("multiexpvecdialog:nvec"))
  {
    m_exp_vec_dialog->set_n_vec(m.get_int("multiexpvecdialog:nvec"));
    m_exp_mat_dialog->set_dim_1(m.get_int("multiexpvecdialog:nvec"));  // for compatibility, overwritten later
    m_exp_mat_dialog->set_dim_2(m.get_int("multiexpvecdialog:nvec"));  // for compatibility, overwritten later
  }
  if(m.exists("multiexpvecdialog:BC"))
  {
    m_exp_vec_dialog->set_BC(m.get_bool("multiexpvecdialog:BC"));
  }
  if(m.exists("multiexpvecdialog:constant"))
  {
    m_exp_vec_dialog->set_constant(m.get_bool("multiexpvecdialog:constant"));
  }

  if(m.exists("multiexpmatdialog:nexp")) m_exp_mat_dialog->set_n_exp(m.get_int("multiexpmatdialog:nexp"));
  if(m.exists("multiexpmatdialog:dim1"))
  {
    m_exp_mat_dialog->set_dim_1(m.get_int("multiexpmatdialog:dim1"));
  }
  if(m.exists("multiexpmatdialog:dim2"))
  {
    m_exp_mat_dialog->set_dim_2(m.get_int("multiexpmatdialog:dim2"));
  }

  if(m.exists("multiexpmatupperdialog:nexp")) m_exp_mat_upper_dialog->set_n_exp(m.get_int("multiexpmatupperdialog:nexp"));
  if(m.exists("multiexpmatupperdialog:dim"))
  {
    m_exp_mat_upper_dialog->set_dim(m.get_int("multiexpmatupperdialog:dim"));
  }

  if(m.exists("multialtexpdialog:nexp")) m_alt_exp_dialog->set_n_exp(m.get_int("multialtexpdialog:nexp"));
  if(m.exists("multialtexpdialog:mexp")) m_alt_exp_dialog->set_m_exp(m.get_int("multialtexpdialog:mexp"));
  if(m.exists("multialtexpdialog:BC")) m_alt_exp_dialog->set_BC(m.get_bool("multialtexpdialog:BC"));
  if(m.exists("multialtexpdialog:constant")) m_alt_exp_dialog->set_constant(m.get_bool("multialtexpdialog:constant"));

  if(m.exists("multialtexpvecdialog:nexp"))
  {
    m_alt_exp_vec_dialog->set_n_exp(m.get_int("multialtexpvecdialog:nexp"));
    m_alt_exp_mat_dialog->set_n_exp(m.get_int("multialtexpvecdialog:nexp"));  // for compatibility, overwritten later
  }
  if(m.exists("multialtexpvecdialog:mexp"))
  {
    m_alt_exp_vec_dialog->set_m_exp(m.get_int("multialtexpvecdialog:mexp"));
    m_alt_exp_mat_dialog->set_m_exp(m.get_int("multialtexpvecdialog:mexp"));  // for compatibility, overwritten later
  }
  if(m.exists("multialtexpvecdialog:nvec"))
  {
    m_alt_exp_vec_dialog->set_n_vec(m.get_int("multialtexpvecdialog:nvec"));
    m_alt_exp_mat_dialog->set_dim_1(m.get_int("multialtexpvecdialog:nvec"));  // for compatibility, overwritten later
    m_alt_exp_mat_dialog->set_dim_2(m.get_int("multialtexpvecdialog:nvec"));  // for compatibility, overwritten later
  }
  if(m.exists("multialtexpvecdialog:BC"))
  {
    m_alt_exp_vec_dialog->set_BC(m.get_bool("multialtexpvecdialog:BC"));
  }
  if(m.exists("multialtexpvecdialog:constant"))
  {
    m_alt_exp_vec_dialog->set_constant(m.get_bool("multialtexpvecdialog:constant"));
  }

  if(m.exists("multialtexpmatdialog:nexp")) m_alt_exp_mat_dialog->set_n_exp(m.get_int("multialtexpmatdialog:nexp"));
  if(m.exists("multialtexpmatdialog:mexp")) m_alt_exp_mat_dialog->set_m_exp(m.get_int("multialtexpmatdialog:mexp"));
  if(m.exists("multialtexpmatdialog:dim1"))
  {
    m_alt_exp_mat_dialog->set_dim_1(m.get_int("multialtexpmatdialog:dim1"));
  }
  if(m.exists("multialtexpmatdialog:dim2"))
  {
    m_alt_exp_mat_dialog->set_dim_2(m.get_int("multialtexpmatdialog:dim2"));
  }

  if(m.exists("threeptmultiexpdialog:nexpinitial")) threept_m_exp_dialog->set_n_exp_initial(m.get_int("threeptmultiexpdialog:nexpinitial"));
  if(m.exists("threeptmultiexpdialog:nexpfinal")) threept_m_exp_dialog->set_n_exp_final(m.get_int("threeptmultiexpdialog:nexpfinal"));

  if(m.exists("threeptmultiexpvecdialog:nexpinitial")) threept_m_exp_vec_dialog->set_n_exp_initial(m.get_int("threeptmultiexpvecdialog:nexpinitial"));
  if(m.exists("threeptmultiexpvecdialog:nexpfinal")) threept_m_exp_vec_dialog->set_n_exp_final(m.get_int("threeptmultiexpvecdialog:nexpfinal"));
  if(m.exists("threeptmultiexpvecdialog:nvec")) threept_m_exp_vec_dialog->set_n_vec(m.get_int("threeptmultiexpvecdialog:nvec"));

  if(m.exists("threeptmultialtexpdialog:nexpinitial")) threept_m_alt_exp_dialog->set_n_exp_initial(m.get_int("threeptmultialtexpdialog:nexpinitial"));
  if(m.exists("threeptmultialtexpdialog:nexpfinal")) threept_m_alt_exp_dialog->set_n_exp_final(m.get_int("threeptmultialtexpdialog:nexpfinal"));
  if(m.exists("threeptmultialtexpdialog:mexpinitial")) threept_m_alt_exp_dialog->set_m_exp_initial(m.get_int("threeptmultialtexpdialog:mexpinitial"));
  if(m.exists("threeptmultialtexpdialog:mexpfinal")) threept_m_alt_exp_dialog->set_m_exp_final(m.get_int("threeptmultialtexpdialog:mexpfinal"));

  if(m.exists("threeptmultialtexpvecdialog:nexpinitial")) threept_m_alt_exp_vec_dialog->set_n_exp_initial(m.get_int("threeptmultialtexpvecdialog:nexpinitial"));
  if(m.exists("threeptmultialtexpvecdialog:nexpfinal")) threept_m_alt_exp_vec_dialog->set_n_exp_final(m.get_int("threeptmultialtexpvecdialog:nexpfinal"));
  if(m.exists("threeptmultialtexpvecdialog:mexpinitial")) threept_m_alt_exp_vec_dialog->set_m_exp_initial(m.get_int("threeptmultialtexpvecdialog:mexpinitial"));
  if(m.exists("threeptmultialtexpvecdialog:mexpfinal")) threept_m_alt_exp_vec_dialog->set_m_exp_final(m.get_int("threeptmultialtexpvecdialog:mexpfinal"));
  if(m.exists("threeptmultialtexpvecdialog:nvec")) threept_m_alt_exp_vec_dialog->set_n_vec(m.get_int("threeptmultialtexpvecdialog:nvec"));

  if(m.exists("grace")) plot_set_dialog->set_grace(m.get_string("grace"));

  if(m.exists("plotting:viewplot")) plot_set_dialog->set_viewplot(m.get_int("plotting:viewplot"));

  if(m.exists("effmassparam")) plot_set_dialog->set_eff_mass(m.get_string("effmassparam"));

  if(m.exists("plotmin"))
  {
    double plotmin=m.get_double("plotmin");
    if(plotmin!=empty_double)
    {
      plot_set_dialog->set_min(plotmin);
    }
  }
  if(m.exists("plotmax"))
  {
    double plotmax=m.get_double("plotmax");
    if(plotmax!=empty_double)
    {
      plot_set_dialog->set_max(plotmax);
    }
  }
  if(m.exists("plotminy"))
  {
    double plotminy=m.get_double("plotminy");
    if(plotminy!=empty_double)
    {
      plot_set_dialog->set_min_y(plotminy);
    }
  }
  if(m.exists("plotmaxy"))
  {
    double plotmaxy=m.get_double("plotmaxy");
    if(plotmaxy!=empty_double)
    {
      plot_set_dialog->set_max_y(plotmaxy);
    }
  }
  if(m.exists("plotminemy"))
  {
    double plotminemy=m.get_double("plotminemy");
    if(plotminemy!=empty_double)
    {
      plot_set_dialog->set_min_em_y(plotminemy);
    }
  }
  if(m.exists("plotmaxemy"))
  {
    double plotmaxemy=m.get_double("plotmaxemy");
    if(plotmaxemy!=empty_double)
    {
      plot_set_dialog->set_max_em_y(plotmaxemy);
    }
  }
  if(m.exists("plotting:continuous")) plot_set_dialog->set_continuous(m.get_bool("plotting:continuous"));
  if(m.exists("plotting:datarange")) plot_set_dialog->set_data_range(m.get_bool("plotting:datarange"));
  if(m.exists("plotting:datarangey")) plot_set_dialog->set_data_range_y(m.get_bool("plotting:datarangey"));
  if(m.exists("plotting:ylog")) plot_set_dialog->set_log(m.get_bool("plotting:ylog"));

  if(m.exists("plotting:negative")) plot_set_dialog->set_neg(m.get_bool("plotting:negative"));
  if(m.exists("plotting:plotvar")) plot_set_dialog->set_plot_var(m.get_int("plotting:plotvar"));

  for(int v=0; v<n_variables; ++v)
  {
    if(m.exists("plotting:plotvalue", v))
    {
      double value=m.get_double("plotting:plotvalue", v);
      if(value!=empty_double)
      {
        plot_set_dialog->set_variable_value(v, value);
      }
    }
  }


  if(    (current_model==MULTIEXP)
      || (current_model==MULTIEXPASQR)
      || (current_model==MULTIEXPASQREXPE)
//      || (current_model==MULTIEXPEXPA)
      || (current_model==MULTIEXPEXPE)
//      || (current_model==MULTIEXPEXPAEXPE)
      || (current_model==MULTIALTEXP)
      || (current_model==MULTIALTEXPASQR)
      || (current_model==MULTIALTEXPASQREXPE)
//      || (current_model==MULTIALTEXPEXPA)
      || (current_model==MULTIALTEXPEXPE)
//      || (current_model==MULTIALTEXPEXPAEXPE)

      || (current_model==MULTIEXPVEC)
      || (current_model==MULTIEXPASQRVEC)
      || (current_model==MULTIEXPASQREXPEVEC)
//      || (current_model==MULTIEXPEXPAVEC)
      || (current_model==MULTIEXPEXPEVEC)
//      || (current_model==MULTIEXPEXPAEXPEVEC)
      || (current_model==MULTIALTEXPVEC)
      || (current_model==MULTIALTEXPASQRVEC)
      || (current_model==MULTIALTEXPASQREXPEVEC)
//      || (current_model==MULTIALTEXPEXPAVEC)
      || (current_model==MULTIALTEXPEXPEVEC)
//      || (current_model==MULTIALTEXPEXPAEXPEVEC)

      || (current_model==MULTIEXPMAT)
      || (current_model==MULTIEXPEXPEMAT)
      || (current_model==MULTIALTEXPMAT)
      || (current_model==MULTIALTEXPEXPEMAT)

      || (current_model==MULTIEXPBC)
      || (current_model==MULTIEXPASQRBC)
      || (current_model==MULTIEXPASQREXPEBC)
//      || (current_model==MULTIEXPEXPABC)
      || (current_model==MULTIEXPEXPEBC)
//      || (current_model==MULTIEXPEXPAEXPEBC)
      || (current_model==MULTIALTEXPBC)
      || (current_model==MULTIALTEXPASQRBC)
      || (current_model==MULTIALTEXPASQREXPEBC)
//      || (current_model==MULTIALTEXPEXPABC)
      || (current_model==MULTIALTEXPEXPEBC)
//      || (current_model==MULTIALTEXPEXPAEXPEBC)

      || (current_model==MULTIEXPVECBC)
      || (current_model==MULTIEXPASQRVECBC)
      || (current_model==MULTIEXPASQREXPEVECBC)
//      || (current_model==MULTIEXPEXPAVECBC)
      || (current_model==MULTIEXPEXPEVECBC)
//      || (current_model==MULTIEXPEXPAEXPEVECBC)
      || (current_model==MULTIALTEXPVECBC)
      || (current_model==MULTIALTEXPASQRVECBC)
      || (current_model==MULTIALTEXPASQREXPEVECBC)
//      || (current_model==MULTIALTEXPEXPAVECBC)
      || (current_model==MULTIALTEXPEXPEVECBC)
//      || (current_model==MULTIALTEXPEXPAEXPEVECBC)

      || (current_model==THREEPT2VARMULTIEXP)
      || (current_model==THREEPT2VARMULTIEXPVEC)

      || (current_model==THREEPT2VARMULTIALTEXP)
      || (current_model==THREEPT2VARMULTIALTEXPVEC)

      || (current_model==THREEPT2VARMULTIEXPEXPE)
      || (current_model==THREEPT2VARMULTIALTEXPEXPE)
      || (current_model==THREEPT2VARMULTIEXPEXPEVEC)
      || (current_model==THREEPT2VARMULTIALTEXPEXPEVEC )

      || (current_model==MULTIEXPNONSYMMAT)
      || (current_model==MULTIEXPEXPENONSYMMAT)
      || (current_model==MULTIALTEXPNONSYMMAT)
      || (current_model==MULTIALTEXPEXPENONSYMMAT)

      || (current_model==MULTIEXPCONST)
      || (current_model==MULTIEXPASQRCONST)
      || (current_model==MULTIEXPEXPECONST)
      || (current_model==MULTIALTEXPCONST)
      || (current_model==MULTIALTEXPASQRCONST)
      || (current_model==MULTIALTEXPEXPECONST)
      || (current_model==MULTIEXPVECCONST)
      || (current_model==MULTIEXPASQRVECCONST)
      || (current_model==MULTIEXPEXPEVECCONST)
      || (current_model==MULTIALTEXPVECCONST)
      || (current_model==MULTIALTEXPASQRVECCONST)
      || (current_model==MULTIALTEXPEXPEVECCONST)
      || (current_model==MULTIEXPBCCONST)
      || (current_model==MULTIEXPASQRBCCONST)
      || (current_model==MULTIEXPEXPEBCCONST)
      || (current_model==MULTIALTEXPBCCONST)
      || (current_model==MULTIALTEXPASQRBCCONST)
      || (current_model==MULTIALTEXPEXPEBCCONST)
      || (current_model==MULTIEXPVECBCCONST)
      || (current_model==MULTIEXPASQRVECBCCONST)
      || (current_model==MULTIEXPEXPEVECBCCONST)
      || (current_model==MULTIALTEXPVECBCCONST)
      || (current_model==MULTIALTEXPASQRVECBCCONST)
      || (current_model==MULTIALTEXPEXPEVECBCCONST)
      || (current_model==MULTIEXPASQREXPECONST)
      || (current_model==MULTIEXPASQREXPEBCCONST)
      || (current_model==MULTIALTEXPASQREXPECONST)
      || (current_model==MULTIALTEXPASQREXPEBCCONST)
      || (current_model==MULTIEXPASQREXPEVECCONST)
      || (current_model==MULTIEXPASQREXPEVECBCCONST)
      || (current_model==MULTIALTEXPASQREXPEVECCONST)
      || (current_model==MULTIALTEXPASQREXPEVECBCCONST)
      || (current_model==MULTIEXPMATII)
      || (current_model==MULTIEXPEXPEMATII)
      || (current_model==MULTIEXPMATIIUPPER)
      || (current_model==MULTIEXPEXPEMATIIUPPER)
      || (current_model==MULTIEXPMATUPPER)
      || (current_model==MULTIEXPEXPEMATUPPER)
  )
  {
    reset_fitter();
    update_model_widgets();
  }

  for(int p=0; p<n_parameters; ++p)
  {
    if(m.exists("startval", p))
    {
      double start_val=m.get_double("startval", p);
      if(start_val!=empty_double)
      {
        mainWidget->set_start_value(p, start_val);
      }
    }

    if(m.exists("prior", p))
    {
      double prior=m.get_double("prior", p);
      if(prior!=empty_double)
      {
        mainWidget->set_prior(p, prior);
      }
    }

    if(m.exists("sigma", p))
    {
      double sigma=m.get_double("sigma", p);
      if(sigma!=empty_double)
      {
        mainWidget->set_sigma(p, sigma);
      }
    }
  }


  if(m.exists("nparametersdof"))
  {
    parderWidget->set_n_parameters_dof(m.get_int("nparametersdof"));
  }

  if(m.exists("chisqrextraterm:enabled"))
  {
    chisqrextraWidget->set_enabled(m.get_bool("chisqrextraterm:enabled"));
    if(m.exists("chisqrextraterm:function")) chisqrextraWidget->set_function(m.get_string("chisqrextraterm:function"));
    if(m.exists("chisqrextraterm:numdiffstep")) chisqrextraWidget->set_num_diff_step(m.get_double("chisqrextraterm:numdiffstep"));

    if(!m.exists("chisqrextraterm:nconstants"))
    {
      QApplication::restoreOverrideCursor();
      QMessageBox::warning(this, tr("QMBF"),
                           tr("%1 does not contain number of chi^2 extra term constants")
                           .arg(fileName));
      return;
    }
    chisqrextraWidget->set_n_constants(m.get_int("chisqrextraterm:nconstants"));
    for(int c=0; c<m.get_int("chisqrextraterm:nconstants"); ++c)
    {
      chisqrextraWidget->set_constant_name(c, m.get_string("chisqrextraterm:constant", c));
      if(m.exists("chisqrextraterm:constantvalue", c))
      {
        double val=m.get_double("chisqrextraterm:constantvalue", c);
        if(val!=empty_double)
        {
          chisqrextraWidget->set_constant_value(c, val);
        }
      }
    }
  }


  QApplication::restoreOverrideCursor();
  setCurrentFile(fileName);
}


bool MainWindow::saveFile(const QString &fileName)
{
  settingsmap m;

  int n_functions=funcWidget->get_n_functions();
  int n_variables=varsWidget->get_n_variables();
  int n_parameters=parderWidget->get_n_parameters();
  int n_constants=constsWidget->get_n_constants();

  m.set_double("version", version);

  m.set_int("currentmodel", current_model);

  m.set_int("nfunctions", n_functions);
  m.set_int("nvariables", n_variables);
  m.set_int("nparameters", n_parameters);
  m.set_int("nconstants", n_constants);

  m.set_int("nparametersdof", parderWidget->get_n_parameters_dof());

  for(int f=0; f<n_functions; ++f)
  {
    m.set_string("function", f, funcWidget->get_function(f));
  }

  for(int v=0; v<n_variables; ++v)
  {
    m.set_string("variable", v, varsWidget->get_variable_name(v));
    m.set_string("fitmin", v, varsWidget->get_fit_min(v));
    m.set_string("fitmax", v, varsWidget->get_fit_max(v));
  }

  for(int p=0; p<n_parameters; ++p)
  {
    m.set_string("parameter", p, parderWidget->get_parameter(p));
    m.set_double("startval", p, mainWidget->get_start_value(p));
    m.set_double("prior", p, mainWidget->get_prior(p));
    m.set_double("sigma", p, mainWidget->get_sigma(p));
  }

  for(int c=0; c<n_constants; ++c)
  {
    m.set_string("constant", c, constsWidget->get_constant_name(c));
    m.set_double("constantvalue", c, constsWidget->get_constant_value(c));
  }

  if(current_model==PARSE)
  {
    for(int f=0; f<n_functions; ++f)
    {
      for(int p=0; p<n_parameters; ++p)
      {
        m.set_string("derivative", f, p, parderWidget->get_derivative(f, p));
      }
    }
  }

  m.set_bool("bayesian", fitsetWidget->get_bayes());
  m.set_bool("numdiff", fitsetWidget->get_num_diff());
  m.set_bool("secondderivminimization", fitsetWidget->get_second_deriv_minimization());
  m.set_bool("secondderivcovariance", fitsetWidget->get_second_deriv_covariance());
  m.set_double("numdiffstep", fitsetWidget->get_num_diff_step());
  m.set_bool("usebse", fitsetWidget->get_use_bse());
  m.set_bool("restrictdatarange", fitsetWidget->get_restrict_data());
  m.set_int("datarangemin", fitsetWidget->get_data_start());
  m.set_int("datarangemax", fitsetWidget->get_data_stop());
  m.set_double("startlambda", fitsetWidget->get_startlambda());
  m.set_double("lambdafactor", fitsetWidget->get_lambdafac());
  m.set_double("tolerance", fitsetWidget->get_tolerance());

  m.set_int("inversionmethod", fitsetWidget->get_inversion_method());
  m.set_double("svdratio", fitsetWidget->get_svd_ratio());
  m.set_double("svdvalue", fitsetWidget->get_svd_value());
  m.set_int("svdcut", fitsetWidget->get_svd());
  m.set_double("rescalevalue", fitsetWidget->get_rescale_value());

  m.set_bool("bootstrapnormalization", fitsetWidget->get_bootstrap_normalization());

  m.set_int("maxsteps", fitsetWidget->get_steps());
  m.set_int("binsize", fitsetWidget->get_bin());
  m.set_int("bssamples", fitsetWidget->get_bssamples());
  m.set_string("bsefile", fitsetWidget->get_boot_file());
  m.set_bool("bootprior", fitsetWidget->get_boot_prior());
  m.set_int("datafiletype", mainWidget->get_data_file_type());
  m.set_string("datafile", mainWidget->get_data_file());
  m.set_string("outputdir", mainWidget->get_output_dir());

  m.set_int("multiexpdialog:nexp", m_exp_dialog->get_n_exp());
  m.set_bool("multiexpdialog:BC", m_exp_dialog->get_BC());
  m.set_bool("multiexpdialog:constant", m_exp_dialog->get_constant());

  m.set_int("multiexpvecdialog:nexp", m_exp_vec_dialog->get_n_exp());
  m.set_int("multiexpvecdialog:nvec", m_exp_vec_dialog->get_n_vec());
  m.set_bool("multiexpvecdialog:BC", m_exp_vec_dialog->get_BC());
  m.set_bool("multiexpvecdialog:constant", m_exp_vec_dialog->get_constant());

  m.set_int("multiexpmatdialog:nexp", m_exp_mat_dialog->get_n_exp());
  m.set_int("multiexpmatdialog:dim1", m_exp_mat_dialog->get_dim_1());
  m.set_int("multiexpmatdialog:dim2", m_exp_mat_dialog->get_dim_2());

  m.set_int("multiexpmatupperdialog:nexp", m_exp_mat_upper_dialog->get_n_exp());
  m.set_int("multiexpmatupperdialog:dim", m_exp_mat_upper_dialog->get_dim());

  m.set_int("multialtexpdialog:nexp", m_alt_exp_dialog->get_n_exp());
  m.set_int("multialtexpdialog:mexp", m_alt_exp_dialog->get_m_exp());
  m.set_bool("multialtexpdialog:BC", m_alt_exp_dialog->get_BC());
  m.set_bool("multialtexpdialog:constant", m_alt_exp_dialog->get_constant());

  m.set_int("multialtexpvecdialog:nexp", m_alt_exp_vec_dialog->get_n_exp());
  m.set_int("multialtexpvecdialog:mexp", m_alt_exp_vec_dialog->get_m_exp());
  m.set_int("multialtexpvecdialog:nvec", m_alt_exp_vec_dialog->get_n_vec());
  m.set_bool("multialtexpvecdialog:BC", m_alt_exp_vec_dialog->get_BC());
  m.set_bool("multialtexpvecdialog:constant", m_alt_exp_vec_dialog->get_constant());

  m.set_int("multialtexpmatdialog:nexp", m_alt_exp_mat_dialog->get_n_exp());
  m.set_int("multialtexpmatdialog:mexp", m_alt_exp_mat_dialog->get_m_exp());
  m.set_int("multialtexpmatdialog:dim1", m_alt_exp_mat_dialog->get_dim_1());
  m.set_int("multialtexpmatdialog:dim2", m_alt_exp_mat_dialog->get_dim_2());

  m.set_int("threeptmultiexpdialog:nexpinitial", threept_m_exp_dialog->get_n_exp_initial());
  m.set_int("threeptmultiexpdialog:nexpfinal", threept_m_exp_dialog->get_n_exp_final());

  m.set_int("threeptmultiexpvecdialog:nexpinitial", threept_m_exp_vec_dialog->get_n_exp_initial());
  m.set_int("threeptmultiexpvecdialog:nexpfinal", threept_m_exp_vec_dialog->get_n_exp_final());
  m.set_int("threeptmultiexpvecdialog:nvec", threept_m_exp_vec_dialog->get_n_vec());

  m.set_int("threeptmultialtexpdialog:nexpinitial", threept_m_alt_exp_dialog->get_n_exp_initial());
  m.set_int("threeptmultialtexpdialog:nexpfinal", threept_m_alt_exp_dialog->get_n_exp_final());
  m.set_int("threeptmultialtexpdialog:mexpinitial", threept_m_alt_exp_dialog->get_m_exp_initial());
  m.set_int("threeptmultialtexpdialog:mexpfinal", threept_m_alt_exp_dialog->get_m_exp_final());

  m.set_int("threeptmultialtexpvecdialog:nexpinitial", threept_m_alt_exp_vec_dialog->get_n_exp_initial());
  m.set_int("threeptmultialtexpvecdialog:nexpfinal", threept_m_alt_exp_vec_dialog->get_n_exp_final());
  m.set_int("threeptmultialtexpvecdialog:mexpinitial", threept_m_alt_exp_vec_dialog->get_m_exp_initial());
  m.set_int("threeptmultialtexpvecdialog:mexpfinal", threept_m_alt_exp_vec_dialog->get_m_exp_final());
  m.set_int("threeptmultialtexpvecdialog:nvec", threept_m_alt_exp_vec_dialog->get_n_vec());


  m.set_string("grace", plot_set_dialog->get_grace());
  m.set_int("plotting:viewplot", plot_set_dialog->get_viewplot());
  m.set_string("effmassparam", plot_set_dialog->get_eff_mass());
  m.set_double("plotmin", plot_set_dialog->get_min());
  m.set_double("plotmax", plot_set_dialog->get_max());
  m.set_double("plotminy", plot_set_dialog->get_min_y());
  m.set_double("plotmaxy", plot_set_dialog->get_max_y());
  m.set_double("plotminemy", plot_set_dialog->get_min_em_y());
  m.set_double("plotmaxemy", plot_set_dialog->get_max_em_y());
  m.set_bool("plotting:continuous", plot_set_dialog->continuous());
  m.set_bool("plotting:datarange", plot_set_dialog->data_range());
  m.set_bool("plotting:datarangey", plot_set_dialog->data_range_y());
  m.set_bool("plotting:ylog", plot_set_dialog->get_log());
  m.set_bool("plotting:negative", plot_set_dialog->get_neg());
  m.set_int("plotting:plotvar", plot_set_dialog->get_plot_var());
  for(int v=0; v<n_variables; ++v)
  {
    m.set_double("plotting:plotvalue", v, plot_set_dialog->get_variable_value(v));
  }

  m.set_bool("chisqrextraterm:enabled", chisqrextraWidget->get_enabled());
  m.set_string("chisqrextraterm:function", chisqrextraWidget->get_function());
  m.set_double("chisqrextraterm:numdiffstep", chisqrextraWidget->get_num_diff_step());
  m.set_int("chisqrextraterm:nconstants", chisqrextraWidget->get_n_constants());
  for(int c=0; c<chisqrextraWidget->get_n_constants(); ++c)
  {
    m.set_string("chisqrextraterm:constant", c, chisqrextraWidget->get_constant_name(c));
    m.set_double("chisqrextraterm:constantvalue", c, chisqrextraWidget->get_constant_value(c));
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  if(!m.save_file(fileName.toStdString()))
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this, tr("QMBF"),
                         tr("Cannot write file %1:\n")
                         .arg(fileName));
    return false;
  }
  QApplication::restoreOverrideCursor();
  setCurrentFile(fileName);
  return true;
}
