#include "multiexpmatupperdialog.h"

multi_exp_mat_upper_dialog::multi_exp_mat_upper_dialog(int n_exp, int max_n_exp, int dim, int max_dim, QWidget* parent)
{
  start_n_exp=n_exp;
  max_n_exps=max_n_exp;
  start_dim=dim;
  maxdim=max_dim;

  setWindowTitle(tr("Create Multiple Exponentials Triangualar Matrix Fit Model"));

  createGadgets();
}

void multi_exp_mat_upper_dialog::set_text(QString text)
{
  text_LB->setText(text);
}

int multi_exp_mat_upper_dialog::get_n_exp()
{
  return n_exp_SB->value();
}

void multi_exp_mat_upper_dialog::set_n_exp(int val)
{
  n_exp_SB->setValue(val);
}

int multi_exp_mat_upper_dialog::get_dim()
{
  return dim_SB->value();
}

void multi_exp_mat_upper_dialog::set_dim(int val)
{
  dim_SB->setValue(val);
}


void multi_exp_mat_upper_dialog::reset()
{
  set_n_exp(start_n_exp);
  set_dim(start_dim);
}


void multi_exp_mat_upper_dialog::modified_slot()
{
  emit modified();
}


void multi_exp_mat_upper_dialog::createGadgets()
{

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  text_LB=new QLabel("Multiple Exponentials, Triangular Matrix Fit");

  n_exp_LB=new QLabel("Number of exponentials:");
  n_exp_SB=new QSpinBox();
  n_exp_SB->setMinimum(1);
  n_exp_SB->setValue(start_n_exp);
  n_exp_SB->setMaximum(max_n_exps);
  connect(n_exp_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  dim_LB=new QLabel("Dimension:");
  dim_SB=new QSpinBox();
  dim_SB->setMinimum(1);
  dim_SB->setValue(start_dim);
  dim_SB->setMaximum(maxdim);
  connect(dim_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  layout=new QGridLayout();

  layout->addWidget(dim_LB, 0, 0);
  layout->addWidget(dim_SB, 0, 1);

  layout->addWidget(n_exp_LB, 1, 0);
  layout->addWidget(n_exp_SB, 1, 1);

  vlayout=new QVBoxLayout();

  vlayout->addWidget(text_LB);
  vlayout->addLayout(layout);
  vlayout->addWidget(buttonBox);

  setLayout(vlayout);
}
