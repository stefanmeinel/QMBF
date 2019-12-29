#include "newbootfiledialog.h"

new_boot_file_dialog::new_boot_file_dialog(QWidget* parent)
{
  setWindowTitle(tr("Create New Bootstrap Ensemble File"));

  createGadgets();
  reset();
}


void new_boot_file_dialog::reset()
{
  datasetsEd->setText(QString("400"));
  samplesEd->setText(QString("500"));
}


int new_boot_file_dialog::get_n_data_sets()
{
  return QString_to_int(datasetsEd->text());
}

int new_boot_file_dialog::get_n_boot_samples()
{
  return QString_to_int(samplesEd->text());
}


void new_boot_file_dialog::createGadgets()
{
  buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Close);

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  datasetsEd=new QLineEdit();
  datasetsEd->setValidator(new QIntValidator(1, 2000000000, datasetsEd));
  datasetsLb=new QLabel("(Number of data sets)/(bin size):");

  samplesEd=new QLineEdit();
  samplesEd->setValidator(new QIntValidator(1, 2000000000, samplesEd));
  samplesLb=new QLabel("Number of bootstrap samples:");

  layout=new QGridLayout();

  layout->addWidget(datasetsLb, 0, 0);
  layout->addWidget(datasetsEd, 0, 1);

  layout->addWidget(samplesLb, 1, 0);
  layout->addWidget(samplesEd, 1, 1);

  vlayout=new QVBoxLayout();

  vlayout->addLayout(layout);
  vlayout->addWidget(buttonBox);

  setLayout(vlayout);
}
