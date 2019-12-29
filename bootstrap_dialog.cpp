#include "bootstrap_dialog.h"

bootstrap_dialog::bootstrap_dialog(QWidget* parent)
{
  setWindowTitle(tr("Bootstrap in progress"));
  this->resize(200, 100);

  fitLb = new QLabel(tr("Bootstrap in progress. Please wait..."));

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

void bootstrap_dialog::set_bssamples(int bs)
{
  pb -> setRange(0, bs);
}


void bootstrap_dialog::update_bar(int progress)
{
  pb -> setValue(progress);
}


void bootstrap_dialog::abortB_pushed()
{
  emit abort();
}

