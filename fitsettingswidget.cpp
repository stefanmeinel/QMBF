#include "fitsettingswidget.h"

FitSettingsWidget::FitSettingsWidget(bool bayes, bool num_diff, bool second_deriv_minimization, bool second_deriv_covariance, double num_diff_step, bool use_bse, bool restrict_data, int startdata, int stopdata, double startlambda, double lambdafac, double tolerance,
                      int svd, double svd_ratio, double svd_value, int steps, int bin, int bssamples, QWidget* parent)
  : QWidget(parent)
{
  start_bayes=bayes;
  start_num_diff=num_diff;
  start_second_deriv_minimization=second_deriv_minimization;
  start_second_deriv_covariance=second_deriv_covariance;
  start_num_diff_step=num_diff_step;
  start_use_bse=use_bse;
  start_restrict_data=restrict_data;
  startdata0=startdata;
  stopdata0=stopdata;
  startlambda0=startlambda;
  lambdafac0=lambdafac;
  tolerance0=tolerance;
  svd0=svd;
  svd_ratio0=svd_ratio;
  svd_value0=svd_value;
  steps0=steps;
  bin0=bin;
  bssamples0=bssamples;
  createGadgets();
  reset();
}

void FitSettingsWidget::set_bayes(bool bayes)
{
  bayes_cb->setChecked(bayes);
}

bool FitSettingsWidget::get_bayes()
{
  return bayes_cb->isChecked();
}

void FitSettingsWidget::set_use_bse(bool use_bse)
{
  use_bse_cb->setChecked(use_bse);
}

bool FitSettingsWidget::get_use_bse()
{
  return use_bse_cb->isChecked();
}

void FitSettingsWidget::set_boot_prior(bool boot_prior)
{
  boot_prior_cb->setChecked(boot_prior);
}


bool FitSettingsWidget::get_boot_prior()
{
  return boot_prior_cb->isChecked();
}


void FitSettingsWidget::set_restrict_data(bool restrict_data)
{
  restrict_data_cb->setChecked(restrict_data);
}


bool FitSettingsWidget::get_restrict_data()
{
  return restrict_data_cb->isChecked();
}

void FitSettingsWidget::set_num_diff(bool num_diff)
{
  num_diff_cb->setChecked(num_diff);
}

bool FitSettingsWidget::get_num_diff()
{
  return num_diff_cb->isChecked();
}


void FitSettingsWidget::set_second_deriv_minimization(bool second_deriv_minimization)
{
  second_deriv_minimization_cb->setChecked(second_deriv_minimization);
}

bool FitSettingsWidget::get_second_deriv_minimization()
{
  return second_deriv_minimization_cb->isChecked();
}

void FitSettingsWidget::set_second_deriv_covariance(bool second_deriv_covariance)
{
  second_deriv_covariance_cb->setChecked(second_deriv_covariance);
}

bool FitSettingsWidget::get_second_deriv_covariance()
{
  return second_deriv_covariance_cb->isChecked();
}


void FitSettingsWidget::set_num_diff_step(double num_diff_step)
{
  numdiffEd->setText(double_to_QString(num_diff_step));
}


double FitSettingsWidget::get_num_diff_step()
{
  return QString_to_double(numdiffEd->text());
}


void FitSettingsWidget::set_startlambda(double startlambda)
{
  startlambdaEd->setText(double_to_QString(startlambda));
}

double FitSettingsWidget::get_startlambda()
{
  return QString_to_double(startlambdaEd->text());
}

void FitSettingsWidget::set_lambdafac(double lambdafac)
{
  lambdafacEd->setText(double_to_QString(lambdafac));
}

double FitSettingsWidget::get_lambdafac()
{
  return QString_to_double(lambdafacEd->text());
}

void FitSettingsWidget::set_tolerance(double tolerance)
{
  toleranceEd->setText(double_to_QString(tolerance));
}

double FitSettingsWidget::get_tolerance()
{
  return QString_to_double(toleranceEd->text());
}


void FitSettingsWidget::set_bootstrap_normalization(bool bootstrap_normalization)
{
  if(bootstrap_normalization)
  {
    bootstrapNormalizationCb->setCurrentIndex(1);
  }
  else
  {
    bootstrapNormalizationCb->setCurrentIndex(0);
  }
}


bool FitSettingsWidget::get_bootstrap_normalization()
{
  if(bootstrapNormalizationCb->currentIndex()==1)
  {
    return true;
  }
  else
  {
    return false;
  }
}



void FitSettingsWidget::set_inversion_method(inversion_method method)
{
  int index=0;
  switch(method)
  {
    case LU_inversion:
      index=0;
      break;
    case simple_cut:
      index=1;
      break;
    case ratio_cut:
      index=2;
      break;
    case absolute_cut:
      index=3;
      break;
    case diagonal:
      index=4;
      break;
    default:
      break;
  }
  invMethodCb->setCurrentIndex(index);
}


inversion_method FitSettingsWidget::get_inversion_method()
{
  int index=invMethodCb->currentIndex();
  inversion_method result=LU_inversion;
  switch(index)
  {
    case 0:
      result=LU_inversion;
      break;
    case 1:
      result=simple_cut;
      break;
    case 2:
      result=ratio_cut;
      break;
    case 3:
      result=absolute_cut;
      break;
    case 4:
      result=diagonal;
      break;
    default:
      break;
  }
  return result;
}

void FitSettingsWidget::set_svd(int svd_cut)
{
  svdEd->setText(int_to_QString(svd_cut));
}

int FitSettingsWidget::get_svd()
{
  return QString_to_int(svdEd->text());
}


void FitSettingsWidget::set_data_start(int start)
{
  startdataEd->setText(int_to_QString(start));
}

int FitSettingsWidget::get_data_start()
{
  return QString_to_int(startdataEd->text());
}


void FitSettingsWidget::set_data_stop(int stop)
{
  stopdataEd->setText(int_to_QString(stop));
}

int FitSettingsWidget::get_data_stop()
{
  return QString_to_int(stopdataEd->text());
}


void FitSettingsWidget::set_svd_ratio(double ratio)
{
  svdRatioEd->setText(double_to_QString(ratio));
}

double FitSettingsWidget::get_svd_ratio()
{
  return QString_to_double(svdRatioEd->text());
}

void FitSettingsWidget::set_svd_value(double value)
{
  svdValueEd->setText(double_to_QString(value));
}

double FitSettingsWidget::get_svd_value()
{
  return QString_to_double(svdValueEd->text());
}

void FitSettingsWidget::set_steps(int steps)
{
  stepsEd->setText(int_to_QString(steps));
}

int FitSettingsWidget::get_steps()
{
  return QString_to_int(stepsEd->text());
}

void FitSettingsWidget::set_bssamples(int bssamples)
{
  bssamplesEd->setText(int_to_QString(bssamples));
}

int FitSettingsWidget::get_bssamples()
{
  return QString_to_int(bssamplesEd->text());
}

void FitSettingsWidget::set_bin(int size)
{
  binningSB->setValue(size);
}

int FitSettingsWidget::get_bin()
{
  return binningSB->value();
}

std::string FitSettingsWidget::get_boot_file()
{
  return (bootfileEd->text()).toStdString();
}

void FitSettingsWidget::set_boot_file(std::string name)
{
   bootfileEd->setText(QString(name.c_str()));
}


void FitSettingsWidget::reset()
{
  bayes_cb->setChecked(start_bayes);
  boot_prior_cb->setChecked(start_bayes);
  boot_prior_cb->setEnabled(start_bayes);
  num_diff_cb->setChecked(start_num_diff);
  second_deriv_minimization_cb->setChecked(start_second_deriv_minimization);
  second_deriv_covariance_cb->setChecked(start_second_deriv_covariance);
  numdiffEd->setText(double_to_QString(start_num_diff_step));
  if(start_num_diff)
  {
    numdiffmodified_slot(Qt::Checked);
  }
  else
  {
    numdiffmodified_slot(Qt::Unchecked);
  }

  if(start_second_deriv_minimization)
  {
    secondderivminimizationmodified_slot(Qt::Checked);
  }
  else
  {
    secondderivminimizationmodified_slot(Qt::Unchecked);
  }

  if(start_second_deriv_covariance)
  {
    secondderivcovariancemodified_slot(Qt::Checked);
  }
  else
  {
    secondderivcovariancemodified_slot(Qt::Unchecked);
  }

  use_bse_cb->setChecked(start_use_bse);
  if(start_use_bse)
  {
    usebsemodified_slot(Qt::Checked);
  }
  else
  {
    usebsemodified_slot(Qt::Unchecked);
  }
  restrict_data_cb->setChecked(start_restrict_data);
  if(start_restrict_data)
  {
    restrictdata_modified_slot(Qt::Checked);
  }
  else
  {
    restrictdata_modified_slot(Qt::Unchecked);
  }
  startdataEd->setText(int_to_QString(startdata0));
  stopdataEd->setText(int_to_QString(stopdata0));
  startlambdaEd->setText(double_to_QString(startlambda0));
  lambdafacEd->setText(double_to_QString(lambdafac0));
  toleranceEd->setText(double_to_QString(tolerance0));
  invMethodCb->setCurrentIndex(0);
  inv_method_modified_slot(0);
  bootstrapNormalizationCb->setCurrentIndex(0);
  bootstrap_normalization_modified_slot(0);
  svdEd->setText(int_to_QString(svd0));
  svdRatioEd->setText(double_to_QString(svd_ratio0));
  svdValueEd->setText(double_to_QString(svd_value0));
  stepsEd->setText(double_to_QString(steps0));
  binningSB->setValue(bin0);
  bssamplesEd->setText(double_to_QString(bssamples0));
  bootfileEd->setText(QString(""));
}

void FitSettingsWidget::modified_slot()
{
  emit modified();
}

void FitSettingsWidget::bayesianmodified_slot(int newState)
{
  boot_prior_cb->setEnabled(get_bayes());
  emit bayesianmodified(newState);
}


void FitSettingsWidget::numdiffmodified_slot(int newState)
{
/*
  if(newState == Qt::Checked)
  {
    numdiffEd->setEnabled(true);
    numdiffLb->setEnabled(true);
  }
  else if(newState == Qt::Unchecked)
  {
    numdiffEd->setEnabled(false);
    numdiffLb->setEnabled(false);
  }
*/
  emit numdiffmodified(newState);
}


void FitSettingsWidget::secondderivminimizationmodified_slot(int newState)
{
  emit secondderivminimizationmodified(newState);
}


void FitSettingsWidget::secondderivcovariancemodified_slot(int newState)
{
  emit secondderivcovariancemodified(newState);
}


void FitSettingsWidget::usebsemodified_slot(int newState)
{
  if(newState == Qt::Checked)
  {
    bootfileEd->setEnabled(true);
    bootfileLb->setEnabled(true);
    bootfilebutton->setEnabled(true);
    newbootfilebutton->setEnabled(true);
    bssamplesEd->setEnabled(false);
    bssamplesLb->setEnabled(false);
  }
  else if(newState == Qt::Unchecked)
  {
    bootfileEd->setEnabled(false);
    bootfileLb->setEnabled(false);
    bootfilebutton->setEnabled(false);
    newbootfilebutton->setEnabled(false);
    bssamplesEd->setEnabled(true);
    bssamplesLb->setEnabled(true);
  }
}


void FitSettingsWidget::restrictdata_modified_slot(int newState)
{
  if(newState == Qt::Checked)
  {
    startdataEd->setEnabled(true);
    startdataLb->setEnabled(true);
    stopdataEd->setEnabled(true);
    stopdataLb->setEnabled(true);
  }
  else if(newState == Qt::Unchecked)
  {
    startdataEd->setEnabled(false);
    startdataLb->setEnabled(false);
    stopdataEd->setEnabled(false);
    stopdataLb->setEnabled(false);
  }
  emit datarange_modified();
}


void FitSettingsWidget::datarange_modified_slot()
{
  emit datarange_modified();
}


void FitSettingsWidget::bin_modified_slot(int newsize)
{
  emit bin_modified(newsize);
}

void FitSettingsWidget::svd_modified_slot()
{
  emit svd_modified();
}

void FitSettingsWidget::inv_method_modified_slot(int newindex)
{
  svdEd->setEnabled(false);
  svdLb->setEnabled(false);
  svdRatioEd->setEnabled(false);
  svdRatioLb->setEnabled(false);
  svdValueEd->setEnabled(false);
  svdValueLb->setEnabled(false);
  if(newindex==1)
  {
    svdEd->setEnabled(true);
    svdLb->setEnabled(true);
  }
  if(newindex==2)
  {
    svdRatioEd->setEnabled(true);
    svdRatioLb->setEnabled(true);
  }
  if(newindex==3)
  {
    svdValueEd->setEnabled(true);
    svdValueLb->setEnabled(true);
  }
  emit svd_modified();
}


void FitSettingsWidget::bootstrap_normalization_modified_slot(int newindex)
{
  if(newindex==1)
  {
    binningSB->setValue(1);
    binningSB->setEnabled(false);
  }
  else
  {
    binningSB->setEnabled(true);
  }
  emit bootstrap_normalization_modified();
}


void FitSettingsWidget::select_bootfile()
{
  QString fileName=QFileDialog::getOpenFileName(this, tr("Select bootstrap ensemble file"),
                                                 QString(""),
                                                 tr("Bootstrap ensemble files (*.bse);; All files (*)"));
  if(!fileName.isEmpty())
  {
    bootfileEd->setText(fileName);
  }
}


void FitSettingsWidget::new_bootfile()
{
  new_boot_file_dialog nbfd(this);
  if(nbfd.exec()==QDialog::Accepted)
  {
    QString fileName=QFileDialog::getSaveFileName(this, tr("Save new bootstrap ensemble file as"),
                                                   QString(""),
                                                   tr("Bootstrap ensemble files (*.bse);; All files (*)"));
    if(!fileName.isEmpty())
    {

      ofstream output((fileName.toStdString()).c_str());
      if(!output)
      {
        QMessageBox::warning(this, tr("QMBF"),
                             tr("Cannot write file %1:\n")
                             .arg(fileName));
        return;
      }

      QApplication::setOverrideCursor(Qt::WaitCursor);

      int bssamples=nbfd.get_n_boot_samples();
      int ndatasets=nbfd.get_n_data_sets();

      output << bssamples << endl;
      output << ndatasets << endl;

      gsl_rng_default_seed=0;
      gsl_rng_env_setup();
      const gsl_rng_type* Trng=gsl_rng_mt19937;
      gsl_rng* rng=gsl_rng_alloc(Trng);

      for(int boot=0; boot<bssamples; ++boot)
      {
        for(int b=0; b<ndatasets; ++b)
        {
          int r=static_cast<int>(gsl_rng_uniform(rng)*ndatasets)+1;
          output << r << endl;
        }
      }

      output.close();

      QApplication::restoreOverrideCursor();

      bootfileEd->setText(fileName);
    }
  }
}


void FitSettingsWidget::createGadgets()
{
  bayes_cb=new QCheckBox(tr("Activate Bayesian constraints"), this);
  connect(bayes_cb, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));
  connect(bayes_cb, SIGNAL(stateChanged(int)), this, SLOT(bayesianmodified_slot(int)));

  boot_prior_cb=new QCheckBox(tr("Use Gaussian random priors for bootstrap / multifit"), this);
  connect(boot_prior_cb, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));

  use_bse_cb=new QCheckBox(tr("Use bootstrap ensemble file"), this);
  connect(use_bse_cb, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));
  connect(use_bse_cb, SIGNAL(stateChanged(int)), this, SLOT(usebsemodified_slot(int)));

  restrict_data_cb=new QCheckBox(tr("Restrict range of data sets"), this);
  connect(restrict_data_cb, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));
  connect(restrict_data_cb, SIGNAL(stateChanged(int)), this, SLOT(restrictdata_modified_slot(int)));

  num_diff_cb=new QCheckBox(tr("Use numerical differentiation for first derivatives   "), this);
  connect(num_diff_cb, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));
  connect(num_diff_cb, SIGNAL(stateChanged(int)), this, SLOT(numdiffmodified_slot(int)));

  second_deriv_covariance_cb=new QCheckBox(tr("Use second derivatives for parameter covariance matrix"), this);
  connect(second_deriv_covariance_cb, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));
  connect(second_deriv_covariance_cb, SIGNAL(stateChanged(int)), this, SLOT(secondderivcovariancemodified_slot(int)));

  second_deriv_minimization_cb=new QCheckBox(tr("Use second derivatives for minimization"), this);
  connect(second_deriv_minimization_cb, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));
  connect(second_deriv_minimization_cb, SIGNAL(stateChanged(int)), this, SLOT(secondderivminimizationmodified_slot(int)));

  numdiffEd=new QLineEdit();
  numdiffEd->setValidator(new QDoubleValidator(1E-99, 1E99, 15, numdiffEd));
  numdiffLb=new QLabel("Numerical differentiation step size:");
  connect(numdiffEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));

  startdataEd=new QLineEdit();
  startdataEd->setValidator(new QIntValidator(1, 2000000000, startdataEd));
  startdataLb=new QLabel("Data range min:");
  connect(startdataEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));
  connect(startdataEd, SIGNAL(textChanged(const QString&)), this, SLOT(datarange_modified_slot()));

  stopdataEd=new QLineEdit();
  stopdataEd->setValidator(new QIntValidator(10, 2000000000, stopdataEd));
  stopdataLb=new QLabel("Data range max:");
  connect(stopdataEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));
  connect(stopdataEd, SIGNAL(textChanged(const QString&)), this, SLOT(datarange_modified_slot()));

  startlambdaEd=new QLineEdit();
  startlambdaEd->setValidator(new QDoubleValidator(1E-99, 1E99, 15, startlambdaEd));
  startlambdaLb=new QLabel("Start lambda:");
  connect(startlambdaEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));

  lambdafacEd=new QLineEdit();
  lambdafacEd->setValidator(new QDoubleValidator(1E-99, 1E99, 15, lambdafacEd));
  lambdafacLb=new QLabel("Lambda factor:");
  connect(lambdafacEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));

  toleranceEd=new QLineEdit();
  toleranceEd->setValidator(new QDoubleValidator(1E-99, 1E99, 15, toleranceEd));
  toleranceLb=new QLabel("Delta(chi^2) tolerance:");
  connect(toleranceEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));

  invMethodLb=new QLabel("(Pseudo-)Inversion method for correlation matrix:");
  invMethodCb=new QComboBox;
  invMethodCb->addItem("LU decomposition");
  invMethodCb->addItem("SVD with fixed cut");
  invMethodCb->addItem("SVD with EV ratio cut");
  invMethodCb->addItem("SVD with EV value cut");
  invMethodCb->addItem("Diagonal only (uncorrelated fit)");
  connect(invMethodCb, SIGNAL(currentIndexChanged(int)), this, SLOT(modified_slot()));
  connect(invMethodCb, SIGNAL(currentIndexChanged(int)), this, SLOT(inv_method_modified_slot(int)));

  bootstrapNormalizationLb=new QLabel("Normalization of correlation matrix:");
  bootstrapNormalizationCb=new QComboBox;
  bootstrapNormalizationCb->addItem("1/(N*(N-1))");
  bootstrapNormalizationCb->addItem("1/(N-1)");
  connect(bootstrapNormalizationCb, SIGNAL(currentIndexChanged(int)), this, SLOT(modified_slot()));
  connect(bootstrapNormalizationCb, SIGNAL(currentIndexChanged(int)), this, SLOT(bootstrap_normalization_modified_slot(int)));


  svdEd=new QLineEdit();
  svdEd->setValidator(new QIntValidator(0, 2000000000, svdEd));
  svdLb=new QLabel("Fixed SVD cut:");
  connect(svdEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));
  connect(svdEd, SIGNAL(textChanged(const QString&)), this, SLOT(svd_modified_slot()));

  svdRatioEd=new QLineEdit();
  svdRatioEd->setValidator(new QDoubleValidator(1E-99, 1E99, 15, svdRatioEd));
  svdRatioLb=new QLabel("SVD EV ratio cut:");
  connect(svdRatioEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));
  connect(svdRatioEd, SIGNAL(textChanged(const QString&)), this, SLOT(svd_modified_slot()));

  svdValueEd=new QLineEdit();
  svdValueEd->setValidator(new QDoubleValidator(1E-99, 1E99, 15, svdValueEd));
  svdValueLb=new QLabel("SVD EV value cut:");
  connect(svdValueEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));
  connect(svdValueEd, SIGNAL(textChanged(const QString&)), this, SLOT(svd_modified_slot()));

  stepsEd=new QLineEdit();
  stepsEd->setValidator(new QIntValidator(1, 2000000000, stepsEd));
  stepsLb=new QLabel("Maximum number of iterations:");
  connect(stepsEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));

  binningSB=new QSpinBox();
  binningSB->setMinimum(1);
  binningSB->setMaximum(2000000000);
  binningLb=new QLabel("Bin size:");
  connect(binningSB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));
  connect(binningSB, SIGNAL(valueChanged(int)), this, SLOT(bin_modified_slot(int)));

  bssamplesEd=new QLineEdit();
  bssamplesEd->setValidator(new QIntValidator(3, 2000000000, bssamplesEd));
  bssamplesLb=new QLabel("Number of bootstrap samples:");
  connect(bssamplesEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));

  bootfileLb=new QLabel("Bootstrap ensemble file:");
  bootfileEd=new QLineEdit();
  connect(bootfileEd, SIGNAL(textChanged(const QString&)), this, SLOT(modified_slot()));

  bootfilebutton=new QToolButton();
  bootfilebutton->setIcon(QPixmap(":/images/fileopen.png"));
  connect(bootfilebutton, SIGNAL(clicked()), this, SLOT(select_bootfile()));

  newbootfilebutton=new QToolButton();
  newbootfilebutton->setIcon(QPixmap(":/images/filenew.png"));
  connect(newbootfilebutton, SIGNAL(clicked()), this, SLOT(new_bootfile()));

  layout=new QGridLayout;

  layout->addWidget(bayes_cb, 0, 0);

  layout->addWidget(boot_prior_cb, 0, 1);

  layout->addWidget(restrict_data_cb, 3, 0);

  hlayout1=new QHBoxLayout();

  hlayout1->addWidget(startdataLb);
  hlayout1->addWidget(startdataEd);
  hlayout1->addWidget(stopdataLb);
  hlayout1->addWidget(stopdataEd);

  layout->addLayout(hlayout1, 3, 1);

  layout->addWidget(num_diff_cb, 4, 0);
  hlayout3=new QHBoxLayout();
  hlayout3->addWidget(second_deriv_covariance_cb);
  hlayout3->addWidget(second_deriv_minimization_cb);
  layout->addLayout(hlayout3, 4, 1);

  layout->addWidget(numdiffLb, 5, 0);
  layout->addWidget(numdiffEd, 5, 1);

  layout->addWidget(startlambdaLb, 6, 0);
  layout->addWidget(startlambdaEd, 6, 1);

  layout->addWidget(lambdafacLb, 7, 0);
  layout->addWidget(lambdafacEd, 7, 1);

  layout->addWidget(toleranceLb, 8, 0);
  layout->addWidget(toleranceEd, 8, 1);

  layout->addWidget(bootstrapNormalizationLb, 9, 0);
  layout->addWidget(bootstrapNormalizationCb, 9, 1);

  layout->addWidget(invMethodLb, 10, 0);
  layout->addWidget(invMethodCb, 10, 1);

  layout->addWidget(svdLb, 11, 0);
  layout->addWidget(svdEd, 11, 1);

  layout->addWidget(svdRatioLb, 12, 0);
  layout->addWidget(svdRatioEd, 12, 1);

  layout->addWidget(svdValueLb, 13, 0);
  layout->addWidget(svdValueEd, 13, 1);

  layout->addWidget(stepsLb, 14, 0);
  layout->addWidget(stepsEd, 14, 1);

  layout->addWidget(binningLb, 15, 0);
  layout->addWidget(binningSB, 15, 1);

  layout->addWidget(bssamplesLb, 16, 0);
  layout->addWidget(bssamplesEd, 16, 1);

  layout->addWidget(use_bse_cb, 17, 0);

  hlayout2=new QHBoxLayout();

  hlayout2->addWidget(bootfileLb);
  hlayout2->addWidget(bootfileEd);
  hlayout2->addWidget(bootfilebutton);
  hlayout2->addWidget(newbootfilebutton);

  layout->addLayout(hlayout2, 17, 1);

  setLayout(layout);
}
