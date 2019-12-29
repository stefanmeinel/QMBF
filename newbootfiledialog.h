#ifndef NEWBOOTFILEDIALOG_H
#define NEWBOOTFILEDIALOG_H

#include <QtWidgets>
#include "convert_qstring.h"

class new_boot_file_dialog : public QDialog
{
  Q_OBJECT

  public:
    new_boot_file_dialog(QWidget* parent = 0);

    int  get_n_data_sets();
    int  get_n_boot_samples();

    void reset();

  private:

    void createGadgets();

    QVBoxLayout* vlayout;
    QGridLayout* layout;

    QLineEdit* datasetsEd;
    QLabel* datasetsLb;

    QLineEdit* samplesEd;
    QLabel* samplesLb;

    QDialogButtonBox* buttonBox;
};

#endif
