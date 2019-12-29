#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets>
#include <string>
#include <map>
#include "line_edit_delegate.h"
#include "convert_qstring.h"
#include "spacing.h"

using namespace std;

class MainWidget: public QWidget
{
  Q_OBJECT

  public:
    MainWidget(bool bayesian, int n_parameters, int max_n_parameters, QWidget* parent=0);

    void set_bayesian(bool bayesian);
    void set_n_parameters(int new_n_parameters);
    void set_parameter(int p, std::string name);

    std::string get_data_file();
    void set_data_file(std::string name);

    int get_data_file_type();
    void set_data_file_type(int index);

    std::string get_output_dir();
    void set_output_dir(std::string name);

    double get_start_value(int p);
    void set_start_value(int p, double value);

    double get_prior(int p);
    void set_prior(int p, double value);

    double get_sigma(int p);
    void set_sigma(int p, double value);

    void set_result(int p, double value);
    double get_result(int p);

    void set_sqrt_cov(int p, double value);
    double get_sqrt_cov(int p);

    void set_BS_sigma(int p, double value);
    double get_BS_sigma(int p);

    void clear_results();

    void print(std::string text);

    void reset();
    void reset_tables();

    void save_tables();
    void restore_tables();

    void updateSizes();

  protected:
    void resizeEvent(QResizeEvent* event);

  signals:
    void modified();
    void data_file_modified();
    void start_fit();
    void start_BS();

  public slots:
    void printm(QString);

  private slots:
    void modified_slot();
    void data_file_modified_slot();

    void scrollbar_moved(int newpos);

    void select_datafile();
    void select_outputdir();

    void fit();
    void bootstrap();

#ifdef Q_WS_MACX
    void values_key_pressed(QKeyEvent* key_event);
    void param_key_pressed(QKeyEvent* key_event);
    void results_key_pressed(QKeyEvent* key_event);
#endif

  private:
    void createGadgets();

    QLabel* fileLb;
    QLineEdit* fileEd;
    QToolButton* datafilebutton;
    QSpacerItem* spacer1;
    QPushButton* fitbutton;

    QLabel* fileTypeLb;
    QComboBox* fileTypeCb;

    QLabel* outputdirLb;
    QLineEdit* outputdirEd;
    QToolButton* outputdirbutton;
    QSpacerItem* spacer2;
    QPushButton* bootbutton;

    QStandardItemModel* param_model;
    LineEditDelegate* param_delegate;
    QTableView* param_tableView;

    QStandardItemModel* values_model;
    LineEditDelegate* values_delegate;
    QTableView* values_tableView;

    QStandardItemModel* results_model;
    LineEditDelegate* results_delegate;
    QTableView* results_tableView;

    QTextEdit* terminal;

    QVBoxLayout* vlayout;
    QGridLayout* gridlayout1;
    QGridLayout* gridlayout2;

    QString current_dir;
    QString current_output_dir;

    int max_parameters;
    int n_parms;
    bool bayes;
    int start_n_parms;
    bool start_bayes;

    map< string, double > old_start;
    map< string, double > old_prior;
    map< string, double > old_sigma;
};

#endif
