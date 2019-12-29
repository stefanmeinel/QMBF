#ifndef THREEPTMULTIEXPDIALOG_H
#define THREEPTMULTIEXPDIALOG_H

#include <QtWidgets>

class threept_multi_exp_dialog : public QDialog
{
  Q_OBJECT

  public:
    threept_multi_exp_dialog(int n_exp_initial, int max_n_exp_initial, int n_exp_final, int max_n_exp_final, QWidget* parent);

    void set_text(QString);

    int get_n_exp_initial();
    void set_n_exp_initial(int);

    int get_n_exp_final();
    void set_n_exp_final(int);

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

    QLabel* n_exp_final_LB;
    QSpinBox* n_exp_final_SB;

    QDialogButtonBox* buttonBox;

    int start_n_exp_initial;
    int start_n_exp_final;
    int max_n_exps_initial;
    int max_n_exps_final;
};

#endif
