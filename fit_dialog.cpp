#include "fit_dialog.h"

fit_dialog::fit_dialog(QWidget* parent)
{
  setWindowTitle(tr("Fitting in progress"));
  this->resize(400, 200);

  fitLb = new QLabel(tr("Fitting in progress. Please wait..."));

  abortB = new QPushButton(tr("Abort"));
  abortB -> setMaximumWidth(100);

  hlayout1 = new QHBoxLayout;
  hlayout1 -> addWidget(fitLb);
  hlayout1 -> setAlignment(fitLb, Qt::AlignHCenter);

  hlayout2 = new QHBoxLayout;
  hlayout2 -> addWidget(abortB);
  hlayout2 -> setAlignment(abortB, Qt::AlignHCenter);

  terminal=new QTextEdit();
  terminal->setReadOnly(true);
  terminal->setLineWrapMode(QTextEdit::NoWrap);
  terminal->setMaximumHeight(200);
  
  vlayout = new QVBoxLayout;
  vlayout -> addLayout(hlayout1);
  vlayout -> addWidget(terminal);
  vlayout -> addLayout(hlayout2);

  setLayout(vlayout);

  connect(abortB, SIGNAL(clicked()), this, SLOT(abortB_pushed()));
}


void fit_dialog::set_dof(int dof)
{
  d_o_f=dof;
}


void fit_dialog::clear()
{
  terminal->clear();
}


void fit_dialog::abortB_pushed()
{
  emit abort();
}

void fit_dialog::step_done(int step, double chi_sqr, double lambda)
{
  std::stringstream sst;
  sst << "step " << step << ":  chi^2/dof = " << chi_sqr/d_o_f << "  lambda = " << lambda;
  terminal->append(QString(sst.str().c_str())); 
  terminal->moveCursor(QTextCursor::End);
}
