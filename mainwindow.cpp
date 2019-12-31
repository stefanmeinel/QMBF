#include "mainwindow.h"


bool _in(const string& sub, const string& s)
{
  if(s.find(sub)!=string::npos)
  {
    return true;
  }
  else
  {
    return false;
  }
}



// *******************************************************************
// constructor
// *******************************************************************
MainWindow::MainWindow(bool load_file, QString file_name)
{
  resize(1200, 700);

  createActions();
  createMenus();
  create_Widgets_Dialogs();

  setCentralWidget(tabWidget);

  _fitter=NULL;
  fitthread = new fit_thread;
  connect(fitthread, SIGNAL(fit_finished(int)), this, SLOT(fit_finished(int)));
  bootstrapthread = new bootstrap_thread();
  connect(bootstrapthread, SIGNAL(fit_finished(int)), this, SLOT(fit_finished(int)));
  connect(bootstrapthread, SIGNAL(finished()), this, SLOT(bootstrap_finished()));
  connect(bootstrapthread, SIGNAL(message_signal(QString)), mainWidget, SLOT(printm(QString)));
  connect(bootstrapthread, SIGNAL(step(int)), bootstrapdialog, SLOT(update_bar(int)));
  multifitthread = new multifit_thread();
  connect(multifitthread, SIGNAL(fit_finished(int)), this, SLOT(fit_finished(int)));
  connect(multifitthread, SIGNAL(finished()), this, SLOT(multifit_finished()));
  connect(multifitthread, SIGNAL(message_signal(QString)), mainWidget, SLOT(printm(QString)));
  connect(multifitthread, SIGNAL(step(int)), multifitdialog, SLOT(update_bar(int)));

  _gaussian_prior=NULL;
   _chisqr_extra_term=NULL;
  _model=NULL;

  reset();

  if(load_file)
  {
    current_dir=QFileInfo(file_name).absolutePath();
    emit lf(file_name);
  }

#ifdef Q_WS_MACX
  FileOpenFilter *file_open_filter = new FileOpenFilter();
  this -> installEventFilter(file_open_filter);
  connect(file_open_filter, SIGNAL(file_open(QString)), this, SLOT(loadFile(QString)));
#endif

  gsl_set_error_handler_off();
}


// *******************************************************************
// this function creates the actions for menu and toolbar
// *******************************************************************
void MainWindow::createActions()
{
  newAct=new QAction(QIcon(":/images/filenew.png"), tr("&New"), this);
  newAct->setShortcut(tr("Ctrl+N"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  openAct=new QAction(QIcon(":/images/fileopen.png"), tr("&Open..."), this);
  openAct->setShortcut(tr("Ctrl+O"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAct=new QAction(QIcon(":/images/filesave.png"), tr("&Save"), this);
  saveAct->setShortcut(tr("Ctrl+S"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  saveAsAct=new QAction(QIcon(":/images/filesaveas.png"), tr("Save &As..."), this);
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  exitAct=new QAction(QIcon(":/images/exit.png"), tr("&Quit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  parse_Act=new QAction(tr("&User-defined Model"), this);
  parse_Act->setCheckable(true);
  parse_Act->setChecked(true);
  connect(parse_Act, SIGNAL(triggered()), this, SLOT(parse_triggered()));
  connect(parse_Act, SIGNAL(toggled(bool)), this, SLOT(parse_toggled()));

  m_exp_Act=new QAction(tr("Multiple Exponentials..."), this);
  m_exp_Act->setCheckable(true);
  m_exp_Act->setChecked(false);
  connect(m_exp_Act, SIGNAL(triggered()), this, SLOT(show_m_exp()));

  m_exp_a_sqr_Act=new QAction(tr("Multiple Exponentials with Square Amplitudes..."), this);
  m_exp_a_sqr_Act->setCheckable(true);
  m_exp_a_sqr_Act->setChecked(false);
  connect(m_exp_a_sqr_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_a_sqr()));

  m_exp_a_sqr_exp_e_Act=new QAction(tr("Multiple Exponentials with Square Amplitudes and Exponential Energies..."), this);
  m_exp_a_sqr_exp_e_Act->setCheckable(true);
  m_exp_a_sqr_exp_e_Act->setChecked(false);
  connect(m_exp_a_sqr_exp_e_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_a_sqr_exp_e()));

//  m_exp_exp_a_Act=new QAction(tr("Multiple Exponentials with Exponential Amplitudes..."), this);
//  m_exp_exp_a_Act->setCheckable(true);
//  m_exp_exp_a_Act->setChecked(false);
//  connect(m_exp_exp_a_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_a()));

  m_exp_exp_e_Act=new QAction(tr("Multiple Exponentials with Exponential Energies..."), this);
  m_exp_exp_e_Act->setCheckable(true);
  m_exp_exp_e_Act->setChecked(false);
  connect(m_exp_exp_e_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_e()));

//  m_exp_exp_a_exp_e_Act=new QAction(tr("Multiple Exponentials with Exponential Amplitudes and Energies..."), this);
//  m_exp_exp_a_exp_e_Act->setCheckable(true);
//  m_exp_exp_a_exp_e_Act->setChecked(false);
//  connect(m_exp_exp_a_exp_e_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_a_exp_e()));

  m_alt_exp_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials..."), this);
  m_alt_exp_Act->setCheckable(true);
  m_alt_exp_Act->setChecked(false);
  connect(m_alt_exp_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp()));

  m_alt_exp_a_sqr_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Square Amplitudes..."), this);
  m_alt_exp_a_sqr_Act->setCheckable(true);
  m_alt_exp_a_sqr_Act->setChecked(false);
  connect(m_alt_exp_a_sqr_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_a_sqr()));

  m_alt_exp_a_sqr_exp_e_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Square Amplitudes and Exponential Energies..."), this);
  m_alt_exp_a_sqr_exp_e_Act->setCheckable(true);
  m_alt_exp_a_sqr_exp_e_Act->setChecked(false);
  connect(m_alt_exp_a_sqr_exp_e_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_a_sqr_exp_e()));

//  m_alt_exp_exp_a_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Amplitudes..."), this);
//  m_alt_exp_exp_a_Act->setCheckable(true);
//  m_alt_exp_exp_a_Act->setChecked(false);
//  connect(m_alt_exp_exp_a_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_exp_a()));

  m_alt_exp_exp_e_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Energies..."), this);
  m_alt_exp_exp_e_Act->setCheckable(true);
  m_alt_exp_exp_e_Act->setChecked(false);
  connect(m_alt_exp_exp_e_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_exp_e()));

//  m_alt_exp_exp_a_exp_e_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Energies and Amplitudes..."), this);
//  m_alt_exp_exp_a_exp_e_Act->setCheckable(true);
//  m_alt_exp_exp_a_exp_e_Act->setChecked(false);
//  connect(m_alt_exp_exp_a_exp_e_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_exp_a_exp_e()));

  m_exp_vec_Act=new QAction(tr("Multiple Exponentials..."), this);
  m_exp_vec_Act->setCheckable(true);
  m_exp_vec_Act->setChecked(false);
  connect(m_exp_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_vec()));

  m_exp_a_sqr_vec_Act=new QAction(tr("Multiple Exponentials with Square Amplitudes..."), this);
  m_exp_a_sqr_vec_Act->setCheckable(true);
  m_exp_a_sqr_vec_Act->setChecked(false);
  connect(m_exp_a_sqr_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_a_sqr_vec()));

  m_exp_a_sqr_exp_e_vec_Act=new QAction(tr("Multiple Exponentials with Square Amplitudes and Exponential Energies..."), this);
  m_exp_a_sqr_exp_e_vec_Act->setCheckable(true);
  m_exp_a_sqr_exp_e_vec_Act->setChecked(false);
  connect(m_exp_a_sqr_exp_e_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_a_sqr_exp_e_vec()));

//  m_exp_exp_a_vec_Act=new QAction(tr("Multiple Exponentials with Exponential Amplitudes..."), this);
//  m_exp_exp_a_vec_Act->setCheckable(true);
//  m_exp_exp_a_vec_Act->setChecked(false);
//  connect(m_exp_exp_a_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_a_vec()));

  m_exp_exp_e_vec_Act=new QAction(tr("Multiple Exponentials with Exponential Energies..."), this);
  m_exp_exp_e_vec_Act->setCheckable(true);
  m_exp_exp_e_vec_Act->setChecked(false);
  connect(m_exp_exp_e_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_e_vec()));

//  m_exp_exp_a_exp_e_vec_Act=new QAction(tr("Multiple Exponentials with Exponential Amplitudes and Energies..."), this);
//  m_exp_exp_a_exp_e_vec_Act->setCheckable(true);
//  m_exp_exp_a_exp_e_vec_Act->setChecked(false);
//  connect(m_exp_exp_a_exp_e_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_a_exp_e_vec()));

  m_alt_exp_vec_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials..."), this);
  m_alt_exp_vec_Act->setCheckable(true);
  m_alt_exp_vec_Act->setChecked(false);
  connect(m_alt_exp_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_vec()));

  m_alt_exp_a_sqr_vec_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Square Amplitudes..."), this);
  m_alt_exp_a_sqr_vec_Act->setCheckable(true);
  m_alt_exp_a_sqr_vec_Act->setChecked(false);
  connect(m_alt_exp_a_sqr_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_a_sqr_vec()));

  m_alt_exp_a_sqr_exp_e_vec_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Square Amplitudes and Exponential Energies..."), this);
  m_alt_exp_a_sqr_exp_e_vec_Act->setCheckable(true);
  m_alt_exp_a_sqr_exp_e_vec_Act->setChecked(false);
  connect(m_alt_exp_a_sqr_exp_e_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_a_sqr_exp_e_vec()));

//  m_alt_exp_exp_a_vec_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Amplitudes..."), this);
//  m_alt_exp_exp_a_vec_Act->setCheckable(true);
//  m_alt_exp_exp_a_vec_Act->setChecked(false);
//  connect(m_alt_exp_exp_a_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_exp_a_vec()));

  m_alt_exp_exp_e_vec_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Energies..."), this);
  m_alt_exp_exp_e_vec_Act->setCheckable(true);
  m_alt_exp_exp_e_vec_Act->setChecked(false);
  connect(m_alt_exp_exp_e_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_exp_e_vec()));

//  m_alt_exp_exp_a_exp_e_vec_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Energies and Amplitudes..."), this);
//  m_alt_exp_exp_a_exp_e_vec_Act->setCheckable(true);
//  m_alt_exp_exp_a_exp_e_vec_Act->setChecked(false);
//  connect(m_alt_exp_exp_a_exp_e_vec_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_exp_a_exp_e_vec()));

  m_exp_mat_Act=new QAction(tr("Multiple Exponentials..."), this);
  m_exp_mat_Act->setCheckable(true);
  m_exp_mat_Act->setChecked(false);
  connect(m_exp_mat_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_mat()));

  m_exp_exp_e_mat_Act=new QAction(tr("Multiple Exponentials with Exponential Energies..."), this);
  m_exp_exp_e_mat_Act->setCheckable(true);
  m_exp_exp_e_mat_Act->setChecked(false);
  connect(m_exp_exp_e_mat_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_e_mat()));

  m_exp_mat_II_Act=new QAction(tr("Multiple Exponentials..."), this);
  m_exp_mat_II_Act->setCheckable(true);
  m_exp_mat_II_Act->setChecked(false);
  connect(m_exp_mat_II_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_mat_II()));

  m_exp_exp_e_mat_II_Act=new QAction(tr("Multiple Exponentials with Exponential Energies..."), this);
  m_exp_exp_e_mat_II_Act->setCheckable(true);
  m_exp_exp_e_mat_II_Act->setChecked(false);
  connect(m_exp_exp_e_mat_II_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_e_mat_II()));

  m_exp_mat_upper_Act=new QAction(tr("Multiple Exponentials..."), this);
  m_exp_mat_upper_Act->setCheckable(true);
  m_exp_mat_upper_Act->setChecked(false);
  connect(m_exp_mat_upper_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_mat_upper()));

  m_exp_exp_e_mat_upper_Act=new QAction(tr("Multiple Exponentials with Exponential Energies..."), this);
  m_exp_exp_e_mat_upper_Act->setCheckable(true);
  m_exp_exp_e_mat_upper_Act->setChecked(false);
  connect(m_exp_exp_e_mat_upper_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_e_mat_upper()));

  m_exp_mat_II_upper_Act=new QAction(tr("Multiple Exponentials..."), this);
  m_exp_mat_II_upper_Act->setCheckable(true);
  m_exp_mat_II_upper_Act->setChecked(false);
  connect(m_exp_mat_II_upper_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_mat_II_upper()));

  m_exp_exp_e_mat_II_upper_Act=new QAction(tr("Multiple Exponentials with Exponential Energies..."), this);
  m_exp_exp_e_mat_II_upper_Act->setCheckable(true);
  m_exp_exp_e_mat_II_upper_Act->setChecked(false);
  connect(m_exp_exp_e_mat_II_upper_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_e_mat_II_upper()));

  m_alt_exp_mat_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials..."), this);
  m_alt_exp_mat_Act->setCheckable(true);
  m_alt_exp_mat_Act->setChecked(false);
  connect(m_alt_exp_mat_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_mat()));

  m_alt_exp_exp_e_mat_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Energies..."), this);
  m_alt_exp_exp_e_mat_Act->setCheckable(true);
  m_alt_exp_exp_e_mat_Act->setChecked(false);
  connect(m_alt_exp_exp_e_mat_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_exp_e_mat()));

  m_exp_nonsym_mat_Act=new QAction(tr("Multiple Exponentials..."), this);
  m_exp_nonsym_mat_Act->setCheckable(true);
  m_exp_nonsym_mat_Act->setChecked(false);
  connect(m_exp_nonsym_mat_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_nonsym_mat()));

  m_exp_exp_e_nonsym_mat_Act=new QAction(tr("Multiple Exponentials with Exponential Energies..."), this);
  m_exp_exp_e_nonsym_mat_Act->setCheckable(true);
  m_exp_exp_e_nonsym_mat_Act->setChecked(false);
  connect(m_exp_exp_e_nonsym_mat_Act, SIGNAL(triggered()), this, SLOT(show_m_exp_exp_e_nonsym_mat()));

  m_alt_exp_nonsym_mat_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials..."), this);
  m_alt_exp_nonsym_mat_Act->setCheckable(true);
  m_alt_exp_nonsym_mat_Act->setChecked(false);
  connect(m_alt_exp_nonsym_mat_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_nonsym_mat()));

  m_alt_exp_exp_e_nonsym_mat_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Energies..."), this);
  m_alt_exp_exp_e_nonsym_mat_Act->setCheckable(true);
  m_alt_exp_exp_e_nonsym_mat_Act->setChecked(false);
  connect(m_alt_exp_exp_e_nonsym_mat_Act, SIGNAL(triggered()), this, SLOT(show_m_alt_exp_exp_e_nonsym_mat()));

  threept_2var_m_exp_Act=new QAction(tr("Multiple Exponentials..."), this);
  threept_2var_m_exp_Act->setCheckable(true);
  threept_2var_m_exp_Act->setChecked(false);
  connect(threept_2var_m_exp_Act, SIGNAL(triggered()), this, SLOT(show_threept_2var_m_exp()));

  threept_2var_m_exp_exp_e_Act=new QAction(tr("Multiple Exponentials with Exponential Energies ..."), this);
  threept_2var_m_exp_exp_e_Act->setCheckable(true);
  threept_2var_m_exp_exp_e_Act->setChecked(false);
  connect(threept_2var_m_exp_exp_e_Act, SIGNAL(triggered()), this, SLOT(show_threept_2var_m_exp_exp_e()));

  threept_2var_m_exp_vec_Act=new QAction(tr("Multiple Exponentials..."), this);
  threept_2var_m_exp_vec_Act->setCheckable(true);
  threept_2var_m_exp_vec_Act->setChecked(false);
  connect(threept_2var_m_exp_vec_Act, SIGNAL(triggered()), this, SLOT(show_threept_2var_m_exp_vec()));

  threept_2var_m_exp_exp_e_vec_Act=new QAction(tr("Multiple Exponentials with Exponential Energies..."), this);
  threept_2var_m_exp_exp_e_vec_Act->setCheckable(true);
  threept_2var_m_exp_exp_e_vec_Act->setChecked(false);
  connect(threept_2var_m_exp_exp_e_vec_Act, SIGNAL(triggered()), this, SLOT(show_threept_2var_m_exp_exp_e_vec()));


  threept_2var_m_alt_exp_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials..."), this);
  threept_2var_m_alt_exp_Act->setCheckable(true);
  threept_2var_m_alt_exp_Act->setChecked(false);
  connect(threept_2var_m_alt_exp_Act, SIGNAL(triggered()), this, SLOT(show_threept_2var_m_alt_exp()));

  threept_2var_m_alt_exp_exp_e_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Energies..."), this);
  threept_2var_m_alt_exp_exp_e_Act->setCheckable(true);
  threept_2var_m_alt_exp_exp_e_Act->setChecked(false);
  connect(threept_2var_m_alt_exp_exp_e_Act, SIGNAL(triggered()), this, SLOT(show_threept_2var_m_alt_exp_exp_e()));

  threept_2var_m_alt_exp_vec_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials..."), this);
  threept_2var_m_alt_exp_vec_Act->setCheckable(true);
  threept_2var_m_alt_exp_vec_Act->setChecked(false);
  connect(threept_2var_m_alt_exp_vec_Act, SIGNAL(triggered()), this, SLOT(show_threept_2var_m_alt_exp_vec()));

  threept_2var_m_alt_exp_exp_e_vec_Act=new QAction(tr("Multiple (incl. oscillating) Exponentials with Exponential Energies..."), this);
  threept_2var_m_alt_exp_exp_e_vec_Act->setCheckable(true);
  threept_2var_m_alt_exp_exp_e_vec_Act->setChecked(false);
  connect(threept_2var_m_alt_exp_exp_e_vec_Act, SIGNAL(triggered()), this, SLOT(show_threept_2var_m_alt_exp_exp_e_vec()));


  plot_data_Act=new QAction(QIcon(":/images/plotdata.png"), tr("Plot averaged &Data"), this);
  connect(plot_data_Act, SIGNAL(triggered()), this, SLOT(plot_data()));
  plot_start_Act=new QAction(QIcon(":/images/plot_start.png"), tr("Plot averaged Data and &Start Functions"), this);
  connect(plot_start_Act, SIGNAL(triggered()), this, SLOT(plot_start()));
  plot_all_Act=new QAction(QIcon(":/images/qmbf.png"), tr("Plot averaged Data and &Fit Result"), this);
  plot_all_Act->setShortcut(tr("Ctrl+P"));
  connect(plot_all_Act, SIGNAL(triggered()), this, SLOT(plot_all()));
  plot_eff_mass_Act=new QAction(QIcon(":/images/eff_mass.png"), tr("&Effective Mass Plot"), this);
  connect(plot_eff_mass_Act, SIGNAL(triggered()), this, SLOT(plot_eff_mass()));

  plot_set_Act=new QAction(tr("&Plot Options..."), this);
  connect(plot_set_Act, SIGNAL(triggered()), this, SLOT(show_plot_settings()));

  start_to_prior_Act=new QAction(QIcon(":/images/ps.png"), tr("Set &Priors to Start Values"), this);
  connect(start_to_prior_Act, SIGNAL(triggered()), this, SLOT(start_to_prior()));
  result_to_prior_Act=new QAction(QIcon(":/images/pf.png"), tr("Set Priors to &Fit Results"), this);
  connect(result_to_prior_Act, SIGNAL(triggered()), this, SLOT(result_to_prior()));
  result_to_start_Act=new QAction(QIcon(":/images/sf.png"), tr("Set &Start Values to Fit Results"), this);
  connect(result_to_start_Act, SIGNAL(triggered()), this, SLOT(result_to_start()));

  chisqr_start_Act=new QAction(QIcon(":/images/chisqr.png"), tr("Compute c&hi^2/dof for Start Values"), this);
  connect(chisqr_start_Act, SIGNAL(triggered()), this, SLOT(chisqr_start()));

  reload_data_file_Act=new QAction(QIcon(":/images/reload.png"), tr("Reload &Data File"), this);
  connect(reload_data_file_Act, SIGNAL(triggered()), this, SLOT(reload_data_file()));

  report_Act=new QAction(QIcon(":/images/text_block.png"), tr("Show &Report"), this);
  connect(report_Act, SIGNAL(triggered()), this, SLOT(show_report()));

  write_results_Act=new QAction(QIcon(":/images/kget.png"), tr("Write Fit Results and &Covariance to Files"), this);
  connect(write_results_Act, SIGNAL(triggered()), this, SLOT(write_results()));

  multifit_Act=new QAction(QIcon(":/images/mf.png"), tr("&Multifit"), this);
  connect(multifit_Act, SIGNAL(triggered()), this, SLOT(multifit()));

  aboutAct=new QAction(QIcon(":/images/qmbf.png"), tr("&About"), this);
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(aboutWindow()));
}







// *******************************************************************
// this function creates the menu and toolbar
// *******************************************************************
void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  modelMenu = menuBar()->addMenu(tr("&Model"));
  modelMenu->addAction(parse_Act);
  modelMenu->addSeparator();
  scalarmodelMenu = modelMenu->addMenu(tr("2-Point &Correlator"));
  scalarmodelMenu->addAction(m_exp_Act);
  scalarmodelMenu->addAction(m_exp_exp_e_Act);
  scalarmodelMenu->addAction(m_exp_a_sqr_Act);
  scalarmodelMenu->addAction(m_exp_a_sqr_exp_e_Act);
//  scalarmodelMenu->addAction(m_exp_exp_a_Act);
//  scalarmodelMenu->addAction(m_exp_exp_a_exp_e_Act);
  scalarmodelMenu->addSeparator();
  scalarmodelMenu->addAction(m_alt_exp_Act);
  scalarmodelMenu->addAction(m_alt_exp_exp_e_Act);
  scalarmodelMenu->addAction(m_alt_exp_a_sqr_Act);
  scalarmodelMenu->addAction(m_alt_exp_a_sqr_exp_e_Act);
//  scalarmodelMenu->addAction(m_alt_exp_exp_a_Act);
//  scalarmodelMenu->addAction(m_alt_exp_exp_a_exp_e_Act);
  vectormodelMenu = modelMenu->addMenu(tr("2-Point Correlator, &Vector Fit"));
  vectormodelMenu->addAction(m_exp_vec_Act);
  vectormodelMenu->addAction(m_exp_exp_e_vec_Act);
  vectormodelMenu->addAction(m_exp_a_sqr_vec_Act);
  vectormodelMenu->addAction(m_exp_a_sqr_exp_e_vec_Act);
//  vectormodelMenu->addAction(m_exp_exp_a_vec_Act);
//  vectormodelMenu->addAction(m_exp_exp_a_exp_e_vec_Act);
  vectormodelMenu->addSeparator();
  vectormodelMenu->addAction(m_alt_exp_vec_Act);
  vectormodelMenu->addAction(m_alt_exp_exp_e_vec_Act);
  vectormodelMenu->addAction(m_alt_exp_a_sqr_vec_Act);
  vectormodelMenu->addAction(m_alt_exp_a_sqr_exp_e_vec_Act);
//  vectormodelMenu->addAction(m_alt_exp_exp_a_vec_Act);
//  vectormodelMenu->addAction(m_alt_exp_exp_a_exp_e_vec_Act);
  matrixmodelMenu = modelMenu->addMenu(tr("2-Point Correlator, &Matrix Fit"));
  matrixmodelMenu->addAction(m_exp_mat_Act);
  matrixmodelMenu->addAction(m_exp_exp_e_mat_Act);
  matrixmodelMenu->addSeparator();
  matrixmodelMenu->addAction(m_alt_exp_mat_Act);
  matrixmodelMenu->addAction(m_alt_exp_exp_e_mat_Act);

  matrixuppermodelMenu = modelMenu->addMenu(tr("2-Point Correlator, &Upper Matrix Fit"));
  matrixuppermodelMenu->addAction(m_exp_mat_upper_Act);
  matrixuppermodelMenu->addAction(m_exp_exp_e_mat_upper_Act);

  nonsymmatrixmodelMenu = modelMenu->addMenu(tr("2-Point Correlator, &Non-symmetric Matrix Fit"));
  nonsymmatrixmodelMenu->addAction(m_exp_nonsym_mat_Act);
  nonsymmatrixmodelMenu->addAction(m_exp_exp_e_nonsym_mat_Act);
  nonsymmatrixmodelMenu->addSeparator();
  nonsymmatrixmodelMenu->addAction(m_alt_exp_nonsym_mat_Act);
  nonsymmatrixmodelMenu->addAction(m_alt_exp_exp_e_nonsym_mat_Act);
  matrixIImodelMenu = modelMenu->addMenu(tr("2-Point Correlator, Matrix Fit Type &II"));
  matrixIImodelMenu->addAction(m_exp_mat_II_Act);
  matrixIImodelMenu->addAction(m_exp_exp_e_mat_II_Act);

  matrixIIuppermodelMenu = modelMenu->addMenu(tr("2-Point Correlator, Upper Matrix Fit Type II"));
  matrixIIuppermodelMenu->addAction(m_exp_mat_II_upper_Act);
  matrixIIuppermodelMenu->addAction(m_exp_exp_e_mat_II_upper_Act);

  modelMenu->addSeparator();
  scalar3pt2varmodelMenu = modelMenu->addMenu(tr("&3-Point Correlator"));
  scalar3pt2varmodelMenu->addAction(threept_2var_m_exp_Act);
  scalar3pt2varmodelMenu->addAction(threept_2var_m_exp_exp_e_Act);
  scalar3pt2varmodelMenu->addSeparator();
  scalar3pt2varmodelMenu->addAction(threept_2var_m_alt_exp_Act);
  scalar3pt2varmodelMenu->addAction(threept_2var_m_alt_exp_exp_e_Act);
  vector3pt2varmodelMenu = modelMenu->addMenu(tr("3-Point Correlator, Vector Fit"));
  vector3pt2varmodelMenu->addAction(threept_2var_m_exp_vec_Act);
  vector3pt2varmodelMenu->addAction(threept_2var_m_exp_exp_e_vec_Act);
  vector3pt2varmodelMenu->addSeparator();
  vector3pt2varmodelMenu->addAction(threept_2var_m_alt_exp_vec_Act);
  vector3pt2varmodelMenu->addAction(threept_2var_m_alt_exp_exp_e_vec_Act);

  plotMenu = menuBar()->addMenu(tr("&Plot"));
  plotMenu->addAction(plot_data_Act);
  plotMenu->addAction(plot_eff_mass_Act);
  plotMenu->addAction(plot_start_Act);
  plotMenu->addAction(plot_all_Act);
  plotMenu->addAction(plot_set_Act);

  toolsMenu = menuBar()->addMenu(tr("&Tools"));
  toolsMenu->addAction(start_to_prior_Act);
  toolsMenu->addAction(result_to_prior_Act);
  toolsMenu->addAction(result_to_start_Act);
  toolsMenu->addSeparator();
  toolsMenu->addAction(chisqr_start_Act);
  toolsMenu->addSeparator();
  toolsMenu->addAction(reload_data_file_Act);
  toolsMenu->addSeparator();
  toolsMenu->addAction(report_Act);
  toolsMenu->addSeparator();
  toolsMenu->addAction(write_results_Act);
  toolsMenu->addSeparator();
  toolsMenu->addAction(multifit_Act);

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);

  setIconSize(QSize(22,22));
  
  fileToolBar = addToolBar(tr("File"));
  fileToolBar->addAction(newAct);
  fileToolBar->addAction(openAct);
  fileToolBar->addAction(saveAct);
  fileToolBar->addAction(saveAsAct);

  plotToolBar = addToolBar(tr("Plot"));
  plotToolBar->addAction(plot_data_Act);
  plotToolBar->addAction(plot_eff_mass_Act);
  plotToolBar->addAction(plot_start_Act);
  plotToolBar->addAction(plot_all_Act);

  toolsToolBar = addToolBar(tr("Tools"));
  toolsToolBar->addAction(start_to_prior_Act);
  toolsToolBar->addAction(result_to_prior_Act);
  toolsToolBar->addAction(result_to_start_Act);
  toolsToolBar->addSeparator();
  toolsToolBar->addAction(chisqr_start_Act);
  toolsToolBar->addSeparator();
  toolsToolBar->addAction(reload_data_file_Act);
  toolsToolBar->addSeparator();
  toolsToolBar->addAction(report_Act);
  toolsToolBar->addSeparator();
  toolsToolBar->addAction(write_results_Act);
}






// *******************************************************************
// this function creates widgets and dialogs
// *******************************************************************
void MainWindow::create_Widgets_Dialogs()
{
  mainWidget=new MainWidget(start_bayes, start_n_parameters, max_n_parameters);
  connect(mainWidget, SIGNAL(modified()), this, SLOT(modified()));
  connect(mainWidget, SIGNAL(data_file_modified()), this, SLOT(data_file_modified()));
  connect(mainWidget, SIGNAL(start_fit()), this, SLOT(fit()));
  connect(mainWidget, SIGNAL(start_BS()), this, SLOT(bootstrap()));

  connect(this, SIGNAL(lf(QString)), this, SLOT(loadFile(const QString&)));

  tabWidget = new QTabWidget();
  tabWidget -> addTab(mainWidget, tr("Main"));

  funcWidget=new FunctionWidget(start_n_functions, max_n_functions);
  connect(funcWidget, SIGNAL(modified()), this, SLOT(modified()));
  connect(funcWidget, SIGNAL(n_functions_modified(int)), this, SLOT(n_functions_modified(int)));
  connect(funcWidget, SIGNAL(function_changed(int, std::string)), this, SLOT(function_changed()));
  tabWidget -> addTab(funcWidget, tr("Model Functions"));

  varsWidget=new VariablesWidget(start_n_variables, max_n_variables);
  connect(varsWidget, SIGNAL(modified()), this, SLOT(modified()));
  connect(varsWidget, SIGNAL(n_variables_modified(int)), this, SLOT(n_variables_modified()));
  connect(varsWidget, SIGNAL(variable_changed(int, std::string)), this, SLOT(variable_changed()));
  connect(varsWidget, SIGNAL(range_changed()), this, SLOT(fit_ranges_changed()));
  tabWidget -> addTab(varsWidget, tr("Variables / Fitting Ranges"));

  parderWidget=new ParamDerivsWidget(start_num_diff, start_n_functions, max_n_functions, start_n_parameters, max_n_parameters);
  connect(parderWidget, SIGNAL(modified()), this, SLOT(modified()));
  connect(parderWidget, SIGNAL(n_parameters_modified(int)), this, SLOT(n_parameters_modified(int)));
  connect(parderWidget, SIGNAL(n_functions_modified(int)), this, SLOT(n_functions_modified(int)));
  connect(parderWidget, SIGNAL(parameter_changed(int, std::string)), this, SLOT(parameter_changed(int, std::string)));
  connect(parderWidget, SIGNAL(derivative_changed(int, int, std::string)), this, SLOT(derivative_changed()));
  tabWidget -> addTab(parderWidget, tr("Parameters / Derivatives"));

  constsWidget=new ConstantsWidget(start_n_constants, max_n_constants);
  connect(constsWidget, SIGNAL(modified()), this, SLOT(modified()));
  connect(constsWidget, SIGNAL(n_constants_modified(int)), this, SLOT(n_constants_modified()));
  connect(constsWidget, SIGNAL(constant_changed(int, std::string)), this, SLOT(constant_changed()));
  tabWidget -> addTab(constsWidget, tr("Constants"));

  chisqrextraWidget=new ChiSqrExtraTermWidget(start_n_constants, max_n_constants);
  connect(chisqrextraWidget, SIGNAL(modified()), this, SLOT(modified()));
  connect(chisqrextraWidget, SIGNAL(function_modified()), this, SLOT(function_changed()));
  connect(chisqrextraWidget, SIGNAL(enabled_modified()), this, SLOT(chisqrextra_enabled_modified()));
  connect(chisqrextraWidget, SIGNAL(n_constants_modified(int)), this, SLOT(n_constants_modified()));
  connect(chisqrextraWidget, SIGNAL(constant_changed(int, std::string)), this, SLOT(constant_changed()));
  tabWidget -> addTab(chisqrextraWidget, tr("Additional term in chi^2"));

  fitsetWidget=new FitSettingsWidget(start_bayes, start_num_diff, start_second_deriv_minimization, start_second_deriv_covariance, start_num_diff_step, start_use_bse, start_restrict_data, start_start_data, start_stop_data, start_startlambda, start_lambdafac,
                                     start_tolerance, start_svd, start_svd_ratio, start_svd_value, start_rescale_value, start_steps, start_bin, start_bssamples);
  connect(fitsetWidget, SIGNAL(modified()), this, SLOT(modified()));
  connect(fitsetWidget, SIGNAL(bayesianmodified(int)), this, SLOT(bayesian_modified(int)));
  connect(fitsetWidget, SIGNAL(bin_modified(int)), this, SLOT(bin_modified()));
  connect(fitsetWidget, SIGNAL(svd_modified()), this, SLOT(svd_modified()));
  connect(fitsetWidget, SIGNAL(cov_normalization_modified()), this, SLOT(cov_normalization_modified()));
  connect(fitsetWidget, SIGNAL(datarange_modified()), this, SLOT(bin_modified()));
  connect(fitsetWidget, SIGNAL(numdiffmodified(int)), this, SLOT(num_diff_modified(int)));
  connect(fitsetWidget, SIGNAL(secondderivminimizationmodified(int)), this, SLOT(second_deriv_minimization_modified(int)));
  connect(fitsetWidget, SIGNAL(secondderivcovariancemodified(int)), this, SLOT(second_deriv_covariance_modified(int)));
  tabWidget -> addTab(fitsetWidget, tr("Fit Settings"));

  m_exp_dialog=new multi_exp_dialog(start_n_exp, max_n_exp, this);
  connect(m_exp_dialog, SIGNAL(modified()), this, SLOT(modified()));

  m_alt_exp_dialog=new multi_alt_exp_dialog(start_n_exp, max_n_exp, start_m_exp, max_m_exp, this);
  connect(m_alt_exp_dialog, SIGNAL(modified()), this, SLOT(modified()));

  m_exp_vec_dialog=new multi_exp_vec_dialog(start_n_exp, max_n_exp, start_n_vec, max_n_vec, this);
  connect(m_exp_vec_dialog, SIGNAL(modified()), this, SLOT(modified()));

  m_exp_mat_dialog=new multi_exp_mat_dialog(start_n_exp, max_n_exp, start_n_vec, start_n_vec, max_n_vec, this);
  connect(m_exp_mat_dialog, SIGNAL(modified()), this, SLOT(modified()));

  m_exp_mat_upper_dialog=new multi_exp_mat_upper_dialog(start_n_exp, max_n_exp, start_n_vec, max_n_vec, this);
  connect(m_exp_mat_upper_dialog, SIGNAL(modified()), this, SLOT(modified()));

  m_alt_exp_vec_dialog=new multi_alt_exp_vec_dialog(start_n_exp, max_n_exp, start_m_exp, max_m_exp, start_n_vec, max_n_vec, this);
  connect(m_alt_exp_vec_dialog, SIGNAL(modified()), this, SLOT(modified()));

  m_alt_exp_mat_dialog=new multi_alt_exp_mat_dialog(start_n_exp, max_n_exp, start_m_exp, max_m_exp, start_n_vec, start_n_vec, max_n_vec, this);
  connect(m_alt_exp_mat_dialog, SIGNAL(modified()), this, SLOT(modified()));

  threept_m_exp_dialog=new threept_multi_exp_dialog(start_n_exp, max_n_exp, start_n_exp, max_n_exp, this);
  connect(threept_m_exp_dialog, SIGNAL(modified()), this, SLOT(modified()));

  threept_m_exp_vec_dialog=new threept_multi_exp_vec_dialog(start_n_exp, max_n_exp, start_n_exp, max_n_exp, start_n_vec, max_n_vec, this);
  connect(threept_m_exp_vec_dialog, SIGNAL(modified()), this, SLOT(modified()));

  threept_m_alt_exp_dialog=new threept_multi_alt_exp_dialog(start_n_exp, max_n_exp, start_m_exp, max_m_exp, start_n_exp, max_n_exp, start_m_exp, max_m_exp, this);
  connect(threept_m_alt_exp_dialog, SIGNAL(modified()), this, SLOT(modified()));

  threept_m_alt_exp_vec_dialog=new threept_multi_alt_exp_vec_dialog(start_n_exp, max_n_exp, start_m_exp, max_m_exp, start_n_exp, max_n_exp, start_m_exp, max_m_exp, start_n_vec, max_n_vec, this);
  connect(threept_m_alt_exp_vec_dialog, SIGNAL(modified()), this, SLOT(modified()));

  plot_set_dialog=new plot_settings_dialog(start_n_variables, max_n_variables, this);
  connect(plot_set_dialog, SIGNAL(modified()), this, SLOT(modified()));

  fitdialog = new fit_dialog();
  connect(fitdialog, SIGNAL(abort()), this, SLOT(abort_fit()));

  bootstrapdialog = new bootstrap_dialog();
  connect(bootstrapdialog, SIGNAL(abort()), this, SLOT(abort_bootstrap()));

  multifitdialog = new multifit_dialog();
  connect(multifitdialog, SIGNAL(abort()), this, SLOT(abort_multifit()));

  funcWidget->updateSizes();
  mainWidget->updateSizes();
  varsWidget->updateSizes();
  constsWidget->updateSizes();
  plot_set_dialog->updateSizes();
}






void MainWindow::closeEvent(QCloseEvent *event)
{
  if(maybeSave())
  {
    event->accept();
  }
  else
  {
    event->ignore();
  }
}


void MainWindow::newFile()
{
  if(maybeSave())
  {
    reset();
    setCurrentFile("");
  }
}


void MainWindow::open()
{
  if(maybeSave())
  {
    QString fileName=QFileDialog::getOpenFileName(this, tr("Open MBF File"),
                                                  current_dir,
                                                  tr("MBF files (*.mbf);; All files (*)"));
    if(!fileName.isEmpty())
    {
      reset();
      loadFile(fileName);
      current_dir=QFileInfo(fileName).absolutePath();
    }
  }
}



// *******************************************************************
// checks whether the mbf file should be saved
// *******************************************************************
bool MainWindow::maybeSave()
{
  if(something_modified)
  {
    int ret=QMessageBox::warning(this, tr("QMBF"),
                                 tr("Do you want to save your changes?"),
                                 QMessageBox::Yes | QMessageBox::Default,
                                 QMessageBox::No, QMessageBox::Cancel
                                 | QMessageBox::Escape);
    if(ret==QMessageBox::Yes)
    {
      return save();
    }
    else if(ret == QMessageBox::Cancel)
    {
      return false;
    }
  }
  return true;
}


bool MainWindow::save()
{
  if(curFile.isEmpty())
  {
    return saveAs();
  }
  else
  {
    return saveFile(curFile);
  }
}


bool MainWindow::saveAs()
{
  QString fileName=QFileDialog::getSaveFileName(this, tr("Save MBF File"),
                                                curFile,
                                                tr("MBF files (*.mbf);; All files (*)"));
  if(fileName.isEmpty())
  {
    return false;
  }
  current_dir=QFileInfo(fileName).absolutePath();
  return saveFile(fileName);
}


// *******************************************************************
// *******************************************************************
#include "mbf_load_save.cpp"
// *******************************************************************
// *******************************************************************



// *******************************************************************
// this function updates the mbf file name
// *******************************************************************
void MainWindow::setCurrentFile(const QString &fileName)
{
  curFile=fileName;

  setWindowModified(false);
  something_modified=false;

  QString shownName;
  if(curFile.isEmpty())
  {
    shownName="untitled.mbf";
  }
  else
  {
    shownName=strippedName(curFile);
  }
  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("QMBF")));
}


// *******************************************************************
// this function returns the name of the file, excluding the path
// *******************************************************************
QString MainWindow::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}



// *******************************************************************
// this function shows an "About" window
// *******************************************************************
void MainWindow::aboutWindow()
{
  stringstream version_st;
  version_st.precision(2);
  version_st.setf(ios::fixed);
  version_st << version;
  QMessageBox::about(this, tr("About QMBF"),
                           tr("\nQMBF\n"
                              "Version %1 \n\n"
                              "by Stefan Meinel").arg(version_st.str().c_str()));
}





void MainWindow::parse_triggered()
{
  current_model=PARSE;
  need_to_reset_fitter=true;
  check_current_model();
}


void MainWindow::parse_toggled()
{
  parderWidget->set_user_def(parse_Act->isChecked());
  constsWidget->set_user_def(parse_Act->isChecked());
  funcWidget->set_user_def(parse_Act->isChecked());
  varsWidget->set_user_def(parse_Act->isChecked());
  fitsetWidget->set_num_diff(parse_Act->isChecked());
}





void MainWindow::update_model_widgets()
{
  mainWidget->save_tables();

  funcWidget->reset();
  constsWidget->reset();
  parderWidget->reset();
  mainWidget->reset_tables();

  int n_variables=_model->get_n_variables();
  varsWidget->set_n_variables(n_variables);
  for(int v=0; v<n_variables; ++v)
  {
    varsWidget->set_variable_name(v, _model->get_variable_name(v));
  }
  int n_parameters=_model->get_n_parameters();
  parderWidget->set_n_parameters(n_parameters);
  for(int p=0; p<n_parameters; ++p)
  {
    parderWidget->set_parameter(p, _model->get_parameter_name(p));
  }
  int n_functions=_model->get_n_functions();
  funcWidget->set_n_functions(n_functions);
  for(int f=0; f<n_functions; ++f)
  {
    funcWidget->set_function(f, _model->get_function_name(f));
  }
  int n_constants=_model->get_n_constants();
  constsWidget->set_n_constants(n_constants);
  for(int c=0; c<n_constants; ++c)
  {
    constsWidget->set_constant_name(c, _model->get_constant_name(c));
  }

  mainWidget->restore_tables();
}







void MainWindow::show_m_exp()
{
  m_exp_dialog->set_text("Multiple Exponentials");
  if(m_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_dialog->get_BC())
    {
      if(m_exp_dialog->get_constant())
      {
        current_model=MULTIEXPBCCONST;
      }
      else
      {
        current_model=MULTIEXPBC;
      }
    }
    else
    {
      if(m_exp_dialog->get_constant())
      {
        current_model=MULTIEXPCONST;
      }
      else
      {
        current_model=MULTIEXP;
      }
    }
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_a_sqr()
{
  m_exp_dialog->set_text("Multiple Exponentials with Square Amplitudes");
  if(m_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_dialog->get_BC())
    {
      if(m_exp_dialog->get_constant())
      {
        current_model=MULTIEXPASQRBCCONST;
      }
      else
      {
        current_model=MULTIEXPASQRBC;
      }
    }
    else
    {
      if(m_exp_dialog->get_constant())
      {
        current_model=MULTIEXPASQRCONST;
      }
      else
      {
        current_model=MULTIEXPASQR;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}



void MainWindow::show_m_exp_a_sqr_exp_e()
{
  m_exp_dialog->set_text("Multiple Exponentials with Square Amplitudes and Exponential Energies");
  if(m_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_dialog->get_BC())
    {
      if(m_exp_dialog->get_constant())
      {
        current_model=MULTIEXPASQREXPEBCCONST;
      }
      else
      {
        current_model=MULTIEXPASQREXPEBC;
      }
    }
    else
    {
      if(m_exp_dialog->get_constant())
      {
        current_model=MULTIEXPASQREXPECONST;
      }
      else
      {
        current_model=MULTIEXPASQREXPE;
      }
    }
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}



/*
void MainWindow::show_m_exp_exp_a()
{
  m_exp_dialog->set_text("Multiple Exponentials with Exponential Amplitudes");
  if(m_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_dialog->get_BC())
    {
      current_model=MULTIEXPEXPABC;
    }
    else
    {
      current_model=MULTIEXPEXPA;
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}
*/

void MainWindow::show_m_exp_exp_e()
{
  m_exp_dialog->set_text("Multiple Exponentials with Exponential Energies");
  if(m_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_dialog->get_BC())
    {
      if(m_exp_dialog->get_constant())
      {
        current_model=MULTIEXPEXPEBCCONST;
      }
      else
      {
        current_model=MULTIEXPEXPEBC;
      }
    }
    else
    {
      if(m_exp_dialog->get_constant())
      {
        current_model=MULTIEXPEXPECONST;
      }
      else
      {
        current_model=MULTIEXPEXPE;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

/*
void MainWindow::show_m_exp_exp_a_exp_e()
{
  m_exp_dialog->set_text("Multiple Exponentials with Exponential Amplitudes and Energies");
  if(m_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_dialog->get_BC())
    {
      current_model=MULTIEXPEXPAEXPEBC;
    }
    else
    {
      current_model=MULTIEXPEXPAEXPE;
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}
*/


void MainWindow::show_m_alt_exp()
{
  m_alt_exp_dialog->set_text("Multiple (incl. oscillating) Exponentials");
  if(m_alt_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_dialog->get_BC())
    {
      if(m_alt_exp_dialog->get_constant())
      {
        current_model=MULTIALTEXPBCCONST;
      }
      else
      {
        current_model=MULTIALTEXPBC;
      }
    }
    else
    {
      if(m_alt_exp_dialog->get_constant())
      {
        current_model=MULTIALTEXPCONST;
      }
      else
      {
        current_model=MULTIALTEXP;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_alt_exp_a_sqr()
{
  m_alt_exp_dialog->set_text("Multiple (incl. oscillating) Exponentials with Square Amplitudes");
  if(m_alt_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_dialog->get_BC())
    {
      if(m_alt_exp_dialog->get_constant())
      {
        current_model=MULTIALTEXPASQRBCCONST;
      }
      else
      {
        current_model=MULTIALTEXPASQRBC;
      }
    }
    else
    {
      if(m_alt_exp_dialog->get_constant())
      {
        current_model=MULTIALTEXPASQRCONST;
      }
      else
      {
        current_model=MULTIALTEXPASQR;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

void MainWindow::show_m_alt_exp_a_sqr_exp_e()
{
  m_alt_exp_dialog->set_text("Multiple (incl. oscillating) Exponentials with Square Amplitudes and Exponential Energies");
  if(m_alt_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_dialog->get_BC())
    {
      if(m_alt_exp_dialog->get_constant())
      {
        current_model=MULTIALTEXPASQREXPEBCCONST;
      }
      else
      {
        current_model=MULTIALTEXPASQREXPEBC;
      }
    }
    else
    {
      if(m_alt_exp_dialog->get_constant())
      {
        current_model=MULTIALTEXPASQREXPECONST;
      }
      else
      {
        current_model=MULTIALTEXPASQREXPE;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


/*
void MainWindow::show_m_alt_exp_exp_a()
{
  m_alt_exp_dialog->set_text("Multiple (incl. oscillating) Exponentials with Exponential Amplitudes");
  if(m_alt_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_dialog->get_BC())
    {
      current_model=MULTIALTEXPEXPABC;
    }
    else
    {
      current_model=MULTIALTEXPEXPA;
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}
*/


void MainWindow::show_m_alt_exp_exp_e()
{
  m_alt_exp_dialog->set_text("Multiple (incl. oscillating) Exponentials with Exponential Energies");
  if(m_alt_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_dialog->get_BC())
    {
      if(m_alt_exp_dialog->get_constant())
      {
        current_model=MULTIALTEXPEXPEBCCONST;
      }
      else
      {
        current_model=MULTIALTEXPEXPEBC;
      }
    }
    else
    {
      if(m_alt_exp_dialog->get_constant())
      {
        current_model=MULTIALTEXPEXPECONST;
      }
      else
      {
        current_model=MULTIALTEXPEXPE;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

/*
void MainWindow::show_m_alt_exp_exp_a_exp_e()
{
  m_alt_exp_dialog->set_text("Multiple (incl. oscillating) Exponentials with Exponential Amplitudes and Energies");
  if(m_alt_exp_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_dialog->get_BC())
    {
      current_model=MULTIALTEXPEXPAEXPEBC;
    }
    else
    {
      current_model=MULTIALTEXPEXPAEXPE;
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}
*/

void MainWindow::show_m_exp_vec()
{
  m_exp_vec_dialog->set_text("Multiple Exponentials, Vector Fit");
  m_exp_vec_dialog->set_BC_enabled(true);
  if(m_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_vec_dialog->get_BC())
    {
      if(m_exp_vec_dialog->get_constant())
      {
        current_model=MULTIEXPVECBCCONST;
      }
      else
      {
        current_model=MULTIEXPVECBC;
      }
    }
    else
    {
      if(m_exp_vec_dialog->get_constant())
      {
        current_model=MULTIEXPVECCONST;
      }
      else
      {
        current_model=MULTIEXPVEC;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_a_sqr_vec()
{
  m_exp_vec_dialog->set_text("Multiple Exponentials with Square Amplitudes, Vector Fit");
  m_exp_vec_dialog->set_BC_enabled(true);
  if(m_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_vec_dialog->get_BC())
    {
      if(m_exp_vec_dialog->get_constant())
      {
        current_model=MULTIEXPASQRVECBCCONST;
      }
      else
      {
        current_model=MULTIEXPASQRVECBC;
      }
    }
    else
    {
      if(m_exp_vec_dialog->get_constant())
      {
        current_model=MULTIEXPASQRVECCONST;
      }
      else
      {
        current_model=MULTIEXPASQRVEC;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_a_sqr_exp_e_vec()
{
  m_exp_vec_dialog->set_text("Multiple Exponentials with Square Amplitudes and Exponential Energies, Vector Fit");
  m_exp_vec_dialog->set_BC_enabled(true);
  if(m_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_vec_dialog->get_BC())
    {
      if(m_exp_vec_dialog->get_constant())
      {
        current_model=MULTIEXPASQREXPEVECBCCONST;
      }
      else
      {
        current_model=MULTIEXPASQREXPEVECBC;
      }
    }
    else
    {
      if(m_exp_vec_dialog->get_constant())
      {
        current_model=MULTIEXPASQREXPEVECCONST;
      }
      else
      {
        current_model=MULTIEXPASQREXPEVEC;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


/*
void MainWindow::show_m_exp_exp_a_vec()
{
  m_exp_vec_dialog->set_text("Multiple Exponentials with Exponential Amplitudes, Vector Fit");
  m_exp_vec_dialog->set_BC_enabled(true);
  if(m_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_vec_dialog->get_BC())
    {
      current_model=MULTIEXPEXPAVECBC;
    }
    else
    {
      current_model=MULTIEXPEXPAVEC;
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}
*/


void MainWindow::show_m_exp_exp_e_vec()
{
  m_exp_vec_dialog->set_text("Multiple Exponentials with Exponential Energies, Vector Fit");
  m_exp_vec_dialog->set_BC_enabled(true);
  if(m_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_vec_dialog->get_BC())
    {
      if(m_exp_vec_dialog->get_constant())
      {
        current_model=MULTIEXPEXPEVECBCCONST;
      }
      else
      {
        current_model=MULTIEXPEXPEVECBC;
      }
    }
    else
    {
      if(m_exp_vec_dialog->get_constant())
      {
        current_model=MULTIEXPEXPEVECCONST;
      }
      else
      {
        current_model=MULTIEXPEXPEVEC;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

/*
void MainWindow::show_m_exp_exp_a_exp_e_vec()
{
  m_exp_vec_dialog->set_text("Multiple Exponentials with Exponential Amplitudes and Energies, Vector Fit");
  m_exp_vec_dialog->set_BC_enabled(true);
  if(m_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_exp_vec_dialog->get_BC())
    {
      current_model=MULTIEXPEXPAEXPEVECBC;
    }
    else
    {
      current_model=MULTIEXPEXPAEXPEVEC;
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}
*/

void MainWindow::show_m_alt_exp_vec()
{
  m_alt_exp_vec_dialog->set_text("Multiple (incl. oscillating) Exponentials, Vector Fit");
  m_alt_exp_vec_dialog->set_BC_enabled(true);
  if(m_alt_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_vec_dialog->get_BC())
    {
      if(m_alt_exp_vec_dialog->get_constant())
      {
        current_model=MULTIALTEXPVECBCCONST;
      }
      else
      {
        current_model=MULTIALTEXPVECBC;
      }
    }
    else
    {
      if(m_alt_exp_vec_dialog->get_constant())
      {
        current_model=MULTIALTEXPVECCONST;
      }
      else
      {
        current_model=MULTIALTEXPVEC;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_alt_exp_a_sqr_vec()
{
  m_alt_exp_vec_dialog->set_text("Multiple (incl. oscillating) Exponentials with Square Amplitudes, Vector Fit");
  m_alt_exp_vec_dialog->set_BC_enabled(true);
  if(m_alt_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_vec_dialog->get_BC())
    {
      if(m_alt_exp_vec_dialog->get_constant())
      {
        current_model=MULTIALTEXPASQRVECBCCONST;
      }
      else
      {
        current_model=MULTIALTEXPASQRVECBC;
      }
    }
    else
    {
      if(m_alt_exp_vec_dialog->get_constant())
      {
        current_model=MULTIALTEXPASQRVECCONST;
      }
      else
      {
        current_model=MULTIALTEXPASQRVEC;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

void MainWindow::show_m_alt_exp_a_sqr_exp_e_vec()
{
  m_alt_exp_vec_dialog->set_text("Multiple (incl. oscillating) Exponentials with Square Amplitudes and Exponential Energies, Vector Fit");
  m_alt_exp_vec_dialog->set_BC_enabled(true);
  if(m_alt_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_vec_dialog->get_BC())
    {
      if(m_alt_exp_vec_dialog->get_constant())
      {
        current_model=MULTIALTEXPASQREXPEVECBCCONST;
      }
      else
      {
        current_model=MULTIALTEXPASQREXPEVECBC;
      }
    }
    else
    {
      if(m_alt_exp_vec_dialog->get_constant())
      {
        current_model=MULTIALTEXPASQREXPEVECCONST;
      }
      else
      {
        current_model=MULTIALTEXPASQREXPEVEC;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

/*
void MainWindow::show_m_alt_exp_exp_a_vec()
{
  m_alt_exp_vec_dialog->set_text("Multiple (incl. oscillating) Exponentials with Exponential Amplitudes, Vector Fit");
  m_alt_exp_vec_dialog->set_BC_enabled(true);
  if(m_alt_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_vec_dialog->get_BC())
    {
      current_model=MULTIALTEXPEXPAVECBC;
    }
    else
    {
      current_model=MULTIALTEXPEXPAVEC;
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}
*/

void MainWindow::show_m_alt_exp_exp_e_vec()
{
  m_alt_exp_vec_dialog->set_text("Multiple (incl. oscillating) Exponentials with Exponential Energies, Vector Fit");
  m_alt_exp_vec_dialog->set_BC_enabled(true);
  if(m_alt_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_vec_dialog->get_BC())
    {
      if(m_alt_exp_vec_dialog->get_constant())
      {
        current_model=MULTIALTEXPEXPEVECBCCONST;
      }
      else
      {
        current_model=MULTIALTEXPEXPEVECBC;
      }
    }
    else
    {
      if(m_alt_exp_vec_dialog->get_constant())
      {
        current_model=MULTIALTEXPEXPEVECCONST;
      }
      else
      {
        current_model=MULTIALTEXPEXPEVEC;
      }
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

/*
void MainWindow::show_m_alt_exp_exp_a_exp_e_vec()
{
  m_alt_exp_vec_dialog->set_text("Multiple (incl. oscillating) Exponentials with Exponential Amplitudes and Energies, Vector Fit");
  m_alt_exp_vec_dialog->set_BC_enabled(true);
  if(m_alt_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    if(m_alt_exp_vec_dialog->get_BC())
    {
      current_model=MULTIALTEXPEXPAEXPEVECBC;
    }
    else
    {
      current_model=MULTIALTEXPEXPAEXPEVEC;
    }

    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}
*/


void MainWindow::show_m_exp_mat()
{
  m_exp_mat_dialog->set_text("Multiple Exponentials, Matrix Fit");
  m_exp_mat_dialog->set_min_dim_1(1);
  if(m_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPMAT;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_mat_II()
{
  m_exp_mat_dialog->set_text("Multiple Exponentials, Matrix Fit Type II");
  m_exp_mat_dialog->set_min_dim_1(1);
  if(m_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPMATII;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_exp_e_mat()
{
  m_exp_mat_dialog->set_text("Multiple Exponentials with Exponential Energies, Matrix Fit");
  m_exp_mat_dialog->set_min_dim_1(1);
  if(m_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPEXPEMAT;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_exp_e_mat_II()
{
  m_exp_mat_dialog->set_text("Multiple Exponentials with Exponential Energies, Matrix Fit Type II");
  m_exp_mat_dialog->set_min_dim_1(1);
  if(m_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPEXPEMATII;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}





void MainWindow::show_m_exp_mat_upper()
{
  m_exp_mat_upper_dialog->set_text("Multiple Exponentials, Upper Matrix Fit");
  if(m_exp_mat_upper_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPMATUPPER;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_mat_II_upper()
{
  m_exp_mat_upper_dialog->set_text("Multiple Exponentials, Upper Matrix Fit Type II");
  if(m_exp_mat_upper_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPMATIIUPPER;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_exp_e_mat_upper()
{
  m_exp_mat_upper_dialog->set_text("Multiple Exponentials with Exponential Energies, Upper Matrix Fit");
  if(m_exp_mat_upper_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPEXPEMATUPPER;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_exp_e_mat_II_upper()
{
  m_exp_mat_upper_dialog->set_text("Multiple Exponentials with Exponential Energies, Upper Matrix Fit Type II");
  if(m_exp_mat_upper_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPEXPEMATIIUPPER;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}









void MainWindow::show_m_alt_exp_mat()
{
  m_alt_exp_mat_dialog->set_text("Multiple (incl. oscillating) Exponentials, Matrix Fit");
  m_alt_exp_mat_dialog->set_min_dim_1(1);
  if(m_alt_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIALTEXPMAT;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_alt_exp_exp_e_mat()
{
  m_alt_exp_mat_dialog->set_text("Multiple (incl. oscillating) Exponentials with Exponential Energies, Matrix Fit");
  m_alt_exp_mat_dialog->set_min_dim_1(1);
  if(m_alt_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIALTEXPEXPEMAT;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}



void MainWindow::show_m_exp_nonsym_mat()
{
  m_exp_mat_dialog->set_text("Multiple Exponentials, Non-symmetric Matrix Fit");
  m_exp_mat_dialog->set_min_dim_1(2);
  if(m_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPNONSYMMAT;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_exp_exp_e_nonsym_mat()
{
  m_exp_mat_dialog->set_text("Multiple Exponentials with Exponential Energies, Non-symmetric Matrix Fit");
  m_exp_mat_dialog->set_min_dim_1(2);
  if(m_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIEXPEXPENONSYMMAT;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_alt_exp_nonsym_mat()
{
  m_alt_exp_mat_dialog->set_text("Multiple (incl. oscillating) Exponentials, Non-symmetric Matrix Fit");
  m_alt_exp_mat_dialog->set_min_dim_1(2);
  if(m_alt_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIALTEXPNONSYMMAT;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_m_alt_exp_exp_e_nonsym_mat()
{
  m_alt_exp_mat_dialog->set_text("Multiple (incl. oscillating) Exponentials with Exponential Energies, Non-symmetric Matrix Fit");
  m_alt_exp_mat_dialog->set_min_dim_1(2);
  if(m_alt_exp_mat_dialog->exec()==QDialog::Accepted)
  {
    current_model=MULTIALTEXPEXPENONSYMMAT;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}



void MainWindow::show_threept_2var_m_exp()
{
  if(threept_m_exp_dialog->exec()==QDialog::Accepted)
  {
    current_model=THREEPT2VARMULTIEXP;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

void MainWindow::show_threept_2var_m_exp_exp_e()
{
  threept_m_exp_dialog->set_text("3-Point Correlator, Multiple Exponentials with Exponential Energies");
  if(threept_m_exp_dialog->exec()==QDialog::Accepted)
  {
    current_model=THREEPT2VARMULTIEXPEXPE;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_threept_2var_m_exp_vec()
{
  if(threept_m_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    current_model=THREEPT2VARMULTIEXPVEC;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

void MainWindow::show_threept_2var_m_exp_exp_e_vec()
{
  threept_m_exp_vec_dialog->set_text("3-Point Correlator, Multiple Exponentials with Exponential Energies, Vector Fit");
  if(threept_m_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    current_model=THREEPT2VARMULTIEXPEXPEVEC;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_threept_2var_m_alt_exp()
{
  if(threept_m_alt_exp_dialog->exec()==QDialog::Accepted)
  {
    current_model=THREEPT2VARMULTIALTEXP;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

void MainWindow::show_threept_2var_m_alt_exp_exp_e()
{
  threept_m_alt_exp_dialog->set_text("3-Point Correlator, Multiple (incl. oscillating) Exponentials with Exponential Energies");
  if(threept_m_alt_exp_dialog->exec()==QDialog::Accepted)
  {
    current_model=THREEPT2VARMULTIALTEXPEXPE;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_threept_2var_m_alt_exp_vec()
{
  if(threept_m_alt_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    current_model=THREEPT2VARMULTIALTEXPVEC;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}

void MainWindow::show_threept_2var_m_alt_exp_exp_e_vec()
{
  threept_m_alt_exp_vec_dialog->set_text("3-Point Correlator, Multiple (incl. oscillating) Exponentials with Exponential Energies, Vector Fit");
  if(threept_m_alt_exp_vec_dialog->exec()==QDialog::Accepted)
  {
    current_model=THREEPT2VARMULTIALTEXPEXPEVEC;
    reset_fitter();
    update_model_widgets();
  }
  check_current_model();
}


void MainWindow::show_plot_settings()
{
  plot_set_dialog->exec();
  plot_set_dialog->updateSizes();
}



//******************************************************************
//******************************************************************
#include "plot.cpp"
//******************************************************************
//******************************************************************



void MainWindow::modified()
{
  something_modified=true;
  setWindowModified(true);
}

void MainWindow::data_file_modified()
{
  need_to_load_data_file=true;
}

void MainWindow::bayesian_modified(int newState)
{
  need_to_fit=true;
  if(newState==2)
  {
    mainWidget->set_bayesian(true);
    parderWidget->set_n_parameters_dof(0);
  }
  if(newState==0)
  {
    mainWidget->set_bayesian(false);
    parderWidget->set_n_parameters_dof(parderWidget->get_n_parameters());
  }
}


void MainWindow::chisqrextra_enabled_modified()
{
  need_to_reset_fitter=true;
  need_to_fit=true;
}


void MainWindow::num_diff_modified(int newState)
{
  need_to_reset_fitter=true;
  need_to_fit=true;
  if(newState==2)
  {
    parderWidget->set_num_diff(true);
  }
  if(newState==0)
  {
    parderWidget->set_num_diff(false);
  }
}

void MainWindow::second_deriv_minimization_modified(int newState)
{
  need_to_fit=true;
}

void MainWindow::second_deriv_covariance_modified(int newState)
{
  need_to_fit=true;
}


void MainWindow::bin_modified()
{
  need_to_set_fit_data=true;
  need_to_fit=true;
}

void MainWindow::svd_modified()
{
  need_to_set_fit_data=true;
  need_to_fit=true;
}

void MainWindow::cov_normalization_modified()
{
  need_to_set_fit_data=true;
  need_to_fit=true;
}

void MainWindow::n_functions_modified(int new_n_functions)
{
  parderWidget->set_n_functions(new_n_functions);
  funcWidget->set_n_functions(new_n_functions);
  need_to_reset_fitter=true;
  need_to_fit=true;
}

void MainWindow::n_parameters_modified(int new_n_parameters)
{
  parderWidget->set_n_parameters(new_n_parameters);
  if(!(fitsetWidget->get_bayes()))
  {
    parderWidget->set_n_parameters_dof(new_n_parameters);
  }
  mainWidget->set_n_parameters(new_n_parameters);
  need_to_reset_fitter=true;
  need_to_fit=true;
}

void MainWindow::n_variables_modified()
{
  need_to_reset_fitter=true;
  need_to_fit=true;
  plot_set_dialog->set_n_variables(varsWidget->get_n_variables());
}

void MainWindow::n_constants_modified()
{
  need_to_reset_fitter=true;
  need_to_fit=true;
}

void MainWindow::parameter_changed(int p, std::string new_name)
{
  mainWidget->set_parameter(p, new_name);
  need_to_reset_fitter=true;
  need_to_fit=true;
}

void MainWindow::derivative_changed()
{
  need_to_reset_fitter=true;
  need_to_fit=true;
}

void MainWindow::function_changed()
{
  need_to_reset_fitter=true;
  need_to_fit=true;
}

void MainWindow::constant_changed()
{
  need_to_reset_fitter=true;
  need_to_fit=true;
}

void MainWindow::variable_changed()
{
  need_to_reset_fitter=true;
  need_to_fit=true;
}

void MainWindow::fit_ranges_changed()
{
  need_to_set_fit_data=true;
  need_to_fit=true;
}






// *******************************************************************
// makes fitter ready for data assignment and fitting
// *******************************************************************
bool MainWindow::prepare_fit()
{
  if(need_to_reset_fitter)
  {
    if(!reset_fitter())
    {
      return false;
    }
  }

  _gaussian_prior->set_enabled(fitsetWidget->get_bayes());

  _fitter->set_num_diff(fitsetWidget->get_num_diff());
  _fitter->set_second_deriv_minimization(fitsetWidget->get_second_deriv_minimization());
  _fitter->set_second_deriv_covariance(fitsetWidget->get_second_deriv_covariance());


  if( (fitsetWidget->get_num_diff()) || (fitsetWidget->get_second_deriv_minimization()) || (fitsetWidget->get_second_deriv_covariance()) )
  {
    double num_diff_step=fitsetWidget->get_num_diff_step();
    if(num_diff_step==empty_double)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: Numerical differentiation step size missing"));
      return false;
    }
    if(num_diff_step==0)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: Numerical differentiation step size set to 0"));
      return false;
    }
    _fitter->set_num_diff_step(num_diff_step);
  }

  double start_lambda=fitsetWidget->get_startlambda();
  if(start_lambda==empty_double)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: start lambda missing"));
    return false;
  }
  if(start_lambda==0)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: Start lambda set to 0"));
    return false;
  }
  _fitter->set_initial_lambda(start_lambda);

  double lambdafac=fitsetWidget->get_lambdafac();
  if(lambdafac==empty_double)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: lambda factor missing"));
    return false;
  }
  if(lambdafac==0)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: Lambda factor set to 0"));
    return false;
  }
  _fitter->set_lambda_factor(lambdafac);

  double tolerance=fitsetWidget->get_tolerance();
  if(tolerance==empty_double)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: delta(chi^2) tolerance missing"));
    return false;
  }
  if(tolerance==0)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: delta(chi^2) tolerance set to 0"));
    return false;
  }
  _fitter->set_tolerance(tolerance);

  _fitter->set_cov_normalization(fitsetWidget->get_cov_normalization());

  inversion_method inv_method=fitsetWidget->get_inversion_method();
  _fitter->set_inversion_method(inv_method);

  if(inv_method==simple_cut)
  {
    int svd_cut=fitsetWidget->get_svd();
    if(svd_cut==empty_int)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: SVD cut missing"));
      return false;
    }
    _fitter->set_svd_cut(fitsetWidget->get_svd());
  }

  if(inv_method==ratio_cut)
  {
    double svd_cut_ratio=fitsetWidget->get_svd_ratio();
    if(svd_cut_ratio==empty_double)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: SVD EV ratio cut missing"));
      return false;
    }
    _fitter->set_svd_cut_ratio(svd_cut_ratio);
  }

  if(inv_method==absolute_cut)
  {
    double absolute_cut=fitsetWidget->get_svd_value();
    if(absolute_cut==empty_double)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: SVD cut missing"));
      return false;
    }
    _fitter->set_svd_cut_absolute(fitsetWidget->get_svd_value());
  }

  if(inv_method==off_diagonal_rescale)
  {
    double rescale_factor=fitsetWidget->get_rescale_value();
    if(rescale_factor==empty_double)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: rescale value missing"));
      return false;
    }
    _fitter->set_off_diagonal_rescale_factor(fitsetWidget->get_rescale_value());
  }

  vector< double > constants(constsWidget->get_n_constants(), 0.0);
  for(int c=0; c<constsWidget->get_n_constants(); ++c)
  {
    double val=constsWidget->get_constant_value(c);
    if(val==empty_double)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: constant value missing"));
      return false;
    }
    constants[c]=val;
  }
  _model->set_constants(constants);


  for(int p=0; p<parderWidget->get_n_parameters(); ++p)
  {
    double val=mainWidget->get_start_value(p);
    if(val==empty_double)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: start value missing"));
      return false;
    }
    _fitter->set_starting_value(p, val);
  }

  if(fitsetWidget->get_bayes())
  {
    for(int p=0; p<parderWidget->get_n_parameters(); ++p)
    {
      double val=mainWidget->get_prior(p);
      if(val==empty_double)
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: prior value missing"));
        return false;
      }
      _gaussian_prior->set_prior(p, val);
    }
    for(int p=0; p<parderWidget->get_n_parameters(); ++p)
    {
      double val=mainWidget->get_sigma(p);
      if(val==empty_double)
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: prior sigma missing"));
        return false;
      }
      if(val<=0.0)
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: prior sigma not positive"));
        return false;
      }
      _gaussian_prior->set_sigma(p, val);
    }
  }

  _chisqr_extra_term->set_enabled(chisqrextraWidget->get_enabled());
  if(chisqrextraWidget->get_enabled())
  {
    double num_diff_step=chisqrextraWidget->get_num_diff_step();
    if(num_diff_step==empty_double)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: additional chi^2 term: numerical differentiation step size missing"));
      return false;
    }
    if(num_diff_step==0)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: additional chi^2 term: numerical differentiation step size set to 0"));
      return false;
    }
    _chisqr_extra_term->set_num_diff_step(num_diff_step);
    vector< double > chisqr_extra_term_constants(chisqrextraWidget->get_n_constants(), 0.0);
    for(int c=0; c<chisqrextraWidget->get_n_constants(); ++c)
    {
      double val=chisqrextraWidget->get_constant_value(c);
      if(val==empty_double)
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: additional chi^2 term: constant value missing"));
        return false;
      }
      chisqr_extra_term_constants[c]=val;
    }
    _chisqr_extra_term->set_constants(chisqr_extra_term_constants);

    vector< double > startvals(parderWidget->get_n_parameters());
    for(int p=0; p<parderWidget->get_n_parameters(); ++p)
    {
      startvals[p]=mainWidget->get_start_value(p);
    }
    _chisqr_extra_term->set_parameters(startvals);
    _chisqr_extra_term->chi_sqr();

    if(_chisqr_extra_term->no_of_errors()!=0)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: Error while evaluating chi^2 extra term function"));
      return false;
    }
  }

  if(need_to_load_data_file)
  {
    if(!load_data_file())
    {
      return false;
    }
  }
  if(need_to_set_fit_data)
  {
    if(!set_fit_data())
    {
      return false;
    }
  }

  if(!(_fitter->test_model()))
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error in fit model"));
    return false;
  }

  return true;
}






// *******************************************************************
// start the simple fit
// *******************************************************************
void MainWindow::fit()
{
  if(!prepare_fit())
  {
    return;
  }

  int steps=fitsetWidget->get_steps();
  if(steps==empty_int)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: Maximum number of iterations missing"));
    return;
  }

  stringstream message;
  message << "Fit started" << endl;
  if(fitsetWidget->get_num_diff())
  {
    message << "Using numerical differentiation. Step size = " << fitsetWidget->get_num_diff_step() << endl;
  }
  mainWidget->print(message.str());

  _fitter -> stop = false;

  fitthread -> set_max_steps(steps);

   fitdialog -> set_dof(_fitter->get_dof()-_fitter->get_cut()-parderWidget->get_n_parameters_dof());
  
  fitdialog -> clear();

  fitthread -> start();

  fitdialog -> exec();
}





// *******************************************************************
// stops the simple fit thread manually
// *******************************************************************
void MainWindow::abort_fit()
{
  _fitter -> stop = true;
}




// *******************************************************************
// this function is called when the simple fit thread is finished
// *******************************************************************
void MainWindow::fit_finished(int boot)
{
  int steps=fitsetWidget->get_steps();
  int steps_needed;
  string fit_message="";
  if(boot == 1)
  {
    steps_needed=bootstrapthread -> get_steps_needed();
  }
  else if(boot == 2)
  {
    steps_needed=multifitthread -> get_steps_needed();
  }
  else
  {
    fitdialog -> close();
    steps_needed=fitthread -> get_steps_needed();
    fit_message=fitthread->get_message();
  }
  if(fit_message!="")
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr(fit_message.c_str()));
  }
  else if( (_fitter -> stop) == true)
  {
    mainWidget->clear_results();
    stringstream message;
    message << "Fit aborted"<< endl;
    mainWidget->print(message.str());
  }
  else if(steps_needed==steps+1)
  {
    mainWidget->clear_results();
    stringstream message;
    message << "Fit did not converge after " << steps << " iterations"<< endl;
    mainWidget->print(message.str());
  }
  else
  {
    mainWidget->clear_results();
    double chisqr=_fitter->get_chi_sqr();
    last_chi_sqr=chisqr;
    double dof=_fitter->get_dof()-_fitter->get_cut()-parderWidget->get_n_parameters_dof();
    if(dof<0.0)
    {
      dof=0.0;
    }

    stringstream message;
    message.precision(15);

    message << "Fit converged after " << steps_needed << " iterations." << endl << "chi^2/dof = " << chisqr/dof << "    Q(dof/2,chi^2/2) = " << gsl_sf_gamma_inc_Q(dof/2.0, chisqr/2.0) << endl;

    for(int p=0; p<parderWidget->get_n_parameters(); ++p)
    {
      mainWidget->set_result(p, _fitter->get_parameter(p));
      message << parderWidget->get_parameter(p) << " = " << _fitter->get_parameter(p) << "    sqrt(cov(" << parderWidget->get_parameter(p) << "," << parderWidget->get_parameter(p) << ")) = " << sqrt(_fitter->get_covariance(p, p)) << endl;
      mainWidget->set_sqrt_cov(p, sqrt(_fitter->get_covariance(p, p)));
    }

    if( !(_fitter->covariance_positive_definite()) )
    {
      message << endl << "WARNING: Hessian of chi^2 is not positive definite" << endl;
    }

    mainWidget->print(message.str());
    need_to_fit=false;
  }
}











void MainWindow::bootstrap()
{
  if(!prepare_fit())
  {
    return;
  }

  int maxsteps=fitsetWidget->get_steps();
  if(maxsteps==empty_int)
  {
    QMessageBox::warning(this, tr("QMBF"),
                                     tr("Error: Maximum number of iterations missing"));
    return;
  }
  bootstrapthread -> set_max_steps(maxsteps);

  string dir=mainWidget->get_output_dir();
  if(dir.empty())
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("No output directory selected"));
    return;
  }
  bootstrapthread -> set_output_dir(dir);

  bootstrapthread -> set_use_bse(fitsetWidget->get_use_bse());

  int bssamples=0;
  if(fitsetWidget->get_use_bse())
  {
    string bsefile=fitsetWidget->get_boot_file();
    if(bsefile.empty())
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: no bootstrap ensemble file selected"));
      return;
    }
    bootstrapthread -> set_bse_file(bsefile);
    ifstream bse(bsefile.c_str());
    if(!bse)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: could not open bootstrap ensemble file"));
      bse.close();
      return;
    }
    bse >> bssamples;
    bootstrapthread -> set_bssamples(bssamples);
    int bsdata=0;
    bse >> bsdata;
    if(bsdata!=fit_data.size())
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Bootstrap ensemble file has wrong number of data sets"));
      bse.close();
      return;
    }
    bse.close();
  }
  else
  {
    bssamples=fitsetWidget->get_bssamples();
  }
  bootstrapdialog -> set_bssamples(bssamples);
  bootstrapthread -> set_bssamples(bssamples);

  bootstrapthread -> set_cov_normalization(fitsetWidget -> get_cov_normalization());

  stringstream message;
  message << "Bootstrap started" << endl;
  mainWidget->print(message.str());

  _fitter -> stop = false;
  bootstrapthread -> stop = false;

  bootstrapthread -> set_n_functions(funcWidget->get_n_functions());

  bootstrapthread -> set_n_parameters(parderWidget->get_n_parameters());

  bootstrapthread -> set_n_parameters_dof(parderWidget->get_n_parameters_dof());

  vector< string > par_names;
  for(int p=0; p<parderWidget -> get_n_parameters(); ++p)
  {
    par_names.push_back(parderWidget -> get_parameter(p));
  }
  bootstrapthread -> set_parameter_names(par_names);

  bootstrapthread -> set_boot_prior( (fitsetWidget -> get_bayes()) && (fitsetWidget -> get_boot_prior()) );

  if(fitsetWidget->get_bayes())
  {
    vector< double > priors;
    vector< double > sigmas;
    for(int p=0; p<parderWidget->get_n_parameters(); ++p)
    {
      priors.push_back(mainWidget->get_prior(p));
      sigmas.push_back(mainWidget->get_sigma(p));
    }
    bootstrapthread -> set_priors(priors);
    bootstrapthread -> set_sigmas(sigmas);
  }

  bootstrapthread -> set_fit_arguments(fit_arguments);

  bootstrapthread -> set_fit_data(fit_data);

  bootstrapthread -> set_file_name(curFile);

  bootstrapthread -> start();

  bootstrapdialog -> update_bar(0);
  bootstrapdialog -> exec();
}




// *******************************************************************
// stops the bootstrap thread manually
// *******************************************************************
void MainWindow::abort_bootstrap()
{
  _fitter -> stop = true;
  bootstrapthread -> stop = true;
}





void MainWindow::bootstrap_finished()
{
  need_to_set_fit_data=true;

  bootstrapdialog -> close();

  if( (bootstrapthread -> get_exit_code())==aborted )
  {
    stringstream message;
    message << "Bootstrap aborted" << endl;
    mainWidget->print(message.str());
    return;
  }

  if( (bootstrapthread -> get_exit_code())==fit_not_converge )
  {
    return;
  }

  if( (bootstrapthread -> get_exit_code())==output_error )
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Cannot write bootstrap output files"));
    return;
  }

  if(fitsetWidget->get_cov_normalization()==bootstrap_normalization)
  {
    stringstream message;
    message << "Bootstrap warning: data correlation matrix normalization set to 1/(N-1). Bootstrap widths will not represent actual error and need to be rescaled. Maybe you should use multifit instad of bootstrap." << endl;
    mainWidget->print(message.str());
  }
  if(fitsetWidget->get_cov_normalization()==jackknife_normalization)
  {
    stringstream message;
    message << "Bootstrap warning: data correlation matrix normalization set to (N-1)/N. Bootstrap widths will not represent actual error and need to be rescaled. Maybe you should use multifit instad of bootstrap." << endl;
    mainWidget->print(message.str());
  }


  for(int p=0; p<parderWidget -> get_n_parameters(); ++p)
  {
    mainWidget->set_BS_sigma(p, bootstrapthread -> get_bs_sigma(p));
  }
}






void MainWindow::multifit()
{
  if(!prepare_fit())
  {
    return;
  }

  int maxsteps=fitsetWidget->get_steps();
  if(maxsteps==empty_int)
  {
    QMessageBox::warning(this, tr("QMBF"),
                                     tr("Error: Maximum number of iterations missing"));
    return;
  }
  multifitthread -> set_max_steps(maxsteps);

  string dir=mainWidget->get_output_dir();
  if(dir.empty())
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("No output directory selected"));
    return;
  }
  multifitthread -> set_output_dir(dir);

  multifitdialog -> set_n_data_sets(fit_data.size());

  stringstream message;
  message << "Multifit started" << endl;
  mainWidget->print(message.str());

  _fitter -> stop = false;
  multifitthread -> stop = false;

  multifitthread -> set_n_functions(funcWidget->get_n_functions());

  multifitthread -> set_n_parameters(parderWidget->get_n_parameters());

  multifitthread -> set_n_parameters_dof(parderWidget->get_n_parameters_dof());

  vector< string > par_names;
  for(int p=0; p<parderWidget -> get_n_parameters(); ++p)
  {
    par_names.push_back(parderWidget -> get_parameter(p));
  }
  multifitthread -> set_parameter_names(par_names);

  multifitthread -> set_boot_prior( (fitsetWidget -> get_bayes()) && (fitsetWidget -> get_boot_prior()) );

  if(fitsetWidget->get_bayes())
  {
    vector< double > priors;
    vector< double > sigmas;
    for(int p=0; p<parderWidget->get_n_parameters(); ++p)
    {
      priors.push_back(mainWidget->get_prior(p));
      sigmas.push_back(mainWidget->get_sigma(p));
    }
    multifitthread -> set_priors(priors);
    multifitthread -> set_sigmas(sigmas);
  }

  multifitthread -> set_fit_arguments(fit_arguments);

  multifitthread -> set_fit_data(fit_data);

  multifitthread -> set_file_name(curFile);

  multifitthread -> start();

  multifitdialog -> update_bar(0);
  multifitdialog -> exec();
}




// *******************************************************************
// stops the multifit thread manually
// *******************************************************************
void MainWindow::abort_multifit()
{
  _fitter -> stop = true;
  multifitthread -> stop = true;
}





void MainWindow::multifit_finished()
{
  need_to_set_fit_data=true;

  multifitdialog -> close();

  if( (multifitthread -> get_exit_code())==aborted )
  {
    stringstream message;
    message << "Multifit aborted" << endl;
    mainWidget->print(message.str());
    return;
  }

  if( (multifitthread -> get_exit_code())==fit_not_converge )
  {
    return;
  }

  if( (multifitthread -> get_exit_code())==output_error )
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Cannot write multifit output files"));
    return;
  }


  for(int p=0; p<parderWidget -> get_n_parameters(); ++p)
  {
    mainWidget->set_BS_sigma(p, multifitthread -> get_multifit_sigma(p));
  }
}






// *******************************************************************
// overall reset
// *******************************************************************
void MainWindow::reset()
{
  setCurrentFile("");

  funcWidget->reset();
  constsWidget->reset();
  chisqrextraWidget->reset();
  fitsetWidget->reset();
  varsWidget->reset();
  parderWidget->reset();
  mainWidget->reset();

  m_exp_dialog->reset();
  m_alt_exp_dialog->reset();
  m_exp_vec_dialog->reset();
  m_exp_mat_dialog->reset();
  m_exp_mat_upper_dialog->reset();
  m_alt_exp_vec_dialog->reset();
  m_alt_exp_mat_dialog->reset();
  threept_m_exp_dialog->reset();
  threept_m_exp_vec_dialog->reset();
  threept_m_alt_exp_dialog->reset();
  threept_m_alt_exp_vec_dialog->reset();

  plot_set_dialog->reset();

  need_to_load_data_file=true;
  need_to_set_fit_data=true;
  need_to_reset_fitter=true;
  need_to_fit=true;

  last_chi_sqr=empty_double;

  current_model=PARSE;
  check_current_model();

  something_modified=false;
  setWindowModified(false);
}



// *******************************************************************
// refresh "checks" in "Model" menu to current model
// *******************************************************************
void MainWindow::check_current_model()
{
  parse_Act->setChecked(false);

  m_exp_Act->setChecked(false);
  m_exp_a_sqr_Act->setChecked(false);
  m_exp_a_sqr_exp_e_Act->setChecked(false);
//  m_exp_exp_a_Act->setChecked(false);
  m_exp_exp_e_Act->setChecked(false);
//  m_exp_exp_a_exp_e_Act->setChecked(false);

  m_alt_exp_Act->setChecked(false);
  m_alt_exp_a_sqr_Act->setChecked(false);
  m_alt_exp_a_sqr_exp_e_Act->setChecked(false);
//  m_alt_exp_exp_a_Act->setChecked(false);
  m_alt_exp_exp_e_Act->setChecked(false);
//  m_alt_exp_exp_a_exp_e_Act->setChecked(false);

  m_exp_vec_Act->setChecked(false);
  m_exp_a_sqr_vec_Act->setChecked(false);
  m_exp_a_sqr_exp_e_vec_Act->setChecked(false);
//  m_exp_exp_a_vec_Act->setChecked(false);
  m_exp_exp_e_vec_Act->setChecked(false);
//  m_exp_exp_a_exp_e_vec_Act->setChecked(false);

  m_alt_exp_vec_Act->setChecked(false);
  m_alt_exp_a_sqr_vec_Act->setChecked(false);
  m_alt_exp_a_sqr_exp_e_vec_Act->setChecked(false);
//  m_alt_exp_exp_a_vec_Act->setChecked(false);
  m_alt_exp_exp_e_vec_Act->setChecked(false);
//  m_alt_exp_exp_a_exp_e_vec_Act->setChecked(false);

  m_exp_mat_Act->setChecked(false);
  m_exp_mat_II_Act->setChecked(false);
  m_exp_exp_e_mat_Act->setChecked(false);
  m_exp_exp_e_mat_II_Act->setChecked(false);

  m_exp_mat_upper_Act->setChecked(false);
  m_exp_mat_II_upper_Act->setChecked(false);
  m_exp_exp_e_mat_upper_Act->setChecked(false);
  m_exp_exp_e_mat_II_upper_Act->setChecked(false);

  m_alt_exp_mat_Act->setChecked(false);
  m_alt_exp_exp_e_mat_Act->setChecked(false);

  m_exp_nonsym_mat_Act->setChecked(false);
  m_exp_exp_e_nonsym_mat_Act->setChecked(false);
  m_alt_exp_nonsym_mat_Act->setChecked(false);
  m_alt_exp_exp_e_nonsym_mat_Act->setChecked(false);

  threept_2var_m_exp_Act->setChecked(false);
  threept_2var_m_exp_exp_e_Act->setChecked(false);
  threept_2var_m_exp_vec_Act->setChecked(false);
  threept_2var_m_exp_exp_e_vec_Act->setChecked(false);
  threept_2var_m_alt_exp_Act->setChecked(false);
  threept_2var_m_alt_exp_exp_e_Act->setChecked(false);
  threept_2var_m_alt_exp_vec_Act->setChecked(false);
  threept_2var_m_alt_exp_exp_e_vec_Act->setChecked(false);

  switch(current_model)
  {
    case PARSE:
      parse_Act->setChecked(true);
      break;

    case MULTIEXP: case MULTIEXPBC: case MULTIEXPCONST: case MULTIEXPBCCONST:
      m_exp_Act->setChecked(true);
      break;
    case MULTIEXPASQR: case MULTIEXPASQRBC: case MULTIEXPASQRCONST: case MULTIEXPASQRBCCONST:
      m_exp_a_sqr_Act->setChecked(true);
      break;
    case MULTIEXPASQREXPE: case MULTIEXPASQREXPEBC: case MULTIEXPASQREXPECONST: case MULTIEXPASQREXPEBCCONST:
      m_exp_a_sqr_exp_e_Act->setChecked(true);
      break;
//    case MULTIEXPEXPA: case MULTIEXPEXPABC:
//      m_exp_exp_a_Act->setChecked(true);
//      break;
    case MULTIEXPEXPE: case MULTIEXPEXPEBC: case MULTIEXPEXPECONST: case MULTIEXPEXPEBCCONST:
      m_exp_exp_e_Act->setChecked(true);
      break;
//    case MULTIEXPEXPAEXPE: case MULTIEXPEXPAEXPEBC:
//      m_exp_exp_a_exp_e_Act->setChecked(true);
//      break;

    case MULTIALTEXP: case MULTIALTEXPBC: case MULTIALTEXPCONST: case MULTIALTEXPBCCONST:
      m_alt_exp_Act->setChecked(true);
      break;
    case MULTIALTEXPASQR: case MULTIALTEXPASQRBC: case MULTIALTEXPASQRCONST: case MULTIALTEXPASQRBCCONST:
      m_alt_exp_a_sqr_Act->setChecked(true);
      break;
    case MULTIALTEXPASQREXPE: case MULTIALTEXPASQREXPEBC: case MULTIALTEXPASQREXPECONST: case MULTIALTEXPASQREXPEBCCONST:
      m_alt_exp_a_sqr_exp_e_Act->setChecked(true);
      break;
//    case MULTIALTEXPEXPA: case MULTIALTEXPEXPABC:
//      m_alt_exp_exp_a_Act->setChecked(true);
//      break;
    case MULTIALTEXPEXPE: case MULTIALTEXPEXPEBC: case MULTIALTEXPEXPECONST: case MULTIALTEXPEXPEBCCONST:
      m_alt_exp_exp_e_Act->setChecked(true);
      break;
//    case MULTIALTEXPEXPAEXPE: case MULTIALTEXPEXPAEXPEBC:
//      m_alt_exp_exp_a_exp_e_Act->setChecked(true);
//      break;

    case MULTIEXPVEC: case MULTIEXPVECBC: case MULTIEXPVECCONST: case MULTIEXPVECBCCONST:
      m_exp_vec_Act->setChecked(true);
      break;
    case MULTIEXPASQRVEC: case MULTIEXPASQRVECBC: case MULTIEXPASQRVECCONST: case MULTIEXPASQRVECBCCONST:
      m_exp_a_sqr_vec_Act->setChecked(true);
      break;
    case MULTIEXPASQREXPEVEC: case MULTIEXPASQREXPEVECBC: case MULTIEXPASQREXPEVECCONST: case MULTIEXPASQREXPEVECBCCONST:
      m_exp_a_sqr_exp_e_vec_Act->setChecked(true);
      break;
//    case MULTIEXPEXPAVEC: case MULTIEXPEXPAVECBC:
//      m_exp_exp_a_vec_Act->setChecked(true);
//     break;
    case MULTIEXPEXPEVEC: case MULTIEXPEXPEVECBC: case MULTIEXPEXPEVECCONST: case MULTIEXPEXPEVECBCCONST:
      m_exp_exp_e_vec_Act->setChecked(true);
      break;
//    case MULTIEXPEXPAEXPEVEC: case MULTIEXPEXPAEXPEVECBC:
//      m_exp_exp_a_exp_e_vec_Act->setChecked(true);
//     break;

    case MULTIALTEXPVEC: case MULTIALTEXPVECBC: case MULTIALTEXPVECCONST: case MULTIALTEXPVECBCCONST:
      m_alt_exp_vec_Act->setChecked(true);
      break;
    case MULTIALTEXPASQRVEC: case MULTIALTEXPASQRVECBC: case MULTIALTEXPASQRVECCONST: case MULTIALTEXPASQRVECBCCONST:
      m_alt_exp_a_sqr_vec_Act->setChecked(true);
      break;
    case MULTIALTEXPASQREXPEVEC: case MULTIALTEXPASQREXPEVECBC: case MULTIALTEXPASQREXPEVECCONST: case MULTIALTEXPASQREXPEVECBCCONST:
      m_alt_exp_a_sqr_exp_e_vec_Act->setChecked(true);
      break;
//    case MULTIALTEXPEXPAVEC: case MULTIALTEXPEXPAVECBC:
//      m_alt_exp_exp_a_vec_Act->setChecked(true);
//      break;
    case MULTIALTEXPEXPEVEC: case MULTIALTEXPEXPEVECBC: case MULTIALTEXPEXPEVECCONST: case MULTIALTEXPEXPEVECBCCONST:
      m_alt_exp_exp_e_vec_Act->setChecked(true);
      break;
//    case MULTIALTEXPEXPAEXPEVEC: case MULTIALTEXPEXPAEXPEVECBC:
//      m_alt_exp_exp_a_exp_e_vec_Act->setChecked(true);
//      break;

    case MULTIEXPMAT:
      m_exp_mat_Act->setChecked(true);
      break;
    case MULTIEXPEXPEMAT:
      m_exp_exp_e_mat_Act->setChecked(true);
      break;
    case MULTIEXPMATUPPER:
      m_exp_mat_upper_Act->setChecked(true);
      break;
    case MULTIEXPEXPEMATUPPER:
      m_exp_exp_e_mat_upper_Act->setChecked(true);
      break;
    case MULTIALTEXPMAT:
      m_alt_exp_mat_Act->setChecked(true);
      break;
    case MULTIALTEXPEXPEMAT:
      m_alt_exp_exp_e_mat_Act->setChecked(true);
      break;

    case MULTIEXPNONSYMMAT:
      m_exp_nonsym_mat_Act->setChecked(true);
      break;
    case MULTIEXPEXPENONSYMMAT:
      m_exp_exp_e_nonsym_mat_Act->setChecked(true);
      break;
    case MULTIALTEXPNONSYMMAT:
      m_alt_exp_nonsym_mat_Act->setChecked(true);
      break;
    case MULTIALTEXPEXPENONSYMMAT:
      m_alt_exp_exp_e_nonsym_mat_Act->setChecked(true);
      break;

    case MULTIEXPMATII:
      m_exp_mat_II_Act->setChecked(true);
      break;
    case MULTIEXPEXPEMATII:
      m_exp_exp_e_mat_II_Act->setChecked(true);
      break;
    case MULTIEXPMATIIUPPER:
      m_exp_mat_II_upper_Act->setChecked(true);
      break;
    case MULTIEXPEXPEMATIIUPPER:
      m_exp_exp_e_mat_II_upper_Act->setChecked(true);
      break;

    case THREEPT2VARMULTIEXP:
      threept_2var_m_exp_Act->setChecked(true);
      break;
    case THREEPT2VARMULTIEXPEXPE:
      threept_2var_m_exp_exp_e_Act->setChecked(true);
      break;
    case THREEPT2VARMULTIEXPVEC:
      threept_2var_m_exp_vec_Act->setChecked(true);
      break;
    case THREEPT2VARMULTIEXPEXPEVEC:
      threept_2var_m_exp_exp_e_vec_Act->setChecked(true);
      break;
    case THREEPT2VARMULTIALTEXP:
      threept_2var_m_alt_exp_Act->setChecked(true);
      break;
    case THREEPT2VARMULTIALTEXPEXPE:
      threept_2var_m_alt_exp_exp_e_Act->setChecked(true);
      break;
    case THREEPT2VARMULTIALTEXPVEC:
      threept_2var_m_alt_exp_vec_Act->setChecked(true);
      break;
    case THREEPT2VARMULTIALTEXPEXPEVEC:
      threept_2var_m_alt_exp_exp_e_vec_Act->setChecked(true);
      break;

    default:

      break;
  }
}






// *******************************************************************
// set priors to start values
// *******************************************************************
void MainWindow::start_to_prior()
{
  for(int p=0; p<parderWidget->get_n_parameters(); ++p)
  {
    double val=mainWidget->get_start_value(p);
    if(val!=empty_double)
    {
      mainWidget->set_prior(p, val);
    }
  }
}






// *******************************************************************
// set start values to fit results
// *******************************************************************
void MainWindow::result_to_start()
{
  for(int p=0; p<parderWidget->get_n_parameters(); ++p)
  {
    double val=mainWidget->get_result(p);
    if(val!=empty_double)
    {
      mainWidget->set_start_value(p, val);
    }
  }
}






// *******************************************************************
// set priors to fit results
// *******************************************************************
void MainWindow::result_to_prior()
{
  for(int p=0; p<parderWidget->get_n_parameters(); ++p)
  {
    double val=mainWidget->get_result(p);
    if(val!=empty_double)
    {
      mainWidget->set_prior(p, val);
    }
  }
}





// *******************************************************************
// display of chi^2
// *******************************************************************
void MainWindow::chisqr_start()
{
  if(!prepare_fit())
  {
    return;
  }
  double chisqr=_fitter->get_chi_sqr_start();
  double dof=_fitter->get_dof()-_fitter->get_cut()-parderWidget->get_n_parameters_dof();
  if(dof<0.0)
  {
    dof=0.0;
  }

  stringstream message;
  message.precision(15);

  message << "Current start values have chi^2/dof = " << chisqr/dof << "    Q(dof/2,chi^2/2) = " << gsl_sf_gamma_inc_Q(dof/2.0, chisqr/2.0) << endl;
  mainWidget->print(message.str());
}




// *******************************************************************
// this function shows a report with all settings and results
// *******************************************************************
void MainWindow::show_report()
{
  ReportView* rv=new ReportView(current_dir);
  stringstream sst;

  sst.setf(ios::left,ios::adjustfield);

  int n_functions=funcWidget->get_n_functions();
  int n_variables=varsWidget->get_n_variables();
  int n_parameters=parderWidget->get_n_parameters();
  int n_constants=constsWidget->get_n_constants();

  stringstream version_st;
  version_st.precision(2);
  version_st.setf(ios::fixed);
  version_st << version;
  sst << "QMBF version = " << version_st.str() << endl << endl;

  sst << "Data file = " << mainWidget->get_data_file() << endl << endl;

  if(fitsetWidget->get_restrict_data())
  {
    sst << "Data range min = " << fitsetWidget->get_data_start() << endl;
    sst << "Data range max = " << fitsetWidget->get_data_stop() << endl << endl;
  }

  if(chisqrextraWidget->get_enabled())
  {
    sst << "enabled chi^2 additional term = " << chisqrextraWidget->get_function() << endl;
    for(int c=0; c<chisqrextraWidget->get_n_constants(); ++c)
    {
      double constant=chisqrextraWidget->get_constant_value(c);
      sst << "   constant  " << chisqrextraWidget->get_constant_name(c)
          << " = ";
      if(constant!=empty_double)
      {
        sst  << constant;
      }
      sst << endl;
    }
    sst << endl;
  }

  for(int f=0; f<n_functions; ++f)
  {
    sst << "Function " << f+1 << ":" << endl << funcWidget->get_function(f) << endl << endl;
  }

  for(int v=0; v<n_variables; ++v)
  {
    sst << "Variable  " << varsWidget->get_variable_name(v)
        << "  Fitting min = " << varsWidget->get_fit_min(v)
        << "  Fitting max = " << varsWidget->get_fit_max(v) << endl << endl;
  }

  for(int c=0; c<n_constants; ++c)
  {
    double constant=constsWidget->get_constant_value(c);
    sst << "Constant  " << constsWidget->get_constant_name(c)
        << " = ";
    if(constant!=empty_double)
    {
      sst  << constant;
    }
    sst << endl << endl;
  }

  sst << "Parameter   Start value     Prior value     Prior sigma     Fit result      sqrt(cov)       BS 68 % error" << endl;
  sst << "---------------------------------------------------------------------------------------------------------" << endl;
  for(int p=0; p<n_parameters; ++p)
  {
    double start_val=mainWidget->get_start_value(p);
    double prior=mainWidget->get_prior(p);
    double sigma=mainWidget->get_sigma(p);

    sst << setw(12) << parderWidget->get_parameter(p);

    if(start_val!=empty_double)
    {
      sst << setw(16) << start_val;
    }
    else
    {
      sst << "                ";
    }
    if(fitsetWidget->get_bayes())
    {
      if( (prior!=empty_double) && (sigma!=empty_double) )
      {
        sst << setw(16) << mainWidget->get_prior(p)
            << setw(16) << mainWidget->get_sigma(p);
      }
      else
      {
        sst << "                " << "                ";
      }
    }
    else
    {
      sst << "                " << "                ";
    }
    double result=mainWidget->get_result(p);
    if(result!=empty_double)
    {
      sst << setw(16) << result;
    }
    else
    {
      sst << "                ";
    }
    double sqrtcov=mainWidget->get_sqrt_cov(p);
    if(sqrtcov!=empty_double)
    {
      sst << setw(16) << sqrtcov;
    }
    else
    {
      sst << "                ";
    }
    double BSsigma=mainWidget->get_BS_sigma(p);
    if(BSsigma!=empty_double)
    {
      sst << setw(16) << BSsigma;
    }
    else
    {
      sst << "                ";
    }
    sst << endl;
  }

  sst << endl;

  if(last_chi_sqr!=empty_double)
  {
    double chisqr=last_chi_sqr;
    double dof=_fitter->get_dof()-_fitter->get_cut()-parderWidget->get_n_parameters_dof();
    if(dof<0.0)
    {
      dof=0.0;
    }
    sst << "chi^2/dof        = " << chisqr/dof << endl;
    sst << "Q(dof/2,chi^2/2) = " << gsl_sf_gamma_inc_Q(dof/2.0, chisqr/2.0) << endl;
    sst << "Effective dof    = " << dof << endl << endl;
  }

  sst << "Fit algorithm settings:" << endl << endl;
  if(fitsetWidget->get_num_diff())
  {
    sst << "Using numerical differentiation for first derivatives." << endl;
  }
  else
  {
    sst << "Using symbolic differentiation for first derivatives." << endl;
  }

  if(fitsetWidget->get_second_deriv_minimization())
  {
    sst << "Using second derivatives for minimization." << endl;
  }

  if(fitsetWidget->get_second_deriv_covariance())
  {
    sst << "Using second derivatives for parameter covariance matrix." << endl;
  }

  if( (fitsetWidget->get_num_diff()) || (fitsetWidget->get_second_deriv_minimization()) || (fitsetWidget->get_second_deriv_covariance()) )
  {
    sst << "Numerical differentiation step size = " << fitsetWidget->get_num_diff_step() << endl;
  }

  sst << "Start lambda                 = " << fitsetWidget->get_startlambda() << endl;
  sst << "Lambda factor                = " << fitsetWidget->get_lambdafac() << endl;
  sst << "Delta(chi^2) tolerance       = " << fitsetWidget->get_tolerance() << endl;

  sst << "Normalization of correlation matrix: ";
  if(fitsetWidget->get_cov_normalization()==standard_normalization)
  {
    sst << "1/(N*(N-1))" << endl;
  }
  else if(fitsetWidget->get_cov_normalization()==bootstrap_normalization)
  {
    sst << "1/(N-1)" << endl;
  }
  else if(fitsetWidget->get_cov_normalization()==jackknife_normalization)
  {
    sst << "(N-1)/N" << endl;
  }


  sst << "(Pseudo-)Inversion method for correlation matrix: ";
  switch(fitsetWidget->get_inversion_method())
  {
    case diagonal:
      sst << "Diagonal only (uncorrelated fit)" << endl;
      break;
    case LU_inversion:
      sst << "LU decomposition" << endl;
      break;
    case simple_cut:
      sst << "SVD with fixed cut" << endl;
      sst << "Fixed SVD cut: " << fitsetWidget->get_svd() << endl;
      break;
    case ratio_cut:
      sst << "SVD with EV ratio cut" << endl;
      sst << "SVD EV ratio cut: " << fitsetWidget->get_svd_ratio() << endl;
      break;
    case absolute_cut:
      sst << "SVD with EV value cut" << endl;
      sst << "SVD EV value cut: " << fitsetWidget->get_svd_value() << endl;
      break;
    case off_diagonal_rescale:
      sst << "Rescale off-diagonal elements" << endl;
      sst << "Rescale factor: " << fitsetWidget->get_rescale_value() << endl;
      break;
    default:
      break;
  }

  sst << "Maximum number of iterations = " << fitsetWidget->get_steps() << endl;
  sst << "Bin size                     = " << fitsetWidget->get_bin() << endl;
  if((fitsetWidget->get_boot_prior()) && (fitsetWidget->get_bayes()))
  {
    sst << "Gaussian random priors for bootstrap/multifit activated." << endl;
  }

  rv->terminal->append(QString((sst.str()).c_str())); 
  rv->show();
}






void MainWindow::write_results()
{
  string dir=mainWidget->get_output_dir();
  if(dir.empty())
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("No output directory selected"));
    return;
  }
  if(*(dir.end()-1)!='/')
  {
    dir.append("/");
  }

  if(need_to_fit)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Fit data first"));
    return;
  }
// write covariance to file
  string cov_filename;
  if(curFile.isEmpty())
  {
    cov_filename=dir+"untitled.mbf_covariance.dat";
  }
  else
  {
    cov_filename=dir+strippedName(curFile).toStdString()+"_covariance.dat";
  }
  ofstream cov_output(cov_filename.c_str());
  if(!cov_output)
  {
    QMessageBox::warning(this, tr("QMBF"),
                                tr("Error: could not write covariance file."));
    return;
  }
  cov_output.precision(14);
  cov_output.setf(ios::left);
  int max_length=1;
  for(int p1=0; p1<parderWidget->get_n_parameters(); ++p1)
  {
    if(parderWidget->get_parameter(p1).length()>max_length)
    {
      max_length=parderWidget->get_parameter(p1).length();
    }
  }
  for(int p1=0; p1<parderWidget->get_n_parameters(); ++p1)
  {
    for(int p2=0; p2<parderWidget->get_n_parameters(); ++p2)
    {
      cov_output << setw(max_length+4) << parderWidget->get_parameter(p1) << setw(max_length+4) << parderWidget->get_parameter(p2) << setw(0) << _fitter->get_covariance(p1, p2) << endl;
    }
  }
  cov_output.close();

// write results to file
  string results_filename;
  if(curFile.isEmpty())
  {
    results_filename=dir+"untitled.mbf_results.dat";
  }
  else
  {
    results_filename=dir+strippedName(curFile).toStdString()+"_results.dat";
  }
  ofstream results_output(results_filename.c_str());
  if(!results_output)
  {
    QMessageBox::warning(this, tr("QMBF"),
                                tr("Error: could not write results file."));
    return;
  }
  results_output.precision(14);
  results_output.setf(ios::left);
  max_length=1;
  for(int p1=0; p1<parderWidget->get_n_parameters(); ++p1)
  {
    if(parderWidget->get_parameter(p1).length()>max_length)
    {
      max_length=parderWidget->get_parameter(p1).length();
    }
  }
  for(int p1=0; p1<parderWidget->get_n_parameters(); ++p1)
  {
    results_output << setw(max_length+4) << parderWidget->get_parameter(p1) << setw(0) << _fitter->get_parameter(p1) << endl;
  }
  results_output.close();
}



// *******************************************************************
// this function loads the data file; it refreshes the vectors
// "file_arguments" and "file_data"
// *******************************************************************
bool MainWindow::load_data_file()
{
  need_to_set_fit_data=true;
  need_to_fit=true;

  string filename=mainWidget->get_data_file();

  if(filename.empty())
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: no data file selected"));
    return false;
  }

  if (mainWidget->get_data_file_type()==0)
  {
    ifstream input(filename.c_str());

    if(!input)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: Could not open data file"));
      return false;
    }

    int n_functions;
    int n_variables;
    int n_data_points;
    int n_data_sets;

    input >> n_functions;
    input >> n_variables;
    input >> n_data_points;
    input >> n_data_sets;

    if(n_functions!=funcWidget->get_n_functions())
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: data file has wrong number of functions"));
      return false;
    }
    if(n_variables!=varsWidget->get_n_variables())
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: data file has wrong number of variables"));
      return false;
    }
    if( (n_data_sets<=0) || (n_data_points<=0) )
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: file contains no data"));
      return false;
    }

    file_arguments.clear();
    file_data.clear();

    vector< double > file_arg_temp(n_variables, 0.0);
    file_arguments.resize(n_data_points, file_arg_temp);

    vector< double > file_data_temp_1(n_functions, 0.0);
    vector< vector< double > > file_data_temp_2(n_data_points, file_data_temp_1);
    file_data.resize(n_data_sets, file_data_temp_2);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    int m_buf;
    for(int m=0; m<n_data_points; ++m)
    {
      input >> m_buf;
      if(m_buf!=m+1)
      {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error while reading data file"));
        return false;
      }
      for(int v=0; v<n_variables; ++v)
      {
        input >> file_arguments[m][v];
      }
    }

    int n_buf;
    for(int n=0; n<n_data_sets; ++n)
    {
      for(int m=0; m<n_data_points; ++m)
      {
        input >> n_buf >> m_buf;
        if( (n_buf!=n+1) || (m_buf!=m+1) )
        {
          QApplication::restoreOverrideCursor();
          QMessageBox::warning(this, tr("QMBF"),
                                     tr("Error while reading data file"));
          return false;
        }
        for(int f=0; f<n_functions; ++f)
        {
          input >> file_data[n][m][f];
        }
      }
    }

    input.close();
    QApplication::restoreOverrideCursor();

  }
  else if (mainWidget->get_data_file_type()==1)
  {
    ifstream input(filename.c_str(), ios::in | ios::binary);

    if(!input)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: Could not open data file"));
      return false;
    }

    int n_functions;
    int n_variables;
    int n_data_points;
    int n_data_sets;

    float temp;
    temp=read_float(input);
    n_functions=static_cast<int>(temp);

    temp=read_float(input);
    n_variables=static_cast<int>(temp);

    temp=read_float(input);
    n_data_points=static_cast<int>(temp);

    temp=read_float(input);
    n_data_sets=static_cast<int>(temp);

    if(n_functions!=funcWidget->get_n_functions())
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: data file has wrong number of functions"));
      return false;
    }
    if(n_variables!=varsWidget->get_n_variables())
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: data file has wrong number of variables"));
      return false;
    }
    if( (n_data_sets<=0) || (n_data_points<=0) )
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: file contains no data"));
      return false;
    }

    file_arguments.clear();
    file_data.clear();

    vector< double > file_arg_temp(n_variables, 0.0);
    file_arguments.resize(n_data_points, file_arg_temp);

    vector< double > file_data_temp_1(n_functions, 0.0);
    vector< vector< double > > file_data_temp_2(n_data_points, file_data_temp_1);
    file_data.resize(n_data_sets, file_data_temp_2);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    for(int m=0; m<n_data_points; ++m)
    {
      for(int v=0; v<n_variables; ++v)
      {
        file_arguments[m][v]=read_float(input);
      }
    }
    for(int n=0; n<n_data_sets; ++n)
    {
      for(int m=0; m<n_data_points; ++m)
      {
        for(int f=0; f<n_functions; ++f)
        {
          file_data[n][m][f]=read_float(input);
        }
      }
    }

    input.close();
    QApplication::restoreOverrideCursor();

  }

  need_to_load_data_file=false;

  return true;
}


// *******************************************************************
// reloading of data file
// *******************************************************************
void MainWindow::reload_data_file()
{
  need_to_set_fit_data=true;
  need_to_fit=true;
  load_data_file();
}





// *******************************************************************
// this function refreshes the vectors "fit_arguments" and "fit_data"
// it also assigns arguments and data to the fitter
// *******************************************************************
bool MainWindow::set_fit_data()
{
  need_to_fit=true;

  int n_data_points=file_arguments.size();
  int n_variables=file_arguments[0].size();
  int n_data_sets=file_data.size();

  int start_n_data;
  int stop_n_data;
  int fit_n_data_sets;

  if(fitsetWidget->get_restrict_data())
  {
    start_n_data=fitsetWidget->get_data_start();
    stop_n_data=fitsetWidget->get_data_stop();

    fit_n_data_sets=stop_n_data-start_n_data+1;

    if (start_n_data<1)
    {
      QMessageBox::warning(this, tr("QMBF"),
                   tr("Data range min must be 1 or larger"));
      return false;
    }

    if (stop_n_data>n_data_sets)
    {
      QMessageBox::warning(this, tr("QMBF"),
                   tr("Range of data sets exceeds data file"));
      return false;
    }

    if (fit_n_data_sets<5)
    {
      QMessageBox::warning(this, tr("QMBF"),
                   tr("Range of data sets too small"));
      return false;
    }

  }
  else
  {
    start_n_data=1;
    stop_n_data=n_data_sets;
    fit_n_data_sets=n_data_sets;
  }

  int n_functions=file_data[0][0].size();

// select data points in fitting range
  vector< parser* > fit_min_parsers;
  vector< parser* > fit_max_parsers;
  for(int v=0; v<n_variables; ++v)
  {
    fit_min_parsers.push_back(new parser(varsWidget->get_fit_min(v)));
    fit_max_parsers.push_back(new parser(varsWidget->get_fit_max(v)));
  }
  map< string, double > table;
  for(int c=0; c<constsWidget->get_n_constants(); ++c)
  {
    table[constsWidget->get_constant_name(c)]=constsWidget->get_constant_value(c);
  }
  fit_arguments.clear();
  vector< int > indices;
  for(int m=0; m<n_data_points; ++m)
  {
    for(int v=0; v<n_variables; ++v)
    {
      table[varsWidget->get_variable_name(v)]=file_arguments[m][v];
    }
    bool in_range=true;
    for(int v=0; v<n_variables; ++v)
    {
      double min=fit_min_parsers[v]->parse(table);
      double max=fit_max_parsers[v]->parse(table);
      if( (fit_min_parsers[v]->get_no_of_errors()>0) || (fit_max_parsers[v]->get_no_of_errors()>0))
      {
        for(vector< parser* >::iterator it=fit_min_parsers.begin(); it!=fit_min_parsers.end(); ++it)
        {
          delete *it;
        }
        for(vector< parser* >::iterator it=fit_max_parsers.begin(); it!=fit_max_parsers.end(); ++it)
        {
          delete *it;
        }
        QMessageBox::warning(this, tr("QMBF"),
                     tr("Error while parsing fitting range"));
        return false;
      }
      double arg=file_arguments[m][v];
      if( !( (min<=arg) && (arg<=max) ) )
      {
        in_range=false;
      }
    }
    if(in_range)
    {
      fit_arguments.push_back(file_arguments[m]);
      indices.push_back(m);
    }
  }
  for(vector< parser* >::iterator it=fit_min_parsers.begin(); it!=fit_min_parsers.end(); ++it)
  {
    delete *it;
  }
  for(vector< parser* >::iterator it=fit_max_parsers.begin(); it!=fit_max_parsers.end(); ++it)
  {
    delete *it;
  }
// end select data points in fitting range

  int fit_n_data_points=fit_arguments.size();

  if(fit_n_data_points==0)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: No data points in fitting range"));
    return false;
  }

  int bin_size=fitsetWidget->get_bin();

  if(fit_n_data_sets/bin_size<5)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: Bin size too large"));
    return false;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  vector< double > fit_data_temp_1(n_functions, 0.0);
  vector< vector< double > > fit_data_temp_2(fit_n_data_points, fit_data_temp_1);

  fit_data.clear();

  fit_data.resize(fit_n_data_sets/bin_size, fit_data_temp_2);

  for(int m=0; m<fit_n_data_points; ++m)
  {
    for(int f=0; f<n_functions; ++f)
    {
      for(int b=0; b<fit_n_data_sets/bin_size; ++b)
      {
        // average over entries e in bin b
        fit_data[b][m][f]=0.0;
        for(int e=0; e<bin_size; ++e)
        {
          fit_data[b][m][f]+=file_data[start_n_data-1+b*bin_size+e][indices[m]][f];
        }
        fit_data[b][m][f]/=bin_size;
      }
    }
  }

  string set_data_message;
  _fitter->set_data(fit_arguments, fit_data, set_data_message);

  QApplication::restoreOverrideCursor();

  int dof=_fitter->get_dof();
  int cut=_fitter->get_cut();

  if( (fitsetWidget->get_inversion_method()) != diagonal)
  {
    if( (dof-cut) > (fit_n_data_sets/bin_size) )
    {
      QMessageBox::warning(this, tr("QMBF"),
                                  tr("Not enough data sets (after binning) to obtain (pseudo-)inverse of data correlation matrix. Reduce bin size or increase SVD cut."));
      return false;
    }
  }

  if(set_data_message!="")
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr(set_data_message.c_str()));
  }

  stringstream message;

  message << "Normalization of correlation matrix: ";
  if(fitsetWidget->get_cov_normalization()==standard_normalization)
  {
    message << "1/(N*(N-1))" << endl;
  }
  else if(fitsetWidget->get_cov_normalization()==bootstrap_normalization)
  {
    message << "1/(N-1)" << endl;
  }
  else if(fitsetWidget->get_cov_normalization()==jackknife_normalization)
  {
    message << "(N-1)/N" << endl;
  }

  message << "(Pseudo-)Inversion method for data correlation matrix: ";
  switch(fitsetWidget->get_inversion_method())
  {
    case LU_inversion:
      message << "LU decomposition" << endl;
      break;
    case simple_cut:
      message << "SVD with fixed cut" << endl;
      message << "Fixed SVD cut: " << fitsetWidget->get_svd() << endl;
      break;
    case ratio_cut:
      message << "SVD with EV ratio cut" << endl;
      message << "SVD EV ratio cut: " << fitsetWidget->get_svd_ratio() << endl;
      break;
    case absolute_cut:
      message << "SVD with EV value cut" << endl;
      message << "SVD EV value cut: " << fitsetWidget->get_svd_value() << endl;
      break;
    case diagonal:
      message << "Diagonal only (uncorrelated fit)" << endl;
      break;
    default:
      break;
  }

  message << "Kept " << dof-cut << " out of " << dof << " eigenvalues." << endl;

  message << "Number of data sets (after binning) was " << fit_n_data_sets/bin_size << endl;

  mainWidget->print(message.str());

  need_to_set_fit_data=false;

  return true;
}



// *******************************************************************
// this function recreates "_fitter" with a new model and prior
// *******************************************************************
bool MainWindow::reset_fitter()
{
  need_to_load_data_file=true;
  need_to_set_fit_data=true;
  need_to_reset_fitter=true;
  need_to_fit=true;

  if (current_model==MULTIEXP)
  {
    delete _model;
    _model=new multi_exp_model(E_name, dE_name, A_name, B_name, t_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQR)
  {
    delete _model;
    _model=new multi_exp_Asqr_model(E_name, dE_name, A_name, B_name, t_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQREXPE)
  {
    delete _model;
    _model=new multi_exp_Asqr_expE_model(E_name, dE_name, A_name, B_name, t_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIEXPEXPA)
//  {
//    delete _model;
//    _model=new multi_exp_expA_model(E_name, dE_name, A_name, B_name, t_name, m_exp_dialog->get_n_exp());
//    plot_set_dialog->set_eff_mass(E_name);
//  }
  else if (current_model==MULTIEXPEXPE)
  {
    delete _model;
    _model=new multi_exp_expE_model(E_name, dE_name, A_name, B_name, t_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIEXPEXPAEXPE)
//  {
//    delete _model;
//    _model=new multi_exp_expA_expE_model(E_name, dE_name, A_name, B_name, t_name, m_exp_dialog->get_n_exp());
//    plot_set_dialog->set_eff_mass("exp("+E_name+")");
//  }
  else if (current_model==MULTIALTEXP)
  {
    delete _model;
    _model=new multi_alt_exp_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQR)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQREXPE)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_expE_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIALTEXPEXPA)
//  {
//    delete _model;
//    _model=new multi_alt_exp_expA_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
//    plot_set_dialog->set_eff_mass(E_name);
//  }
  else if (current_model==MULTIALTEXPEXPE)
  {
    delete _model;
    _model=new multi_alt_exp_expE_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIALTEXPEXPAEXPE)
//  {
//    delete _model;
//    _model=new multi_alt_exp_expA_expE_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
//    plot_set_dialog->set_eff_mass("exp("+E_name+")");
//  }

  else if (current_model==MULTIEXPVEC)
  {
    delete _model;
    _model=new multi_exp_vec_model(E_name, dE_name, A_name, B_name, t_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQRVEC)
  {
    delete _model;
    _model=new multi_exp_Asqr_vec_model(E_name, dE_name, A_name, B_name, t_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQREXPEVEC)
  {
    delete _model;
    _model=new multi_exp_Asqr_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIEXPEXPAVEC)
//  {
//    delete _model;
//    _model=new multi_exp_expA_vec_model(E_name, dE_name, A_name, B_name, t_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
//    plot_set_dialog->set_eff_mass(E_name);
//  }
  else if (current_model==MULTIEXPEXPEVEC)
  {
    delete _model;
    _model=new multi_exp_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIEXPEXPAEXPEVEC)
//  {
//    delete _model;
//    _model=new multi_exp_expA_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
//    plot_set_dialog->set_eff_mass("exp("+E_name+")");
//  }
  else if (current_model==MULTIALTEXPVEC)
  {
    delete _model;
    _model=new multi_alt_exp_vec_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQRVEC)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_vec_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQREXPEVEC)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIALTEXPEXPAVEC)
//  {
//    delete _model;
//    _model=new multi_alt_exp_expA_vec_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
//    plot_set_dialog->set_eff_mass(E_name);
//  }
  else if (current_model==MULTIALTEXPEXPEVEC)
  {
    delete _model;
    _model=new multi_alt_exp_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIALTEXPEXPAEXPEVEC)
//  {
//    delete _model;
//    _model=new multi_alt_exp_expA_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
//    plot_set_dialog->set_eff_mass("exp("+E_name+")");
//  }

  else if (current_model==MULTIEXPMAT)
  {
    delete _model;
    _model=new multi_exp_mat_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_dialog->get_n_exp(), m_exp_mat_dialog->get_dim_1(), m_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPEXPEMAT)
  {
    delete _model;
    _model=new multi_exp_expE_mat_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_dialog->get_n_exp(), m_exp_mat_dialog->get_dim_1(), m_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIEXPMATII)
  {
    delete _model;
    _model=new multi_exp_mat_II_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_dialog->get_n_exp(), m_exp_mat_dialog->get_dim_1(), m_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPEXPEMATII)
  {
    delete _model;
    _model=new multi_exp_expE_mat_II_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_dialog->get_n_exp(), m_exp_mat_dialog->get_dim_1(), m_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }

  else if (current_model==MULTIEXPMATUPPER)
  {
    delete _model;
    _model=new multi_exp_mat_upper_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_upper_dialog->get_n_exp(), m_exp_mat_upper_dialog->get_dim());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPEXPEMATUPPER)
  {
    delete _model;
    _model=new multi_exp_expE_mat_upper_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_upper_dialog->get_n_exp(), m_exp_mat_upper_dialog->get_dim());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIEXPMATIIUPPER)
  {
    delete _model;
    _model=new multi_exp_mat_II_upper_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_upper_dialog->get_n_exp(), m_exp_mat_upper_dialog->get_dim());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPEXPEMATIIUPPER)
  {
    delete _model;
    _model=new multi_exp_expE_mat_II_upper_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_upper_dialog->get_n_exp(), m_exp_mat_upper_dialog->get_dim());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }


  else if (current_model==MULTIALTEXPMAT)
  {
    delete _model;
    _model=new multi_alt_exp_mat_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_mat_dialog->get_n_exp(), m_alt_exp_mat_dialog->get_m_exp(), m_alt_exp_mat_dialog->get_dim_1(), m_alt_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPEXPEMAT)
  {
    delete _model;
    _model=new multi_alt_exp_expE_mat_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_mat_dialog->get_n_exp(), m_alt_exp_mat_dialog->get_m_exp(), m_alt_exp_mat_dialog->get_dim_1(), m_alt_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }


  else if (current_model==MULTIEXPNONSYMMAT)
  {
    delete _model;
    _model=new multi_exp_nonsym_mat_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_dialog->get_n_exp(), m_exp_mat_dialog->get_dim_1(), m_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPEXPENONSYMMAT)
  {
    delete _model;
    _model=new multi_exp_expE_nonsym_mat_model(E_name, dE_name, A_name, B_name, t_name, m_exp_mat_dialog->get_n_exp(), m_exp_mat_dialog->get_dim_1(), m_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIALTEXPNONSYMMAT)
  {
    delete _model;
    _model=new multi_alt_exp_nonsym_mat_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_mat_dialog->get_n_exp(), m_alt_exp_mat_dialog->get_m_exp(), m_alt_exp_mat_dialog->get_dim_1(), m_alt_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPEXPENONSYMMAT)
  {
    delete _model;
    _model=new multi_alt_exp_expE_nonsym_mat_model(E_name, dE_name, A_name, B_name, t_name, m_alt_exp_mat_dialog->get_n_exp(), m_alt_exp_mat_dialog->get_m_exp(), m_alt_exp_mat_dialog->get_dim_1(), m_alt_exp_mat_dialog->get_dim_2());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }


  else if (current_model==MULTIEXPBC)
  {
    delete _model;
    _model=new multi_exp_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQRBC)
  {
    delete _model;
    _model=new multi_exp_Asqr_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQREXPEBC)
  {
    delete _model;
    _model=new multi_exp_Asqr_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIEXPEXPABC)
//  {
//    delete _model;
//    _model=new multi_exp_expA_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_dialog->get_n_exp());
//    plot_set_dialog->set_eff_mass(E_name);
//  }
  else if (current_model==MULTIEXPEXPEBC)
  {
    delete _model;
    _model=new multi_exp_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIEXPEXPAEXPEBC)
//  {
//    delete _model;
//    _model=new multi_exp_expA_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_dialog->get_n_exp());
//    plot_set_dialog->set_eff_mass("exp("+E_name+")");
//  }
  else if (current_model==MULTIALTEXPBC)
  {
    delete _model;
    _model=new multi_alt_exp_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQRBC)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQREXPEBC)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIALTEXPEXPABC)
//  {
//    delete _model;
//    _model=new multi_alt_exp_expA_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
//    plot_set_dialog->set_eff_mass(E_name);
//  }
  else if (current_model==MULTIALTEXPEXPEBC)
  {
    delete _model;
    _model=new multi_alt_exp_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIALTEXPEXPAEXPEBC)
//  {
//    delete _model;
//    _model=new multi_alt_exp_expA_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
//    plot_set_dialog->set_eff_mass("exp("+E_name+")");
//  }

  else if (current_model==MULTIEXPVECBC)
  {
    delete _model;
    _model=new multi_exp_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQRVECBC)
  {
    delete _model;
    _model=new multi_exp_Asqr_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQREXPEVECBC)
  {
    delete _model;
    _model=new multi_exp_Asqr_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIEXPEXPAVECBC)
//  {
//    delete _model;
//    _model=new multi_exp_expA_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
//    plot_set_dialog->set_eff_mass(E_name);
//  }
  else if (current_model==MULTIEXPEXPEVECBC)
  {
    delete _model;
    _model=new multi_exp_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIEXPEXPAEXPEVECBC)
//  {
//    delete _model;
//    _model=new multi_exp_expA_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
//    plot_set_dialog->set_eff_mass("exp("+E_name+")");
//  }
  else if (current_model==MULTIALTEXPVECBC)
  {
    delete _model;
    _model=new multi_alt_exp_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQRVECBC)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQREXPEVECBC)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIALTEXPEXPAVECBC)
//  {
//    delete _model;
//    _model=new multi_alt_exp_expA_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
//    plot_set_dialog->set_eff_mass(E_name);
//  }
  else if (current_model==MULTIALTEXPEXPEVECBC)
  {
    delete _model;
    _model=new multi_alt_exp_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
//  else if (current_model==MULTIALTEXPEXPAEXPEVECBC)
//  {
//    delete _model;
//    _model=new multi_alt_exp_expA_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
//    plot_set_dialog->set_eff_mass("exp("+E_name+")");
//  }
  else if (current_model==THREEPT2VARMULTIEXP)
  {
    delete _model;
    _model=new threept_multi_exp_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, threept_m_exp_dialog->get_n_exp_initial(), threept_m_exp_dialog->get_n_exp_final());
    plot_set_dialog->set_eff_mass("");
  }
  else if (current_model==THREEPT2VARMULTIEXPEXPE)
  {
    delete _model;
    _model=new threept_multi_exp_expE_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, threept_m_exp_dialog->get_n_exp_initial(), threept_m_exp_dialog->get_n_exp_final());
    plot_set_dialog->set_eff_mass("");
  }
  else if (current_model==THREEPT2VARMULTIEXPVEC)
  {
    delete _model;
    _model=new threept_multi_exp_vec_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, threept_m_exp_vec_dialog->get_n_exp_initial(), threept_m_exp_vec_dialog->get_n_exp_final(), threept_m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("");
  }
  else if (current_model==THREEPT2VARMULTIEXPEXPEVEC)
  {
    delete _model;
    _model=new threept_multi_exp_expE_vec_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, threept_m_exp_vec_dialog->get_n_exp_initial(), threept_m_exp_vec_dialog->get_n_exp_final(), threept_m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("");
  }
  else if (current_model==THREEPT2VARMULTIALTEXP)
  {
    delete _model;
    _model=new threept_multi_alt_exp_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, threept_m_alt_exp_dialog->get_n_exp_initial(), threept_m_alt_exp_dialog->get_m_exp_initial(), threept_m_alt_exp_dialog->get_n_exp_final(), threept_m_alt_exp_dialog->get_m_exp_final());
    plot_set_dialog->set_eff_mass("");
  }
  else if (current_model==THREEPT2VARMULTIALTEXPEXPE)
  {
    delete _model;
    _model=new threept_multi_alt_exp_expE_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, threept_m_alt_exp_dialog->get_n_exp_initial(), threept_m_alt_exp_dialog->get_m_exp_initial(), threept_m_alt_exp_dialog->get_n_exp_final(), threept_m_alt_exp_dialog->get_m_exp_final());
    plot_set_dialog->set_eff_mass("");
  }
  else if (current_model==THREEPT2VARMULTIALTEXPVEC)
  {
    delete _model;
    _model=new threept_multi_alt_exp_vec_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, threept_m_alt_exp_vec_dialog->get_n_exp_initial(), threept_m_alt_exp_vec_dialog->get_m_exp_initial(), threept_m_alt_exp_vec_dialog->get_n_exp_final(), threept_m_alt_exp_vec_dialog->get_m_exp_final(), threept_m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("");
  }
  else if (current_model==THREEPT2VARMULTIALTEXPEXPEVEC)
  {
    delete _model;
    _model=new threept_multi_alt_exp_expE_vec_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, threept_m_alt_exp_vec_dialog->get_n_exp_initial(), threept_m_alt_exp_vec_dialog->get_m_exp_initial(), threept_m_alt_exp_vec_dialog->get_n_exp_final(), threept_m_alt_exp_vec_dialog->get_m_exp_final(), threept_m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("");
  }
  else if (current_model==MULTIEXPCONST)
  {
    delete _model;
    _model=new multi_exp_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQRCONST)
  {
    delete _model;
    _model=new multi_exp_Asqr_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQREXPECONST)
  {
    delete _model;
    _model=new multi_exp_Asqr_expE_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIEXPEXPECONST)
  {
    delete _model;
    _model=new multi_exp_expE_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIALTEXPCONST)
  {
    delete _model;
    _model=new multi_alt_exp_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQRCONST)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQREXPECONST)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_expE_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIALTEXPEXPECONST)
  {
    delete _model;
    _model=new multi_alt_exp_expE_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIEXPVECCONST)
  {
    delete _model;
    _model=new multi_exp_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQRVECCONST)
  {
    delete _model;
    _model=new multi_exp_Asqr_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQREXPEVECCONST)
  {
    delete _model;
    _model=new multi_exp_Asqr_expE_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIEXPEXPEVECCONST)
  {
    delete _model;
    _model=new multi_exp_expE_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIALTEXPVECCONST)
  {
    delete _model;
    _model=new multi_alt_exp_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQRVECCONST)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQREXPEVECCONST)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_expE_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIALTEXPEXPEVECCONST)
  {
    delete _model;
    _model=new multi_alt_exp_expE_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIEXPBCCONST)
  {
    delete _model;
    _model=new multi_exp_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQRBCCONST)
  {
    delete _model;
    _model=new multi_exp_Asqr_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQREXPEBCCONST)
  {
    delete _model;
    _model=new multi_exp_Asqr_expE_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIEXPEXPEBCCONST)
  {
    delete _model;
    _model=new multi_exp_expE_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_exp_dialog->get_n_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIALTEXPBCCONST)
  {
    delete _model;
    _model=new multi_alt_exp_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQRBCCONST)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQREXPEBCCONST)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_expE_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIALTEXPEXPEBCCONST)
  {
    delete _model;
    _model=new multi_alt_exp_expE_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_alt_exp_dialog->get_n_exp(), m_alt_exp_dialog->get_m_exp());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIEXPVECBCCONST)
  {
    delete _model;
    _model=new multi_exp_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQRVECBCCONST)
  {
    delete _model;
    _model=new multi_exp_Asqr_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIEXPASQREXPEVECBCCONST)
  {
    delete _model;
    _model=new multi_exp_Asqr_expE_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIEXPEXPEVECBCCONST)
  {
    delete _model;
    _model=new multi_exp_expE_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_exp_vec_dialog->get_n_exp(), m_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIALTEXPVECBCCONST)
  {
    delete _model;
    _model=new multi_alt_exp_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQRVECBCCONST)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass(E_name);
  }
  else if (current_model==MULTIALTEXPASQREXPEVECBCCONST)
  {
    delete _model;
    _model=new multi_alt_exp_Asqr_expE_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else if (current_model==MULTIALTEXPEXPEVECBCCONST)
  {
    delete _model;
    _model=new multi_alt_exp_expE_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, m_alt_exp_vec_dialog->get_n_exp(), m_alt_exp_vec_dialog->get_m_exp(), m_alt_exp_vec_dialog->get_n_vec());
    plot_set_dialog->set_eff_mass("exp("+E_name+")");
  }
  else // use PARSE
  {

    vector< string > functions(funcWidget->get_n_functions());
    vector< string > variables(varsWidget->get_n_variables());
    vector< string > parameters(parderWidget->get_n_parameters());
    vector< string > constants(constsWidget->get_n_constants());
    vector< string > der_temp_vec(parderWidget->get_n_parameters());
    vector< vector< string > > derivatives(funcWidget->get_n_functions(), der_temp_vec);

    for(int f=0; f<funcWidget->get_n_functions(); ++f)
    {
      functions[f]=funcWidget->get_function(f);
      if(functions[f].empty())
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: function missing"));
        return false;
      }
      if( _in("{", functions[f]) || _in("&", functions[f]) || _in("&", functions[f]) || _in("!", functions[f]) || _in("}", functions[f])
       || _in("%", functions[f]) || _in("?", functions[f]) || _in("[", functions[f]) || _in(":", functions[f])
       || _in(">", functions[f]) || _in("#", functions[f]) || _in("~", functions[f]) || _in("$", functions[f])
       || _in("]", functions[f]) || _in("<", functions[f]) )
      {
        stringstream sst;
        sst << "Error: forbidden character in function " << f+1 << endl;
        QMessageBox::warning(this, tr("QMBF"),
                                   tr(sst.str().c_str()));
        return false;
      }
    }

    for(int v=0; v<varsWidget->get_n_variables(); ++v)
    {
      if((varsWidget->get_variable_name(v)).empty())
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: variable name missing"));
        return false;
      }
      if(std::find(variables.begin(), variables.end(), varsWidget->get_variable_name(v))!=variables.end())
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: variable name repeated"));
        return false;
      }
      variables[v]=varsWidget->get_variable_name(v);
      if( _in("{", variables[v]) || _in("&", variables[v]) || _in("&", variables[v]) || _in("!", variables[v]) || _in("}", variables[v])
       || _in("%", variables[v]) || _in("?", variables[v]) || _in("[", variables[v]) || _in(":", variables[v])
       || _in(">", variables[v]) || _in("#", variables[v]) || _in("~", variables[v]) || _in("$", variables[v])
       || _in("]", variables[v]) || _in("<", variables[v]) ||
                     _in("arcsin", variables[v]) ||  _in("sinh", variables[v]) ||
                     _in("sin", variables[v]) ||  _in("arccos", variables[v]) ||
                     _in("cosh", variables[v]) ||  _in("cos", variables[v]) ||
                     _in("arctan", variables[v]) ||  _in("tanh", variables[v]) ||
                     _in("tan", variables[v]) ||  _in("exp", variables[v]) ||
                     _in("log", variables[v]) ||  _in("sqrt", variables[v]) ||
                     _in("sqr", variables[v]) ||  _in("alt", variables[v]) )
      {
        stringstream sst;
        sst << "Error: forbidden character or substring in variable " << v+1 << endl;
        QMessageBox::warning(this, tr("QMBF"),
                                   tr(sst.str().c_str()));
        return false;
      }
    }

    for(int p=0; p<parderWidget->get_n_parameters(); ++p)
    {
      if((parderWidget->get_parameter(p)).empty())
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: parameter name missing"));
        return false;
      }
      if(std::find(parameters.begin(), parameters.end(), parderWidget->get_parameter(p))!=parameters.end())
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: parameter name repeated"));
        return false;
      }
      parameters[p]=parderWidget->get_parameter(p);
      if( _in("{", parameters[p]) || _in("&", parameters[p]) || _in("&", parameters[p]) || _in("!", parameters[p]) || _in("}", parameters[p])
       || _in("%", parameters[p]) || _in("?", parameters[p]) || _in("[", parameters[p]) || _in(":", parameters[p])
       || _in(">", parameters[p]) || _in("#", parameters[p]) || _in("~", parameters[p]) || _in("$", parameters[p])
       || _in("]", parameters[p]) || _in("<", parameters[p]) ||
                     _in("arcsin", parameters[p]) ||  _in("sinh", parameters[p]) ||
                     _in("sin", parameters[p]) ||  _in("arccos", parameters[p]) ||
                     _in("cosh", parameters[p]) ||  _in("cos", parameters[p]) ||
                     _in("arctan", parameters[p]) ||  _in("tanh", parameters[p]) ||
                     _in("tan", parameters[p]) ||  _in("exp", parameters[p]) ||
                     _in("log", parameters[p]) ||  _in("sqrt", parameters[p]) ||
                     _in("sqr", parameters[p]) ||  _in("alt", parameters[p]) )
      {
        stringstream sst;
        sst << "Error: forbidden character or substring in parameter " << p+1 << endl;
        QMessageBox::warning(this, tr("QMBF"),
                                   tr(sst.str().c_str()));
        return false;
      }
    }

    for(int c=0; c<constsWidget->get_n_constants(); ++c)
    {
      if((constsWidget->get_constant_name(c)).empty())
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: constant name missing"));
        return false;
      }
      if(std::find(constants.begin(), constants.end(), constsWidget->get_constant_name(c))!=constants.end())
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: constant name repeated"));
        return false;
      }
      constants[c]=constsWidget->get_constant_name(c);
      if( _in("{", constants[c]) || _in("&", constants[c]) || _in("&", constants[c]) || _in("!", constants[c]) || _in("}", constants[c])
       || _in("%", constants[c]) || _in("?", constants[c]) || _in("[", constants[c]) || _in(":", constants[c])
       || _in(">", constants[c]) || _in("#", constants[c]) || _in("~", constants[c]) || _in("$", constants[c])
       || _in("]", constants[c]) || _in("<", constants[c]) ||
                     _in("arcsin", constants[c]) ||  _in("sinh", constants[c]) ||
                     _in("sin", constants[c]) ||  _in("arccos", constants[c]) ||
                     _in("cosh", constants[c]) ||  _in("cos", constants[c]) ||
                     _in("arctan", constants[c]) ||  _in("tanh", constants[c]) ||
                     _in("tan", constants[c]) ||  _in("exp", constants[c]) ||
                     _in("log", constants[c]) ||  _in("sqrt", constants[c]) ||
                     _in("sqr", constants[c]) ||  _in("alt", constants[c]) )
      {
        stringstream sst;
        sst << "Error: forbidden character or substring in constant " << c+1 << endl;
        QMessageBox::warning(this, tr("QMBF"),
                                   tr(sst.str().c_str()));
        return false;
      }
    }

    if(!(fitsetWidget->get_num_diff()))
    {
      for(int f=0; f<funcWidget->get_n_functions(); ++f)
      {
        for(int p=0; p<parderWidget->get_n_parameters(); ++p)
        {
          derivatives[f][p]=parderWidget->get_derivative(f, p);
          if(derivatives[f][p].empty())
          {
            stringstream sst;
            sst << "Error: derivative" << endl;
            sst << "of function " << f+1 << endl;
            sst << "by parameter " << p+1 << endl;
            sst << "missing" << endl;

            QMessageBox::warning(this, tr("QMBF"),
                                       tr(sst.str().c_str()));
            return false;
          }
          if( _in("{", derivatives[f][p]) || _in("&", derivatives[f][p]) || _in("&", derivatives[f][p]) || _in("!", derivatives[f][p]) || _in("}", derivatives[f][p])
           || _in("%", derivatives[f][p]) || _in("?", derivatives[f][p]) || _in("[", derivatives[f][p]) || _in(":", derivatives[f][p])
           || _in(">", derivatives[f][p]) || _in("#", derivatives[f][p]) || _in("~", derivatives[f][p]) || _in("$", derivatives[f][p])
           || _in("]", derivatives[f][p]) || _in("<", derivatives[f][p]) )
          {
            stringstream sst;
            sst << "Error: forbidden character in derivative" << endl;
            sst << "of function " << f+1 << endl;
            sst << "by parameter " << p+1 << endl;

            QMessageBox::warning(this, tr("QMBF"),
                                       tr(sst.str().c_str()));
            return false;
          }
        }
      }
    }

    delete _model;
    _model=new parse_model(functions, variables, parameters, constants, derivatives);
//    plot_set_dialog->set_eff_mass("");
  }

  delete _gaussian_prior;
  _gaussian_prior=new gaussian_prior(_model->get_n_parameters());


  string chisqr_extra_term_function="";
  vector< string > chisqr_extra_term_constant_names;
  double chisqr_extra_term_num_diff_step=1e-08;
  if(chisqrextraWidget->get_enabled())
  {
    chisqr_extra_term_function=chisqrextraWidget->get_function();
    if(chisqr_extra_term_function.empty())
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: chi^2 extra term function missing"));
      return false;
    }
    if( _in("{", chisqr_extra_term_function) || _in("&", chisqr_extra_term_function) || _in("&", chisqr_extra_term_function) || _in("!", chisqr_extra_term_function) || _in("}", chisqr_extra_term_function)
      || _in("%", chisqr_extra_term_function) || _in("?", chisqr_extra_term_function) || _in("[", chisqr_extra_term_function) || _in(":", chisqr_extra_term_function)
      || _in(">", chisqr_extra_term_function) || _in("#", chisqr_extra_term_function) || _in("~", chisqr_extra_term_function) || _in("$", chisqr_extra_term_function)
      || _in("]", chisqr_extra_term_function) || _in("<", chisqr_extra_term_function) )
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: forbidden character in chi^2 extra term function"));
      return false;
    }
    chisqr_extra_term_num_diff_step=chisqrextraWidget->get_num_diff_step();
    chisqr_extra_term_constant_names.resize(chisqrextraWidget->get_n_constants(), "");
    for(int c=0; c<chisqrextraWidget->get_n_constants(); ++c)
    {
      if((chisqrextraWidget->get_constant_name(c)).empty())
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: chi^2 extra term: constant name missing"));
        return false;
      }
      if(std::find(chisqr_extra_term_constant_names.begin(), chisqr_extra_term_constant_names.end(), chisqrextraWidget->get_constant_name(c))!=chisqr_extra_term_constant_names.end())
      {
        QMessageBox::warning(this, tr("QMBF"),
                                   tr("Error: chi^2 extra term: constant name repeated"));
        return false;
      }
      chisqr_extra_term_constant_names[c]=chisqrextraWidget->get_constant_name(c);
      if( _in("{", chisqr_extra_term_constant_names[c]) || _in("&", chisqr_extra_term_constant_names[c]) || _in("&", chisqr_extra_term_constant_names[c]) || _in("!", chisqr_extra_term_constant_names[c]) || _in("}", chisqr_extra_term_constant_names[c])
       || _in("%", chisqr_extra_term_constant_names[c]) || _in("?", chisqr_extra_term_constant_names[c]) || _in("[", chisqr_extra_term_constant_names[c]) || _in(":", chisqr_extra_term_constant_names[c])
       || _in(">", chisqr_extra_term_constant_names[c]) || _in("#", chisqr_extra_term_constant_names[c]) || _in("~", chisqr_extra_term_constant_names[c]) || _in("$", chisqr_extra_term_constant_names[c])
       || _in("]", chisqr_extra_term_constant_names[c]) || _in("<", chisqr_extra_term_constant_names[c]) ||
                     _in("arcsin", chisqr_extra_term_constant_names[c]) ||  _in("sinh", chisqr_extra_term_constant_names[c]) ||
                     _in("sin", chisqr_extra_term_constant_names[c]) ||  _in("arccos", chisqr_extra_term_constant_names[c]) ||
                     _in("cosh", chisqr_extra_term_constant_names[c]) ||  _in("cos", chisqr_extra_term_constant_names[c]) ||
                     _in("arctan", chisqr_extra_term_constant_names[c]) ||  _in("tanh", chisqr_extra_term_constant_names[c]) ||
                     _in("tan", chisqr_extra_term_constant_names[c]) ||  _in("exp", chisqr_extra_term_constant_names[c]) ||
                     _in("log", chisqr_extra_term_constant_names[c]) ||  _in("sqrt", chisqr_extra_term_constant_names[c]) ||
                     _in("sqr", chisqr_extra_term_constant_names[c]) ||  _in("alt", chisqr_extra_term_constant_names[c]) )
      {
        stringstream sst;
        sst << "Error: forbidden character or substring in chi^2 extra term constant " << c+1 << endl;
        QMessageBox::warning(this, tr("QMBF"),
                                   tr(sst.str().c_str()));
        return false;
      }
    }
    for(int p=0; p<_model->get_n_parameters(); ++p)
    {
      if(std::find(chisqr_extra_term_constant_names.begin(), chisqr_extra_term_constant_names.end(), _model->get_parameter_name(p))!=chisqr_extra_term_constant_names.end())
      {
        stringstream sst;
        sst << "Error: constant name \"" << _model->get_parameter_name(p) << "\" in chi^2 extra term function is equal to a fit parameter name " << std::endl << std::endl;
        QMessageBox::warning(this, tr("QMBF"),
                                   tr(sst.str().c_str()));
        return false;
      }
    }
    for(int c=0; c<_model->get_n_constants(); ++c)
    {
      if(std::find(chisqr_extra_term_constant_names.begin(), chisqr_extra_term_constant_names.end(), _model->get_constant_name(c))!=chisqr_extra_term_constant_names.end())
      {
        stringstream sst;
        sst << "Error: constant name \"" << _model->get_constant_name(c) << "\" in chi^2 extra term function is equal to a fit constant name " << std::endl << std::endl;
        QMessageBox::warning(this, tr("QMBF"),
                                   tr(sst.str().c_str()));
        return false;
      }
    }
  }
  delete _chisqr_extra_term;
  _chisqr_extra_term = new chisqr_extra_term(_model->get_n_parameters(), chisqr_extra_term_constant_names.size(), chisqr_extra_term_function);
  _chisqr_extra_term->set_enabled(chisqrextraWidget->get_enabled());
  _chisqr_extra_term->set_num_diff_step(chisqr_extra_term_num_diff_step);
  for(int p=0; p<_model->get_n_parameters(); ++p)
  {
    _chisqr_extra_term->set_parameter_name(p, _model->get_parameter_name(p));
  }
  _chisqr_extra_term->init_parameter_list();
  for(int c=0; c<chisqr_extra_term_constant_names.size(); ++c)
  {
    _chisqr_extra_term->set_constant_name(c, chisqr_extra_term_constant_names[c]);
  }


  delete _fitter;
  _fitter=new fitter(_model, _gaussian_prior, _chisqr_extra_term);
  connect(_fitter, SIGNAL(step_done(int, double, double)), fitdialog, SLOT(step_done(int, double, double)));

  fitthread -> set_fitter(_fitter);
  bootstrapthread -> set_fitter(_fitter);
  bootstrapthread -> set_gaussian_prior(_gaussian_prior);
  multifitthread -> set_fitter(_fitter);
  multifitthread -> set_gaussian_prior(_gaussian_prior);

  need_to_reset_fitter=false;

  return true;
}
