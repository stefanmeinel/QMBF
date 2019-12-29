#include "plotsettingsdialog.h"

plot_settings_dialog::plot_settings_dialog(int n_variables, int max_n_variables, QWidget* parent)
{
  setWindowTitle(tr("Plot Options"));
  max_variables=max_n_variables;
  n_vars=n_variables;
  plot_var=0;
  createGadgets();
  this->resize(400, 500);
  geometry=this->saveGeometry();
}

void plot_settings_dialog::restore_geometry()
{
  this->restoreGeometry(geometry);
}

void plot_settings_dialog::hideEvent(QHideEvent* event)
{
  geometry=this->saveGeometry();
  emit hide();
  QWidget::hideEvent(event);
}

void plot_settings_dialog::updateSizes()
{
  int scrollBarWidth=0;
  if(variablevalues_tableView->verticalScrollBar()->isVisible())
  {
    scrollBarWidth=variablevalues_tableView->verticalScrollBar()->width();
  }
  variablevalues_tableView->setColumnWidth(0, variablevalues_tableView->width()- variablevalues_tableView->verticalHeader()->width()-scrollBarWidth-table_padding);
}

void plot_settings_dialog::resizeEvent(QResizeEvent* event)
{
  updateSizes();
  QWidget::resizeEvent(event);
}


void plot_settings_dialog::set_n_variables(int new_n_variables)
{
  if(new_n_variables>n_vars)
  {
    for(int v=n_vars; v<=new_n_variables-1; ++v)
    {
      variablevalues_tableView->setRowHidden(v, false);
    }
  }
  else if(new_n_variables<n_vars)
  {
    for(int v=n_vars-1; v>new_n_variables-1; --v)
    {
      variablevalues_tableView->setRowHidden(v, true);
    }
  }
  n_vars=new_n_variables;
  plot_var_SB->setMaximum(n_vars);
  updateSizes();
}

int plot_settings_dialog::get_plot_var()
{
  return plot_var;
}

void plot_settings_dialog::set_plot_var(int v)
{
  plot_var_modified_slot(v+1);
}


void plot_settings_dialog::plot_var_modified_slot(int new_plot_var)
{
  variablevalues_tableView->setRowHidden(plot_var,    false);
  variablevalues_tableView->setRowHidden(new_plot_var-1, true);
  plot_var=new_plot_var-1;
}

double plot_settings_dialog::get_variable_value(int v)
{
  if((v>n_vars-1))
  {
    return 0.0;
  }
  QModelIndex index = variablevalues_model->index(v, 0, QModelIndex());
  return QString_to_double((variablevalues_model->data(index, Qt::EditRole)).toString());
}


void plot_settings_dialog::set_variable_value(int v, double value)
{
  if((v>n_vars-1))
  {
    return;
  }
  QModelIndex index = variablevalues_model->index(v, 0, QModelIndex());
  variablevalues_model->setData(index, QVariant(double_to_QString(value)), Qt::EditRole);
}


std::string plot_settings_dialog::get_grace()
{
  return (grace_LE->text()).toStdString();
}

void plot_settings_dialog::set_grace(std::string name)
{
  grace_LE->setText(QString(name.c_str()));
}

int plot_settings_dialog::get_viewplot()
{
  return viewplotCb->currentIndex();
}

void plot_settings_dialog::set_viewplot(int index)
{
  viewplotCb->setCurrentIndex(index);
}

std::string plot_settings_dialog::get_eff_mass()
{
  return (eff_mass_LE->text()).toStdString();
}

void plot_settings_dialog::set_eff_mass(std::string name)
{
  eff_mass_LE->setText(QString(name.c_str()));
}

double plot_settings_dialog::get_min()
{
  return QString_to_double(plot_min_LE->text());
}

void plot_settings_dialog::set_min(double val)
{
  plot_min_LE->setText(double_to_QString(val));
}

double plot_settings_dialog::get_max()
{
  return QString_to_double(plot_max_LE->text());
}

void plot_settings_dialog::set_max(double val)
{
  plot_max_LE->setText(double_to_QString(val));
}




double plot_settings_dialog::get_min_y()
{
  return QString_to_double(plot_min_y_LE->text());
}

void plot_settings_dialog::set_min_y(double val)
{
  plot_min_y_LE->setText(double_to_QString(val));
}

double plot_settings_dialog::get_max_y()
{
  return QString_to_double(plot_max_y_LE->text());
}

void plot_settings_dialog::set_max_y(double val)
{
  plot_max_y_LE->setText(double_to_QString(val));
}



double plot_settings_dialog::get_min_em_y()
{
  return QString_to_double(plot_min_em_y_LE->text());
}

void plot_settings_dialog::set_min_em_y(double val)
{
  plot_min_em_y_LE->setText(double_to_QString(val));
}

double plot_settings_dialog::get_max_em_y()
{
  return QString_to_double(plot_max_em_y_LE->text());
}

void plot_settings_dialog::set_max_em_y(double val)
{
  plot_max_em_y_LE->setText(double_to_QString(val));
}



bool plot_settings_dialog::continuous()
{
  return !(continuous_CB->isChecked());
}


void plot_settings_dialog::set_continuous(bool val)
{
  continuous_CB->setChecked(!val);
}


bool plot_settings_dialog::data_range()
{
  return (data_range_CB->isChecked());
}


void plot_settings_dialog::set_data_range(bool val)
{
  data_range_CB->setChecked(val);
}

bool plot_settings_dialog::data_range_y()
{
  return (data_range_y_CB->isChecked());
}


void plot_settings_dialog::set_data_range_y(bool val)
{
  data_range_y_CB->setChecked(val);
}


bool plot_settings_dialog::get_log()
{
  return log_CB->isChecked();
}

void plot_settings_dialog::set_log(bool val)
{
  log_CB->setChecked(val);
}


bool plot_settings_dialog::get_neg()
{
  return neg_CB->isChecked();
}

void plot_settings_dialog::set_neg(bool val)
{
  neg_CB->setChecked(val);
}


void plot_settings_dialog::reset()
{
  plot_min_LE->setText("0");
  plot_max_LE->setText("100");
  continuous_CB->setChecked(false);
  data_range_CB->setChecked(true);
  log_CB->setChecked(true);
  neg_CB->setChecked(false);
  viewplotCb->setCurrentIndex(0);
  grace_LE->setText("xmgrace");
  plot_var_SB->setValue(1);
  plot_var=0;
  for(int row = 0; row < max_variables; ++row)
  {
    QModelIndex index = variablevalues_model->index(row, 0, QModelIndex());
    variablevalues_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  variablevalues_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int v=n_vars; v<max_variables; ++v)
  {
    variablevalues_tableView->setRowHidden(v, true);
  }
  variablevalues_tableView->setRowHidden(plot_var, true);
}


void plot_settings_dialog::data_range_CB_changed(int newstate)
{
  bool enabled=false;
  if(newstate==Qt::Checked) enabled=false;
  if(newstate==Qt::Unchecked) enabled=true;
  plot_min_LB->setEnabled(enabled);
  plot_min_LE->setEnabled(enabled);
  plot_max_LB->setEnabled(enabled);
  plot_max_LE->setEnabled(enabled);
}


void plot_settings_dialog::data_range_y_CB_changed(int newstate)
{
  bool enabled=false;
  if(newstate==Qt::Checked) enabled=false;
  if(newstate==Qt::Unchecked) enabled=true;
  plot_min_y_LB->setEnabled(enabled);
  plot_min_y_LE->setEnabled(enabled);
  plot_max_y_LB->setEnabled(enabled);
  plot_max_y_LE->setEnabled(enabled);
  plot_min_em_y_LB->setEnabled(enabled);
  plot_min_em_y_LE->setEnabled(enabled);
  plot_max_em_y_LB->setEnabled(enabled);
  plot_max_em_y_LE->setEnabled(enabled);
}

void plot_settings_dialog::modified_slot()
{
  emit modified();
}


void plot_settings_dialog::createGadgets()
{
  buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  plot_var_SB=new QSpinBox();
  plot_var_SB->setMinimumWidth(60);
  plot_var_SB->setMinimum(1);
  plot_var_SB->setValue(plot_var+1);
  plot_var_SB->setMaximum(n_vars);
  connect(plot_var_SB, SIGNAL(valueChanged(int)), this, SLOT(plot_var_modified_slot(int)));
  connect(plot_var_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));
  plot_var_Lb=new QLabel("Variable on horizontal axis:");

  hlayout1=new QHBoxLayout;

  hlayout1->addWidget(plot_var_Lb, Qt::AlignLeft);
  hlayout1->addWidget(plot_var_SB, 1, Qt::AlignRight);

  variablevalues_tableView=new QTableView();
  variablevalues_tableView->setMinimumHeight(100);
  variablevalues_model=new QStandardItemModel(max_variables, 1);
  variablevalues_model->setHeaderData(0, Qt::Horizontal, "Fixed value for plotting");
  variablevalues_tableView->setModel(variablevalues_model);
  variablevalues_delegate = new LineEditDelegate(false, true);
  variablevalues_tableView->setItemDelegate(variablevalues_delegate);
  for(int row = 0; row < max_variables; ++row)
  {
    QModelIndex index = variablevalues_model->index(row, 0, QModelIndex());
    variablevalues_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  variablevalues_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int v=n_vars; v<max_variables; ++v)
  {
    variablevalues_tableView->setRowHidden(v, true);
  }
  variablevalues_tableView->setRowHidden(plot_var, true);
  connect(variablevalues_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));
  vv_Lb=new QLabel("Enter fixed values for the other variables below:");

  grace_LB=new QLabel("Grace command:");
  grace_LE=new QLineEdit();
  grace_LE->setText("xmgrace");
  connect(grace_LE, SIGNAL(textChanged(const QString &)), this, SLOT(modified_slot())); 

  plot_min_LB=new QLabel("x-axis min:");
  plot_min_LE=new QLineEdit();
  plot_min_LE->setText("0");
  plot_min_LE->setValidator(new QDoubleValidator(-1E99, 1E99, 15, plot_min_LE));
  connect(plot_min_LE, SIGNAL(textChanged(const QString &)), this, SLOT(modified_slot())); 

  plot_max_LB=new QLabel("x-axis max:");
  plot_max_LE=new QLineEdit();
  plot_max_LE->setText("100");
  plot_max_LE->setValidator(new QDoubleValidator(-1E99, 1E99, 15, plot_max_LE)); 
  connect(plot_max_LE, SIGNAL(textChanged(const QString &)), this, SLOT(modified_slot())); 

  plot_min_y_LB=new QLabel("y-axis min:");
  plot_min_y_LE=new QLineEdit();
  plot_min_y_LE->setText("0.0000001");
  plot_min_y_LE->setValidator(new QDoubleValidator(-1E99, 1E99, 15, plot_min_y_LE));
  connect(plot_min_y_LE, SIGNAL(textChanged(const QString &)), this, SLOT(modified_slot())); 

  plot_max_y_LB=new QLabel("y-axis max:");
  plot_max_y_LE=new QLineEdit();
  plot_max_y_LE->setText("1");
  plot_max_y_LE->setValidator(new QDoubleValidator(-1E99, 1E99, 15, plot_max_y_LE)); 
  connect(plot_max_y_LE, SIGNAL(textChanged(const QString &)), this, SLOT(modified_slot())); 

  plot_min_em_y_LB=new QLabel("y-axis min (effective mass plot):");
  plot_min_em_y_LE=new QLineEdit();
  plot_min_em_y_LE->setText("0");
  plot_min_em_y_LE->setValidator(new QDoubleValidator(-1E99, 1E99, 15, plot_min_em_y_LE));
  connect(plot_min_em_y_LE, SIGNAL(textChanged(const QString &)), this, SLOT(modified_slot())); 

  plot_max_em_y_LB=new QLabel("y-axis max (effective mass plot):");
  plot_max_em_y_LE=new QLineEdit();
  plot_max_em_y_LE->setText("1");
  plot_max_em_y_LE->setValidator(new QDoubleValidator(-1E99, 1E99, 15, plot_max_em_y_LE)); 
  connect(plot_max_em_y_LE, SIGNAL(textChanged(const QString &)), this, SLOT(modified_slot())); 

  eff_mass_LB=new QLabel("Formula for effective mass parameter (e.g. exp(E) ) :");
  eff_mass_LE=new QLineEdit();
  eff_mass_LE->setText("");
  connect(eff_mass_LE, SIGNAL(textChanged(const QString &)), this, SLOT(modified_slot())); 

  gridlayout1=new QGridLayout();
  gridlayout1->addWidget(grace_LB, 0, 0);
  gridlayout1->addWidget(grace_LE, 0, 1);

  viewplotLb=new QLabel("View plot:");
  viewplotCb=new QComboBox;
  viewplotCb->addItem("in QMBF window (generate SVG with Grace)");
  viewplotCb->addItem("directly in Grace");
  viewplotCb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  connect(viewplotCb, SIGNAL(currentIndexChanged(int)), this, SLOT(modified_slot()));

  gridlayout1->addWidget(viewplotLb, 1, 0);
  gridlayout1->addWidget(viewplotCb, 1, 1);



  gridlayout2=new QGridLayout();
  gridlayout2->addWidget(plot_min_LB, 0, 0);
  gridlayout2->addWidget(plot_min_LE, 0, 1);
  gridlayout2->addWidget(plot_max_LB, 1, 0);
  gridlayout2->addWidget(plot_max_LE, 1, 1);

  gridlayout3=new QGridLayout();
  gridlayout3->addWidget(plot_min_y_LB, 0, 0);
  gridlayout3->addWidget(plot_min_y_LE, 0, 1);
  gridlayout3->addWidget(plot_max_y_LB, 1, 0);
  gridlayout3->addWidget(plot_max_y_LE, 1, 1);
  gridlayout3->addWidget(plot_min_em_y_LB, 2, 0);
  gridlayout3->addWidget(plot_min_em_y_LE, 2, 1);
  gridlayout3->addWidget(plot_max_em_y_LB, 3, 0);
  gridlayout3->addWidget(plot_max_em_y_LE, 3, 1);

  gridlayout4=new QGridLayout();
  gridlayout4->addWidget(eff_mass_LB, 0, 0);
  gridlayout4->addWidget(eff_mass_LE, 1, 0);

  continuous_CB=new QCheckBox(tr("Evaluate model functions only at data points"));
  continuous_CB->setChecked(false);
  connect(continuous_CB, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));


  data_range_CB=new QCheckBox(tr("Adjust x-axis plot range to data points"));
  connect(data_range_CB, SIGNAL(stateChanged(int)), this, SLOT(data_range_CB_changed(int)));
  connect(data_range_CB, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));
  data_range_CB->setChecked(true);

  data_range_y_CB=new QCheckBox(tr("Adjust y-axis plot range to data points"));
  connect(data_range_y_CB, SIGNAL(stateChanged(int)), this, SLOT(data_range_y_CB_changed(int)));
  connect(data_range_y_CB, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));
  data_range_y_CB->setChecked(true);

  log_CB=new QCheckBox(tr("Logarithmic y-axis"));
  log_CB->setChecked(true);
  connect(log_CB, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));

  neg_CB=new QCheckBox(tr("Multiply by (-1)"));
  neg_CB->setChecked(true);
  connect(neg_CB, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));

  vlayout=new QVBoxLayout();

  vlayout->addLayout(hlayout1);
  vlayout->addWidget(vv_Lb);
  vlayout->addWidget(variablevalues_tableView);
  spacer0=new QSpacerItem(10,10);
  vlayout->addItem(spacer0);
  vlayout->addLayout(gridlayout1);
  spacer1=new QSpacerItem(10,10);
  vlayout->addItem(spacer1);
  vlayout->addWidget(data_range_CB);
  vlayout->addLayout(gridlayout2);
  spacer2=new QSpacerItem(10,10);
  vlayout->addItem(spacer2);
  vlayout->addWidget(data_range_y_CB);
  vlayout->addLayout(gridlayout3);
  spacer3=new QSpacerItem(10,10);
  vlayout->addItem(spacer3);
  vlayout->addLayout(gridlayout4);
  spacer4=new QSpacerItem(10,10);
  vlayout->addItem(spacer4);
  vlayout->addWidget(continuous_CB);
  vlayout->addWidget(log_CB);
  vlayout->addWidget(neg_CB);
  vlayout->addWidget(buttonBox);

  setLayout(vlayout);
}
