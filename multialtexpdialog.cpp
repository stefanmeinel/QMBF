#include "multialtexpdialog.h"

multi_alt_exp_dialog::multi_alt_exp_dialog(int n_exp, int max_n_exp, int m_exp, int max_m_exp, QWidget* parent)
{
  start_n_exp=n_exp;
  max_n_exps=max_n_exp;
  start_m_exp=m_exp;
  max_m_exps=max_m_exp;

  setWindowTitle(tr("Create Multiple (incl. oscillating) Exponentials Model"));

  createGadgets();
}

void multi_alt_exp_dialog::set_text(QString text)
{
  text_LB->setText(text);
}

bool multi_alt_exp_dialog::get_BC()
{
  return BC_CB->isChecked();
}

void multi_alt_exp_dialog::set_BC(bool val)
{
  BC_CB->setChecked(val);
}

bool multi_alt_exp_dialog::get_constant()
{
  return constant_CB->isChecked();
}

void multi_alt_exp_dialog::set_constant(bool val)
{
  constant_CB->setChecked(val);
}


int multi_alt_exp_dialog::get_n_exp()
{
  return n_exp_SB->value();
}

void multi_alt_exp_dialog::set_n_exp(int val)
{
  n_exp_SB->setValue(val);
}

int multi_alt_exp_dialog::get_m_exp()
{
  return m_exp_SB->value();
}

void multi_alt_exp_dialog::set_m_exp(int val)
{
  m_exp_SB->setValue(val);
}


void multi_alt_exp_dialog::reset()
{
  set_n_exp(start_n_exp);
  set_m_exp(start_m_exp);
  set_BC(false);
}


void multi_alt_exp_dialog::modified_slot()
{
  emit modified();
}


void multi_alt_exp_dialog::createGadgets()
{

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  BC_CB=new QCheckBox("Periodic boundary conditions");
  BC_CB->setChecked(false);
  connect(BC_CB, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));

  constant_CB=new QCheckBox("Time-independent contribution");
  constant_CB->setChecked(false);
  connect(constant_CB, SIGNAL(stateChanged(int)), this, SLOT(modified_slot()));

  text_LB=new QLabel("Multiple (incl. oscillating) Exponentials");

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

  layout=new QGridLayout();

  layout->addWidget(BC_CB, 0, 0);
  layout->addWidget(constant_CB, 1, 0);

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
