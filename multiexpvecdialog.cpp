#include "multiexpvecdialog.h"

multi_exp_vec_dialog::multi_exp_vec_dialog(int n_exp, int max_n_exp, int n_vec, int max_n_vec, QWidget* parent)
{
  start_n_exp=n_exp;
  max_n_exps=max_n_exp;
  start_n_vec=n_vec;
  max_n_vec_dim=max_n_vec;

  setWindowTitle(tr("Create Multiple Exponentials Vector Fit Model"));

  createGadgets();
}

void multi_exp_vec_dialog::set_text(QString text)
{
  text_LB->setText(text);
}

bool multi_exp_vec_dialog::get_BC()
{
  return BC_CB->isChecked();
}

void multi_exp_vec_dialog::set_BC(bool val)
{
  BC_CB->setChecked(val);
}

bool multi_exp_vec_dialog::get_constant()
{
  return constant_CB->isChecked();
}

void multi_exp_vec_dialog::set_constant(bool val)
{
  constant_CB->setChecked(val);
}

void multi_exp_vec_dialog::set_BC_enabled(bool val)
{
  BC_CB->setEnabled(val);
}

int multi_exp_vec_dialog::get_n_exp()
{
  return n_exp_SB->value();
}

void multi_exp_vec_dialog::set_n_exp(int val)
{
  n_exp_SB->setValue(val);
}

int multi_exp_vec_dialog::get_n_vec()
{
  return n_vec_SB->value();
}

void multi_exp_vec_dialog::set_n_vec(int val)
{
  n_vec_SB->setValue(val);
}


void multi_exp_vec_dialog::reset()
{
  set_n_exp(start_n_exp);
  set_n_vec(start_n_vec);
  set_BC(false);
  set_BC_enabled(true);
}


void multi_exp_vec_dialog::modified_slot()
{
  emit modified();
}


void multi_exp_vec_dialog::createGadgets()
{

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  text_LB=new QLabel("Multiple Exponentials, Vector/Matrix Fit");

  n_exp_LB=new QLabel("Number of exponentials:");
  n_exp_SB=new QSpinBox();
  n_exp_SB->setMinimum(1);
  n_exp_SB->setValue(start_n_exp);
  n_exp_SB->setMaximum(max_n_exps);
  connect(n_exp_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  n_vec_LB=new QLabel("Dimension:");
  n_vec_SB=new QSpinBox();
  n_vec_SB->setMinimum(1);
  n_vec_SB->setValue(start_n_vec);
  n_vec_SB->setMaximum(max_n_vec_dim);
  connect(n_vec_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  BC_CB=new QCheckBox("Periodic boundary conditions");
  BC_CB->setChecked(false);
  connect(BC_CB, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));

  constant_CB=new QCheckBox("Time-independent contribution");
  constant_CB->setChecked(false);
  connect(constant_CB, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));

  layout=new QGridLayout();

  layout->addWidget(BC_CB, 0, 0);
  layout->addWidget(constant_CB, 1, 0);

  layout->addWidget(n_vec_LB, 2, 0);
  layout->addWidget(n_vec_SB, 2, 1);

  layout->addWidget(n_exp_LB, 3, 0);
  layout->addWidget(n_exp_SB, 3, 1);

  vlayout=new QVBoxLayout();

  vlayout->addWidget(text_LB);
  vlayout->addLayout(layout);
  vlayout->addWidget(buttonBox);

  setLayout(vlayout);
}
