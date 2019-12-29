#ifndef MULTIEXPDIALOG_H
#define MULTIEXPDIALOG_H

#include <QtWidgets>

class multi_exp_dialog : public QDialog
{
  Q_OBJECT

  public:
    multi_exp_dialog(int n_exp, int max_n_exp, QWidget* parent = 0);

    int get_n_exp();
    void set_n_exp(int);
    void set_text(QString);

    bool get_BC();
    void set_BC(bool);

    bool get_constant();
    void set_constant(bool);

    void reset();

  private slots:

    void modified_slot();

  signals:

    void modified();


  private:

    void createGadgets();

    QVBoxLayout* vlayout;
    QGridLayout* layout;


    QCheckBox* BC_CB;
    QCheckBox* constant_CB;

    QLabel* text_LB;
    QLabel* n_exp_LB;
    QSpinBox* n_exp_SB;

    QDialogButtonBox* buttonBox;

    int start_n_exp;
    int max_n_exps;
};

#endif
