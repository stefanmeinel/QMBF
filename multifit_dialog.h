#ifndef MULTIFIT_DIALOG_H
#define MULTIFIT_DIALOG_H

#include <QtWidgets>

class multifit_dialog : public QDialog
{
  Q_OBJECT

  public:
    multifit_dialog(QWidget* parent = 0);

    void set_n_data_sets(int n_data_sets);

  signals:

    void abort();

  public slots:

    void update_bar(int);

  private slots:

    void abortB_pushed();

  private:

    QProgressBar* pb;

    QVBoxLayout* vlayout;

    QHBoxLayout* hlayout1;
    QHBoxLayout* hlayout2;

    QLabel* fitLb;

    QPushButton* abortB;

};

#endif
