#ifndef THREEPTMULTIALTEXPVECDIALOG_H
#define THREEPTMULTIALTEXPVECDIALOG_H

#include <QtWidgets>

class threept_multi_alt_exp_vec_dialog : public QDialog
{
  Q_OBJECT

  public:
    threept_multi_alt_exp_vec_dialog(int n_exp_initial, int max_n_exp_initial, int m_exp_initial, int max_m_exp_initial, int n_exp_final, int max_n_exp_final, int m_exp_final, int max_m_exp_final, int n_vec, int max_n_vec, QWidget* parent);

    void set_text(QString);

    int get_n_exp_initial();
    void set_n_exp_initial(int);

    int get_n_vec();
    void set_n_vec(int);

    int get_m_exp_initial();
    void set_m_exp_initial(int);

    int get_n_exp_final();
    void set_n_exp_final(int);

    int get_m_exp_final();
    void set_m_exp_final(int);

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

    QLabel* n_exp_initial_LB;
    QSpinBox* n_exp_initial_SB;

    QLabel* m_exp_initial_LB;
    QSpinBox* m_exp_initial_SB;

    QLabel* n_exp_final_LB;
    QSpinBox* n_exp_final_SB;

    QLabel* m_exp_final_LB;
    QSpinBox* m_exp_final_SB;

    QLabel* n_vec_LB;
    QSpinBox* n_vec_SB;

    QDialogButtonBox* buttonBox;

    int start_n_exp_initial;
    int start_n_exp_final;
    int max_n_exps_initial;
    int max_n_exps_final;
    int start_m_exp_initial;
    int start_m_exp_final;
    int max_m_exps_initial;
    int max_m_exps_final;
    int start_n_vec;
    int max_n_vec_dim;
};

#endif
