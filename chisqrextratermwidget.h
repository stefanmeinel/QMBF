#ifndef CHISQREXTRATERMWIDGET_H
#define CHISQREXTRATERMWIDGET_H

#include <QtWidgets>
#include <string>
#include "line_edit_delegate.h"
#include "convert_qstring.h"
#include "spacing.h"

class ChiSqrExtraTermWidget: public QWidget
{
  Q_OBJECT

  public:
    ChiSqrExtraTermWidget(int n_constants, int max_n_constants, QWidget* parent=0);

    void set_enabled(bool);
    bool get_enabled();

    bool set_n_constants(int);
    int get_n_constants();

    void set_constant_name(int c, std::string name);
    std::string get_constant_name(int c);

    void set_constant_value(int c, double value);
    double get_constant_value(int c);

    void set_num_diff_step(double);
    double get_num_diff_step();

    void set_function(std::string func);
    std::string get_function();

    void reset();

    void updateSizes();

  protected:
    void resizeEvent(QResizeEvent* event);

  signals:
    void modified();
    void enabled_modified();
    void function_modified();
    void n_constants_modified(int new_n_constants);
    void constant_changed(int c, std::string new_name);

  private slots:
    void modified_slot();
    void enabled_modified_slot(int);
    void function_modified_slot();
    void n_constants_modified_slot(int new_n_constants);
    void scrollbar_moved(int newpos);
    void constants_changed_slot(const QModelIndex&, const QModelIndex&);

#ifdef Q_WS_MACX
    void constantnames_key_pressed(QKeyEvent* key_event);
    void constantvalues_key_pressed(QKeyEvent* key_event);
#endif

  private:

    void createGadgets();

    void set_user_def(bool);

    QGridLayout* gridlayout;
    QSpacerItem* spacer1;

    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout1;
    QHBoxLayout* hlayout2;

    QCheckBox* enabled_cb;

    QLineEdit* numdiffEd;
    QLabel* numdiffLb;

    QLineEdit* funcEd;
    QLabel* funcLb;


    QSpinBox* n_constants_SB;
    QLabel* n_constants_Lb;

    QStandardItemModel* constantnames_model;
    LineEditDelegate* constantnames_delegate;
    QTableView* constantnames_tableView;

    QStandardItemModel* constantvalues_model;
    LineEditDelegate* constantvalues_delegate;
    QTableView* constantvalues_tableView;

    int max_constants;
    int n_consts;
};

#endif
