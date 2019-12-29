#ifndef MULTIALTEXPDIALOG_H
#define MULTIALTEXPDIALOG_H

#include <QtWidgets>

class multi_alt_exp_dialog : public QDialog
{
  Q_OBJECT

  public:
    multi_alt_exp_dialog(int n_exp, int max_n_exp, int m_exp, int max_m_exp, QWidget* parent);

    void set_text(QString);

    int get_n_exp();
    void set_n_exp(int);

    int get_m_exp();
    void set_m_exp(int);

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

    QLabel* m_exp_LB;
    QSpinBox* m_exp_SB;

    QDialogButtonBox* buttonBox;

    int start_n_exp;
    int start_m_exp;
    int max_n_exps;
    int max_m_exps;
};

#endif
