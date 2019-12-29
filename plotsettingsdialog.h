#ifndef PLOTSETTINGSDIALOG_H
#define PLOTSETTINGSDIALOG_H

#include <QtWidgets>
#include "line_edit_delegate.h"
#include "convert_qstring.h"
#include "spacing.h"

class plot_settings_dialog : public QDialog
{
  Q_OBJECT

  public:
    plot_settings_dialog(int n_variables, int max_n_variables, QWidget* parent = 0);

    void restore_geometry();

    std::string get_grace();
    void set_grace(std::string);

    int get_viewplot();
    void set_viewplot(int index);

    double get_min();
    void set_min(double);

    double get_max();
    void set_max(double);

    double get_min_y();
    void set_min_y(double);

    double get_max_y();
    void set_max_y(double);

    double get_min_em_y();
    void set_min_em_y(double);

    double get_max_em_y();
    void set_max_em_y(double);

    void set_n_variables(int);

    int get_plot_var();
    void set_plot_var(int v);

    double get_variable_value(int v);
    void set_variable_value(int v, double value);

    std::string get_eff_mass();
    void set_eff_mass(std::string);

    bool continuous();
    void set_continuous(bool);

    bool data_range();
    void set_data_range(bool);

    bool data_range_y();
    void set_data_range_y(bool);

    bool get_log();
    void set_log(bool);

    bool get_neg();
    void set_neg(bool);

    void reset();

    void updateSizes();

  protected:
    void hideEvent(QHideEvent* event);
    void resizeEvent(QResizeEvent* event);

  private slots:
    void data_range_CB_changed(int newstate);
    void data_range_y_CB_changed(int newstate);
    void modified_slot();
    void plot_var_modified_slot(int new_plot_var);

  signals:

    void hide();
    void modified();

  private:

    void createGadgets();

    QVBoxLayout* vlayout;

    QGridLayout* gridlayout1;
    QGridLayout* gridlayout2;
    QGridLayout* gridlayout3;
    QGridLayout* gridlayout4;
    QSpacerItem* spacer0;
    QSpacerItem* spacer1;
    QSpacerItem* spacer2;
    QSpacerItem* spacer3;
    QSpacerItem* spacer4;

    QHBoxLayout* hlayout1;

    QSpinBox* plot_var_SB;
    QLabel* plot_var_Lb;

    QLabel* vv_Lb;
    QStandardItemModel* variablevalues_model;
    LineEditDelegate* variablevalues_delegate;
    QTableView* variablevalues_tableView;

    QLabel* grace_LB;
    QLineEdit* grace_LE;

    QLabel* viewplotLb;
    QComboBox* viewplotCb;

    QCheckBox* continuous_CB;

    QCheckBox* data_range_CB;
    QCheckBox* data_range_y_CB;

    QCheckBox* log_CB;

    QCheckBox* neg_CB;

    QLabel* plot_min_LB;
    QLineEdit* plot_min_LE;

    QLabel* plot_max_LB;
    QLineEdit* plot_max_LE;

    QLabel* plot_min_y_LB;
    QLineEdit* plot_min_y_LE;

    QLabel* plot_max_y_LB;
    QLineEdit* plot_max_y_LE;

    QLabel* plot_min_em_y_LB;
    QLineEdit* plot_min_em_y_LE;

    QLabel* plot_max_em_y_LB;
    QLineEdit* plot_max_em_y_LE;

    QLabel* eff_mass_LB;
    QLineEdit* eff_mass_LE;

    QDialogButtonBox* buttonBox;

    int max_variables;
    int n_vars;
    int plot_var;

    QByteArray geometry;
};

#endif
