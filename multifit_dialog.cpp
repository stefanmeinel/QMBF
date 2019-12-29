#include "multifit_dialog.h"

multifit_dialog::multifit_dialog(QWidget* parent)
{
  setWindowTitle(tr("Multifit in progress"));
  this->resize(200, 100);

  fitLb = new QLabel(tr("Multifit in progress. Please wait..."));

  pb = new QProgressBar();

  abortB = new QPushButton(tr("Abort"));
  abortB -> setMaximumWidth(100);

  hlayout1 = new QHBoxLayout;
  hlayout1 -> addWidget(fitLb);
  hlayout1 -> setAlignment(fitLb, Qt::AlignHCenter);

  hlayout2 = new QHBoxLayout;
  hlayout2 -> addWidget(abortB);
  hlayout2 -> setAlignment(abortB, Qt::AlignHCenter);

  vlayout = new QVBoxLayout;
  vlayout -> addLayout(hlayout1);
  vlayout -> addWidget(pb);
  vlayout -> addLayout(hlayout2);

  setLayout(vlayout);

  connect(abortB, SIGNAL(clicked()), this, SLOT(abortB_pushed()));
}

void multifit_dialog::set_n_data_sets(int n_data_sets)
{
  pb -> setRange(0, n_data_sets);
}


void multifit_dialog::update_bar(int progress)
{
  pb -> setValue(progress);
}


void multifit_dialog::abortB_pushed()
{
  emit abort();
}

