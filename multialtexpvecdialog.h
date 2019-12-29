#ifndef MULTIALTEXPVECDIALOG_H
#define MULTIALTEXPVECDIALOG_H

#include <QtWidgets>

class multi_alt_exp_vec_dialog : public QDialog
{
  Q_OBJECT

  public:
    multi_alt_exp_vec_dialog(int n_exp, int max_n_exp, int m_exp, int max_m_exp, int n_vec, int max_n_vec, QWidget* parent = 0);

    void set_text(QString);

    int get_n_exp();
    void set_n_exp(int);

    int get_m_exp();
    void set_m_exp(int);

    int get_n_vec();
    void set_n_vec(int);

    bool get_BC();
    void set_BC(bool);

    void set_BC_enabled(bool);

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

    QLabel* n_vec_LB;
    QSpinBox* n_vec_SB;

    QDialogButtonBox* buttonBox;

    int start_n_exp;
    int max_n_exps;
    int start_m_exp;
    int max_m_exps;
    int start_n_vec;
    int max_n_vec_dim;
};

#endif
