#ifndef PARAMDERIVSWIDGET_H
#define PARAMDERIVSWIDGET_H

#include <QtWidgets>
#include <string>
#include <sstream>
#include "line_edit_delegate.h"

class ParamDerivsWidget: public QWidget
{
  Q_OBJECT

  public:
    ParamDerivsWidget(bool num_diff, int n_functions, int max_n_functions, int n_parameters, int max_n_parameters, QWidget* parent=0);

    void set_user_def(bool);

    void set_num_diff(bool);

    bool set_n_parameters(int);
    int get_n_parameters();

    void set_n_parameters_dof(int);
    int get_n_parameters_dof();

    void set_n_functions(int);
    int get_n_functions();

    void set_parameter(int p, std::string name);
    std::string get_parameter(int p);

    void set_derivative(int f, int p, std::string name);
    std::string get_derivative(int f, int p);

    void reset();

  protected:
    void resizeEvent(QResizeEvent* event);

  signals:
    void modified();
    void n_parameters_modified(int new_n_parameters);
    void n_functions_modified(int new_n_functions);
    void parameter_changed(int p, std::string new_name);
    void derivative_changed(int f, int p, std::string new_name);

  private slots:
    void modified_slot();
    void n_parameters_modified_slot(int new_n_parameters);
    void n_functions_modified_slot(int new_n_functions);
    void parameters_changed_slot(const QModelIndex&, const QModelIndex&);
    void derivatives_changed_slot(const QModelIndex&, const QModelIndex&);

#ifdef Q_WS_MACX
    void parder_key_pressed(QKeyEvent* key_event);
#endif

  private:

    void createGadgets();

    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout;
    QHBoxLayout* hlayout2;

    QSpinBox* n_parameters_SB;
    QLabel* n_parameters_Lb;

    QSpinBox* n_parameters_dof_SB;
    QLabel* n_parameters_dof_Lb;

    QStandardItemModel* parder_model;
    LineEditDelegate* parder_delegate;
    QTableView* parder_tableView;

    int max_parameters;
    int max_functions;
    int start_n_parms;
    int start_n_funcs;
    int n_parms;
    int n_funcs;

    bool userdef;
    bool numdiff;
};

#endif
