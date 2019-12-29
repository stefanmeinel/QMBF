#ifndef BOOTSTRAP_DIALOG_H
#define BOOTSTRAP_DIALOG_H

#include <QtWidgets>

class bootstrap_dialog : public QDialog
{
  Q_OBJECT

  public:
    bootstrap_dialog(QWidget* parent = 0);

    void set_bssamples(int bs);

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
