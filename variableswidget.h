#ifndef VARIABLESWIDGET_H
#define VARIABLESWIDGET_H

#include <QtWidgets>
#include <string>
#include "line_edit_delegate.h"
#include "convert_qstring.h"
#include "spacing.h"

class VariablesWidget: public QWidget
{
  Q_OBJECT

  public:
    VariablesWidget(int n_variables, int max_n_variables, QWidget* parent=0);

    void set_user_def(bool);

    void set_n_variables(int);
    int get_n_variables();

    void set_variable_name(int v, std::string name);
    std::string get_variable_name(int v);

    void set_fit_min(int v, std::string value);
    std::string get_fit_min(int v);

    void set_fit_max(int v, std::string value);
    std::string get_fit_max(int v);

    void reset();

    void updateSizes();

  protected:
    void resizeEvent(QResizeEvent* event);

  signals:
    void modified();
    void n_variables_modified(int new_n_variables);
    void variable_changed(int v, std::string new_name);
    void range_changed();

  private slots:
    void modified_slot();
    void n_variables_modified_slot(int new_n_variables);
    void scrollbar_moved(int newpos);
    void variables_changed_slot(const QModelIndex&, const QModelIndex&);
    void ranges_changed_slot();

#ifdef Q_WS_MACX
    void variablenames_key_pressed(QKeyEvent* key_event);
    void fit_min_max_key_pressed(QKeyEvent* key_event);
#endif

  private:

    void createGadgets();

    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout;
    QGridLayout* gridlayout;

    QSpinBox* n_variables_SB;
    QLabel* n_variables_Lb;

    QStandardItemModel* variablenames_model;
    LineEditDelegate* variablenames_delegate;
    QTableView* variablenames_tableView;

    QStandardItemModel* fit_min_max_model;
    LineEditDelegate* fit_min_max_delegate;
    QTableView* fit_min_max_tableView;

    int max_variables;
    int n_vars;
    int start_n_vars;
};

#endif
