#ifndef MULTIEXPMATUPPERDIALOG_H
#define MULTIEXPMATUPPERDIALOG_H

#include <QtWidgets>

class multi_exp_mat_upper_dialog : public QDialog
{
  Q_OBJECT

  public:
    multi_exp_mat_upper_dialog(int n_exp, int max_n_exp, int dim, int max_dim, QWidget* parent = 0);

    int get_n_exp();
    void set_n_exp(int);

    int get_dim();
    void set_dim(int);

    void set_text(QString);

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

    QLabel* dim_LB;
    QSpinBox* dim_SB;

    QDialogButtonBox* buttonBox;

    int start_n_exp;
    int max_n_exps;
    int start_dim;
    int maxdim;
};

#endif
