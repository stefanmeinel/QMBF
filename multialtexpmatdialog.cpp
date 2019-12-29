#include "multialtexpmatdialog.h"

multi_alt_exp_mat_dialog::multi_alt_exp_mat_dialog(int n_exp, int max_n_exp, int m_exp, int max_m_exp, int dim_1, int dim_2, int max_dim, QWidget* parent)
{
  start_n_exp=n_exp;
  max_n_exps=max_n_exp;
  start_m_exp=m_exp;
  max_m_exps=max_m_exp;
  start_dim_1=dim_1;
  start_dim_2=dim_2;
  maxdim=max_dim;

  setWindowTitle(tr("Create Multiple (inc. oscillating) Exponentials Matrix Fit Model"));

  createGadgets();
}

void multi_alt_exp_mat_dialog::set_text(QString text)
{
  text_LB->setText(text);
}


int multi_alt_exp_mat_dialog::get_dim_1()
{
  return dim_1_SB->value();
}

void multi_alt_exp_mat_dialog::set_min_dim_1(int val)
{
  dim_1_SB->setMinimum(val);
}

void multi_alt_exp_mat_dialog::set_dim_1(int val)
{
  dim_1_SB->setValue(val);
}

int multi_alt_exp_mat_dialog::get_dim_2()
{
  return dim_2_SB->value();
}

void multi_alt_exp_mat_dialog::set_dim_2(int val)
{
  dim_2_SB->setValue(val);
}

int multi_alt_exp_mat_dialog::get_n_exp()
{
  return n_exp_SB->value();
}

void multi_alt_exp_mat_dialog::set_n_exp(int val)
{
  n_exp_SB->setValue(val);
}

int multi_alt_exp_mat_dialog::get_m_exp()
{
  return m_exp_SB->value();
}

void multi_alt_exp_mat_dialog::set_m_exp(int val)
{
  m_exp_SB->setValue(val);
}

void multi_alt_exp_mat_dialog::reset()
{
  set_n_exp(start_n_exp);
  set_m_exp(start_m_exp);
  set_dim_1(start_dim_1);
  set_dim_2(start_dim_2);
}

void multi_alt_exp_mat_dialog::modified_slot()
{
  emit modified();
}

void multi_alt_exp_mat_dialog::createGadgets()
{
  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  text_LB=new QLabel("Multiple (incl. oscillating) Exponentials, Vector/Matrix Fit");

  n_exp_LB=new QLabel("Number of standard exponentials:");
  n_exp_SB=new QSpinBox();
  n_exp_SB->setMinimum(1);
  n_exp_SB->setValue(start_n_exp);
  n_exp_SB->setMaximum(max_n_exps);
  connect(n_exp_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  m_exp_LB=new QLabel("Number of oscillating exponentials:");
  m_exp_SB=new QSpinBox();
  m_exp_SB->setMinimum(1);
  m_exp_SB->setValue(start_m_exp);
  m_exp_SB->setMaximum(max_m_exps);
  connect(m_exp_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  dim_1_LB=new QLabel("Dimension 1:");
  dim_1_SB=new QSpinBox();
  dim_1_SB->setMinimum(1);
  dim_1_SB->setValue(start_dim_1);
  dim_1_SB->setMaximum(maxdim);
  connect(dim_1_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  dim_2_LB=new QLabel("Dimension 2:");
  dim_2_SB=new QSpinBox();
  dim_2_SB->setMinimum(1);
  dim_2_SB->setValue(start_dim_2);
  dim_2_SB->setMaximum(maxdim);
  connect(dim_2_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  layout=new QGridLayout();

  layout->addWidget(dim_1_LB, 0, 0);
  layout->addWidget(dim_1_SB, 0, 1);

  layout->addWidget(dim_2_LB, 1, 0);
  layout->addWidget(dim_2_SB, 1, 1);

  layout->addWidget(n_exp_LB, 2, 0);
  layout->addWidget(n_exp_SB, 2, 1);

  layout->addWidget(m_exp_LB, 3, 0);
  layout->addWidget(m_exp_SB, 3, 1);


  vlayout=new QVBoxLayout();

  vlayout->addWidget(text_LB);

  vlayout->addLayout(layout);
  vlayout->addWidget(buttonBox);

  setLayout(vlayout);
}
