#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QtWidgets>
#include <string>
#include "line_edit_delegate.h"
#include "spacing.h"

class FunctionWidget: public QWidget
{
  Q_OBJECT

  public:
    FunctionWidget(int n_functions, int max_n_functions, QWidget* parent=0);

    void set_user_def(bool);

    bool set_n_functions(int);
    int get_n_functions();

    void set_function(int f, std::string name);
    std::string get_function(int f);

    void reset();

    void updateSizes();

  protected:
    void resizeEvent(QResizeEvent* event);

  signals:
    void modified();
    void n_functions_modified(int new_n_functions);
    void function_changed(int f, std::string new_name);

  private slots:
    void modified_slot();
    void n_functions_modified_slot(int new_n_functions);
    void functions_changed_slot(const QModelIndex&, const QModelIndex&);

#ifdef Q_WS_MACX
    void functions_key_pressed(QKeyEvent* key_event);
#endif

  private:

    void createGadgets();

    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout;

    QSpinBox* n_functions_SB;
    QLabel* n_functions_Lb;

    QStandardItemModel* functions_model;
    LineEditDelegate* functions_delegate;
    QTableView* functions_tableView;

    int max_functions;
    int n_funcs;
};

#endif
