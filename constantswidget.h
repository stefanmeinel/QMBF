#ifndef CONSTANTSWIDGET_H
#define CONSTANTSWIDGET_H

#include <QtWidgets>
#include <string>
#include "line_edit_delegate.h"
#include "convert_qstring.h"
#include "spacing.h"

class ConstantsWidget: public QWidget
{
  Q_OBJECT

  public:
    ConstantsWidget(int n_constants, int max_n_constants, QWidget* parent=0);

    void set_user_def(bool);

    bool set_n_constants(int);
    int get_n_constants();

    void set_constant_name(int c, std::string name);
    std::string get_constant_name(int c);

    void set_constant_value(int c, double value);
    double get_constant_value(int c);

    void reset();

    void updateSizes();

  protected:
    void resizeEvent(QResizeEvent* event);

  signals:
    void modified();
    void n_constants_modified(int new_n_constants);
    void constant_changed(int c, std::string new_name);

  private slots:
    void modified_slot();
    void n_constants_modified_slot(int new_n_constants);
    void scrollbar_moved(int newpos);
    void constants_changed_slot(const QModelIndex&, const QModelIndex&);

#ifdef Q_WS_MACX
    void constantnames_key_pressed(QKeyEvent* key_event);
    void constantvalues_key_pressed(QKeyEvent* key_event);
#endif

  private:

    void createGadgets();

    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout1;
    QHBoxLayout* hlayout2;

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
