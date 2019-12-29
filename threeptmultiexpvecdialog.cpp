#include "threeptmultiexpvecdialog.h"

threept_multi_exp_vec_dialog::threept_multi_exp_vec_dialog(int n_exp_initial, int max_n_exp_initial, int n_exp_final, int max_n_exp_final, int n_vec, int max_n_vec, QWidget* parent)
{
  start_n_exp_initial=n_exp_initial;
  max_n_exps_initial=max_n_exp_initial;
  start_n_exp_final=n_exp_final;
  max_n_exps_final=max_n_exp_final;
  start_n_vec=n_vec;
  max_n_vec_dim=max_n_vec;

  setWindowTitle(tr("Create 3-Point Multiple Exponentials Vector Fit Model"));

  createGadgets();
}

void threept_multi_exp_vec_dialog::set_text(QString text)
{
  text_LB->setText(text);
}


int threept_multi_exp_vec_dialog::get_n_exp_initial()
{
  return n_exp_initial_SB->value();
}

void threept_multi_exp_vec_dialog::set_n_exp_initial(int val)
{
  n_exp_initial_SB->setValue(val);
}

int threept_multi_exp_vec_dialog::get_n_exp_final()
{
  return n_exp_final_SB->value();
}

void threept_multi_exp_vec_dialog::set_n_exp_final(int val)
{
  n_exp_final_SB->setValue(val);
}


int threept_multi_exp_vec_dialog::get_n_vec()
{
  return n_vec_SB->value();
}

void threept_multi_exp_vec_dialog::set_n_vec(int val)
{
  n_vec_SB->setValue(val);
}



void threept_multi_exp_vec_dialog::reset()
{
  set_n_exp_initial(start_n_exp_initial);
  set_n_exp_final(start_n_exp_final);
  set_n_vec(start_n_vec);
}


void threept_multi_exp_vec_dialog::modified_slot()
{
  emit modified();
}


void threept_multi_exp_vec_dialog::createGadgets()
{

  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  text_LB=new QLabel("3-Point Correlator, Multiple Exponentials, Vector Fit");

  n_vec_LB=new QLabel("Dimension:");
  n_vec_SB=new QSpinBox();
  n_vec_SB->setMinimum(1);
  n_vec_SB->setValue(start_n_vec);
  n_vec_SB->setMaximum(max_n_vec_dim);
  connect(n_vec_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  n_exp_initial_LB=new QLabel("Number of exponentials for initial state:");
  n_exp_initial_SB=new QSpinBox();
  n_exp_initial_SB->setMinimum(1);
  n_exp_initial_SB->setValue(start_n_exp_initial);
  n_exp_initial_SB->setMaximum(max_n_exps_initial);
  connect(n_exp_initial_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  n_exp_final_LB=new QLabel("Number of exponentials for final state:");
  n_exp_final_SB=new QSpinBox();
  n_exp_final_SB->setMinimum(1);
  n_exp_final_SB->setValue(start_n_exp_final);
  n_exp_final_SB->setMaximum(max_n_exps_initial);
  connect(n_exp_final_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));

  layout=new QGridLayout();

  layout->addWidget(n_vec_LB, 0, 0);
  layout->addWidget(n_vec_SB, 0, 1);

  layout->addWidget(n_exp_initial_LB, 1, 0);
  layout->addWidget(n_exp_initial_SB, 1, 1);

  layout->addWidget(n_exp_final_LB, 2, 0);
  layout->addWidget(n_exp_final_SB, 2, 1);

  vlayout=new QVBoxLayout();

  vlayout->addWidget(text_LB);
  vlayout->addLayout(layout);
  vlayout->addWidget(buttonBox);

  setLayout(vlayout);
}
