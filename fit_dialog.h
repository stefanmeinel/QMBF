#ifndef FIT_DIALOG_H
#define FIT_DIALOG_H

#include <QtWidgets>
#include <string>
#include <sstream>

class fit_dialog : public QDialog
{
  Q_OBJECT

  public:
    fit_dialog(QWidget* parent = 0);

    void set_dof(int dof);
    void clear();

  signals:

    void abort();

  private slots:

    void abortB_pushed();
    void step_done(int step, double chi_sqr, double lambda);

  private:

    QVBoxLayout* vlayout;

    QHBoxLayout* hlayout1;
    QHBoxLayout* hlayout2;

    QTextEdit* terminal;

    QLabel* fitLb;

    QPushButton* abortB;
    
    int d_o_f;

};

#endif
