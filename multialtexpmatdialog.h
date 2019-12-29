#ifndef MULTIALTEXPMATDIALOG_H
#define MULTIALTEXPMATDIALOG_H

#include <QtWidgets>

class multi_alt_exp_mat_dialog : public QDialog
{
  Q_OBJECT

  public:
    multi_alt_exp_mat_dialog(int n_exp, int max_n_exp, int m_exp, int max_m_exp, int dim_1, int dim_2, int max_dim,  QWidget* parent = 0);

    void set_text(QString);

    int get_n_exp();
    void set_n_exp(int);

    int get_m_exp();
    void set_m_exp(int);

    void set_min_dim_1(int);

    int get_dim_1();
    void set_dim_1(int);

    int get_dim_2();
    void set_dim_2(int);

    void reset();

  private slots:

    void modified_slot();

  signals:

    void modified();

  private:

    void createGadgets();

    QVBoxLayout* vlayout;
    QGridLayout* layout;

    QLabel* text_LB;
    QLabel* n_exp_LB;
    QSpinBox* n_exp_SB;

    QLabel* m_exp_LB;
    QSpinBox* m_exp_SB;

    QLabel* dim_1_LB;
    QSpinBox* dim_1_SB;

    QLabel* dim_2_LB;
    QSpinBox* dim_2_SB;

    QDialogButtonBox* buttonBox;

    int start_n_exp;
    int max_n_exps;
    int start_m_exp;
    int max_m_exps;
    int start_dim_1;
    int start_dim_2;
    int maxdim;
};

#endif
