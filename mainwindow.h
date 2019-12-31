#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_errno.h>

#ifdef Q_WS_MACX
  #include "file_open_filter.h"
#endif

#include "parser.h"

#include "mainwidget.h"
#include "functionwidget.h"
#include "variableswidget.h"
#include "paramderivswidget.h"
#include "constantswidget.h"
#include "chisqrextratermwidget.h"
#include "fitsettingswidget.h"

#include "multiexpdialog.h"
#include "multiexpvecdialog.h"
#include "multiexpmatdialog.h"
#include "multiexpmatupperdialog.h"
#include "multialtexpdialog.h"
#include "multialtexpvecdialog.h"
#include "multialtexpmatdialog.h"
#include "threeptmultiexpdialog.h"
#include "threeptmultiexpvecdialog.h"
#include "threeptmultialtexpdialog.h"
#include "threeptmultialtexpvecdialog.h"

#include "plotsettingsdialog.h"

#include "plotview.h"
#include "report_view.h"

#include "chisqr_extra_term.h"
#include "fitter.h"
#include "fit_thread.h"
#include "bootstrap_thread.h"
#include "multifit_thread.h"
#include "fit_dialog.h"
#include "bootstrap_dialog.h"
#include "multifit_dialog.h"
#include "abstract_prior.h"
#include "gaussian_prior.h"
#include "abstract_model.h"


#include "parse_model.h"

#include "parser.h"

#include "binary_io.h"

#include "multi_alt_exp_Asqr_BC_model.h"
#include "multi_alt_exp_Asqr_expE_BC_model.h"
#include "multi_alt_exp_Asqr_expE_model.h"
#include "multi_alt_exp_Asqr_expE_vec_BC_model.h"
#include "multi_alt_exp_Asqr_expE_vec_model.h"
#include "multi_alt_exp_Asqr_model.h"
#include "multi_alt_exp_Asqr_vec_BC_model.h"
#include "multi_alt_exp_Asqr_vec_model.h"
#include "multi_alt_exp_BC_model.h"
#include "multi_alt_exp_expE_BC_model.h"
#include "multi_alt_exp_expE_mat_model.h"
#include "multi_alt_exp_expE_model.h"
#include "multi_alt_exp_expE_nonsym_mat_model.h"
#include "multi_alt_exp_expE_vec_BC_model.h"
#include "multi_alt_exp_expE_vec_model.h"
#include "multi_alt_exp_mat_model.h"
#include "multi_alt_exp_model.h"
#include "multi_alt_exp_nonsym_mat_model.h"
#include "multi_alt_exp_vec_BC_model.h"
#include "multi_alt_exp_vec_model.h"
#include "multi_exp_Asqr_BC_model.h"
#include "multi_exp_Asqr_expE_BC_model.h"
#include "multi_exp_Asqr_expE_model.h"
#include "multi_exp_Asqr_expE_vec_BC_model.h"
#include "multi_exp_Asqr_expE_vec_model.h"
#include "multi_exp_Asqr_model.h"
#include "multi_exp_Asqr_vec_BC_model.h"
#include "multi_exp_Asqr_vec_model.h"
#include "multi_exp_BC_model.h"
#include "multi_exp_expE_BC_model.h"
#include "multi_exp_expE_mat_model.h"
#include "multi_exp_expE_mat_II_model.h"
#include "multi_exp_expE_mat_upper_model.h"
#include "multi_exp_expE_mat_II_upper_model.h"
#include "multi_exp_expE_model.h"
#include "multi_exp_expE_nonsym_mat_model.h"
#include "multi_exp_expE_vec_BC_model.h"
#include "multi_exp_expE_vec_model.h"
#include "multi_exp_mat_model.h"
#include "multi_exp_mat_II_model.h"
#include "multi_exp_mat_upper_model.h"
#include "multi_exp_mat_II_upper_model.h"
#include "multi_exp_model.h"
#include "multi_exp_nonsym_mat_model.h"
#include "multi_exp_vec_BC_model.h"
#include "multi_exp_vec_model.h"
#include "multi_exp_const_model.h"
#include "multi_exp_expE_const_model.h"
#include "multi_exp_Asqr_const_model.h"
#include "multi_exp_Asqr_expE_const_model.h"
#include "multi_alt_exp_const_model.h"
#include "multi_alt_exp_expE_const_model.h"
#include "multi_alt_exp_Asqr_const_model.h"
#include "multi_alt_exp_Asqr_expE_const_model.h"
#include "multi_exp_vec_const_model.h"
#include "multi_exp_expE_vec_const_model.h"
#include "multi_exp_Asqr_vec_const_model.h"
#include "multi_exp_Asqr_expE_vec_const_model.h"
#include "multi_alt_exp_vec_const_model.h"
#include "multi_alt_exp_expE_vec_const_model.h"
#include "multi_alt_exp_Asqr_vec_const_model.h"
#include "multi_alt_exp_Asqr_expE_vec_const_model.h"
#include "multi_exp_BC_const_model.h"
#include "multi_exp_expE_BC_const_model.h"
#include "multi_exp_Asqr_BC_const_model.h"
#include "multi_exp_Asqr_expE_BC_const_model.h"
#include "multi_alt_exp_BC_const_model.h"
#include "multi_alt_exp_expE_BC_const_model.h"
#include "multi_alt_exp_Asqr_BC_const_model.h"
#include "multi_alt_exp_Asqr_expE_BC_const_model.h"
#include "multi_exp_vec_BC_const_model.h"
#include "multi_exp_expE_vec_BC_const_model.h"
#include "multi_exp_Asqr_vec_BC_const_model.h"
#include "multi_exp_Asqr_expE_vec_BC_const_model.h"
#include "multi_alt_exp_vec_BC_const_model.h"
#include "multi_alt_exp_expE_vec_BC_const_model.h"
#include "multi_alt_exp_Asqr_vec_BC_const_model.h"
#include "multi_alt_exp_Asqr_expE_vec_BC_const_model.h"
#include "threept_multi_alt_exp_expE_model.h"
#include "threept_multi_alt_exp_expE_vec_model.h"
#include "threept_multi_alt_exp_model.h"
#include "threept_multi_alt_exp_vec_model.h"
#include "threept_multi_exp_expE_model.h"
#include "threept_multi_exp_expE_vec_model.h"
#include "threept_multi_exp_model.h"
#include "threept_multi_exp_vec_model.h"

#include "settingsmap.h"

using namespace std;

class QAction;
class QMenu;

const double version=5.65;

const int start_n_functions=1;
const int max_n_functions=100;

const int start_n_variables=1;
const int max_n_variables=100;

const int start_n_parameters=1;
const int max_n_parameters=500;

const int start_n_constants=0;
const int max_n_constants=100;

const bool start_bayes=false;
const bool start_num_diff=true;
const bool start_second_deriv_minimization=false;
const bool start_second_deriv_covariance=true;
const bool start_use_bse=false;
const bool start_restrict_data=false;
const int start_start_data=1;
const int start_stop_data=1000;
const double start_num_diff_step=1e-08;
const double start_startlambda=0.001;
const double start_lambdafac=10.0;
const double start_tolerance=0.001;
const int start_svd=0;
const double start_svd_ratio=0.000001;
const double start_svd_value=0.000000000001;
const double start_rescale_value=0.99;
const int start_steps=100;
const int start_bin=1;
const int start_bssamples=500;

const int start_n_exp=1;
const int max_n_exp=100;
const int start_m_exp=1;
const int max_m_exp=100;
const int start_n_vec=2;
const int max_n_vec=100;

const string A_name="A";
const string B_name="B";
const string C_name="C";
const string E_name="E";
const string dE_name="dE";
const string E_initial_name="E";
const string dE_initial_name="dE";
const string E_final_name="F";
const string dE_final_name="dF";
const string t_name="t";
const string T_name="T";

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(bool load_file, QString file_name);

  signals:
    void lf(QString);

  protected:

    void closeEvent(QCloseEvent*);

#ifdef Q_WS_MACX
  public slots:
    void loadFile(const QString& fileName);
#endif

  private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();

    void parse_triggered();
    void parse_toggled();

    void show_m_exp();
    void show_m_exp_a_sqr();
    void show_m_exp_a_sqr_exp_e();
//    void show_m_exp_exp_a();
    void show_m_exp_exp_e();
//    void show_m_exp_exp_a_exp_e();
    void show_m_alt_exp();
    void show_m_alt_exp_a_sqr();
    void show_m_alt_exp_a_sqr_exp_e();
//    void show_m_alt_exp_exp_a();
    void show_m_alt_exp_exp_e();
//    void show_m_alt_exp_exp_a_exp_e();
    void show_m_exp_vec();
    void show_m_exp_a_sqr_vec();
    void show_m_exp_a_sqr_exp_e_vec();
//    void show_m_exp_exp_a_vec();
    void show_m_exp_exp_e_vec();
//    void show_m_exp_exp_a_exp_e_vec();
    void show_m_alt_exp_vec();
    void show_m_alt_exp_a_sqr_vec();
    void show_m_alt_exp_a_sqr_exp_e_vec();
//    void show_m_alt_exp_exp_a_vec();
    void show_m_alt_exp_exp_e_vec();
//    void show_m_alt_exp_exp_a_exp_e_vec();
    void show_m_exp_mat();
    void show_m_exp_mat_II();
    void show_m_exp_exp_e_mat();
    void show_m_exp_exp_e_mat_II();
    void show_m_exp_mat_upper();
    void show_m_exp_mat_II_upper();
    void show_m_exp_exp_e_mat_upper();
    void show_m_exp_exp_e_mat_II_upper();
    void show_m_alt_exp_mat();
    void show_m_alt_exp_exp_e_mat();
    void show_m_exp_nonsym_mat();
    void show_m_exp_exp_e_nonsym_mat();
    void show_m_alt_exp_nonsym_mat();
    void show_m_alt_exp_exp_e_nonsym_mat();
    void show_threept_2var_m_exp();
    void show_threept_2var_m_exp_exp_e();
    void show_threept_2var_m_exp_vec();
    void show_threept_2var_m_exp_exp_e_vec();
    void show_threept_2var_m_alt_exp();
    void show_threept_2var_m_alt_exp_exp_e();
    void show_threept_2var_m_alt_exp_vec();
    void show_threept_2var_m_alt_exp_exp_e_vec();
    void update_model_widgets();

    void show_plot_settings();
    void plot_data();
    void plot_all();
    void plot_start();
    void plot_eff_mass();

    void start_to_prior();
    void result_to_start();
    void result_to_prior();
    void chisqr_start();
    void reload_data_file();
    void show_report();
    void write_results();

    void bayesian_modified(int);
    void chisqrextra_enabled_modified();
    void num_diff_modified(int);
    void second_deriv_minimization_modified(int);
    void second_deriv_covariance_modified(int);
    void bin_modified();
    void svd_modified();
    void cov_normalization_modified();
    void n_functions_modified(int);
    void n_parameters_modified(int);
    void n_constants_modified();
    void n_variables_modified();
    void parameter_changed(int, std::string);
    void derivative_changed();
    void function_changed();
    void constant_changed();
    void variable_changed();
    void fit_ranges_changed();

    void aboutWindow();

    void modified();
    void data_file_modified();

#ifndef Q_WS_MACX
    void loadFile(const QString& fileName);
#endif

    void fit();
    void bootstrap();
    void multifit();
    void fit_finished(int boot=0);
    void abort_fit();
    void abort_bootstrap();
    void bootstrap_finished();
    void abort_multifit();
    void multifit_finished();

  private:

    void createActions();
    void createMenus();
    void create_Widgets_Dialogs();

    void check_current_model();

    bool prepare_fit();

    bool load_data_file();

    bool set_fit_data();

    bool set_plot_data();

    bool reset_fitter();

    void reset();

    bool maybeSave();
    bool saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);

    QString strippedName(const QString& fullFileName);

    QTabWidget* tabWidget;

    QMenu* fileMenu;
    QMenu* modelMenu;

    QMenu* scalarmodelMenu;
    QMenu* vectormodelMenu;
    QMenu* matrixmodelMenu;
    QMenu* matrixIImodelMenu;
    QMenu* matrixuppermodelMenu;
    QMenu* matrixIIuppermodelMenu;
    QMenu* nonsymmatrixmodelMenu;
    QMenu* scalar3pt2varmodelMenu;
    QMenu* vector3pt2varmodelMenu;

    QMenu* plotMenu;
    QMenu* toolsMenu;
    QMenu* helpMenu;

    QToolBar* fileToolBar;
    QToolBar* plotToolBar;
    QToolBar* toolsToolBar;

    QString current_dir;
    QString curFile;

    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* exitAct;

    QAction* parse_Act;

    QAction* m_exp_Act;
    QAction* m_exp_a_sqr_Act;
    QAction* m_exp_a_sqr_exp_e_Act;
//    QAction* m_exp_exp_a_Act;
    QAction* m_exp_exp_e_Act;
//    QAction* m_exp_exp_a_exp_e_Act;
    QAction* m_alt_exp_Act;
    QAction* m_alt_exp_a_sqr_Act;
    QAction* m_alt_exp_a_sqr_exp_e_Act;
//    QAction* m_alt_exp_exp_a_Act;
    QAction* m_alt_exp_exp_e_Act;
//    QAction* m_alt_exp_exp_a_exp_e_Act;
    QAction* m_exp_vec_Act;
    QAction* m_exp_a_sqr_vec_Act;
    QAction* m_exp_a_sqr_exp_e_vec_Act;
//    QAction* m_exp_exp_a_vec_Act;
    QAction* m_exp_exp_e_vec_Act;
//    QAction* m_exp_exp_a_exp_e_vec_Act;
    QAction* m_alt_exp_vec_Act;
    QAction* m_alt_exp_a_sqr_vec_Act;
    QAction* m_alt_exp_a_sqr_exp_e_vec_Act;
//    QAction* m_alt_exp_exp_a_vec_Act;
    QAction* m_alt_exp_exp_e_vec_Act;
//    QAction* m_alt_exp_exp_a_exp_e_vec_Act;
    QAction* m_exp_mat_Act;
    QAction* m_exp_mat_II_Act;
    QAction* m_exp_exp_e_mat_Act;
    QAction* m_exp_exp_e_mat_II_Act;
    QAction* m_exp_mat_upper_Act;
    QAction* m_exp_mat_II_upper_Act;
    QAction* m_exp_exp_e_mat_upper_Act;
    QAction* m_exp_exp_e_mat_II_upper_Act;
    QAction* m_alt_exp_mat_Act;
    QAction* m_alt_exp_exp_e_mat_Act;
    QAction* m_exp_nonsym_mat_Act;
    QAction* m_exp_exp_e_nonsym_mat_Act;
    QAction* m_alt_exp_nonsym_mat_Act;
    QAction* m_alt_exp_exp_e_nonsym_mat_Act;
    QAction* threept_2var_m_exp_Act;
    QAction* threept_2var_m_exp_exp_e_Act;
    QAction* threept_2var_m_exp_vec_Act;
    QAction* threept_2var_m_exp_exp_e_vec_Act;
    QAction* threept_2var_m_alt_exp_Act;
    QAction* threept_2var_m_alt_exp_exp_e_Act;
    QAction* threept_2var_m_alt_exp_vec_Act;
    QAction* threept_2var_m_alt_exp_exp_e_vec_Act;

    QAction* plot_set_Act;
    QAction* plot_data_Act;
    QAction* plot_all_Act;
    QAction* plot_start_Act;
    QAction* plot_eff_mass_Act;

    QAction* start_to_prior_Act;
    QAction* result_to_start_Act;
    QAction* result_to_prior_Act;
    QAction* chisqr_start_Act;
    QAction* reload_data_file_Act;
    QAction* report_Act;
    QAction* write_results_Act;

    QAction* multifit_Act;

    QAction* aboutAct;

    MainWidget* mainWidget;

    FunctionWidget* funcWidget;
    VariablesWidget* varsWidget;
    ParamDerivsWidget* parderWidget;
    ConstantsWidget* constsWidget;
    ChiSqrExtraTermWidget* chisqrextraWidget;
    FitSettingsWidget* fitsetWidget;

    plot_settings_dialog* plot_set_dialog;

    multi_exp_dialog* m_exp_dialog;
    multi_alt_exp_dialog* m_alt_exp_dialog;
    multi_exp_vec_dialog* m_exp_vec_dialog;
    multi_exp_mat_dialog* m_exp_mat_dialog;
    multi_exp_mat_upper_dialog* m_exp_mat_upper_dialog;
    multi_alt_exp_vec_dialog* m_alt_exp_vec_dialog;
    multi_alt_exp_mat_dialog* m_alt_exp_mat_dialog;
    threept_multi_exp_dialog* threept_m_exp_dialog;
    threept_multi_exp_vec_dialog* threept_m_exp_vec_dialog;
    threept_multi_alt_exp_dialog* threept_m_alt_exp_dialog;
    threept_multi_alt_exp_vec_dialog* threept_m_alt_exp_vec_dialog;

    enum model  // keep existing entries for backward compatibility! UPDATE mbf_load_save!
    {
       PARSE=0,

       MULTIEXP=1,
       MULTIEXPASQR=7,
//       MULTIEXPEXPA=8,
       MULTIEXPEXPE=37,
//       MULTIEXPEXPAEXPE=38,
       MULTIALTEXP=4,
       MULTIALTEXPASQR=9,
//       MULTIALTEXPEXPA=10,
       MULTIALTEXPEXPE=39,
//       MULTIALTEXPEXPAEXPE=40,

       MULTIEXPVEC=2,
       MULTIEXPASQRVEC=17,
//       MULTIEXPEXPAVEC=18,
       MULTIEXPEXPEVEC=41,
//       MULTIEXPEXPAEXPEVEC=42,
       MULTIALTEXPVEC=5,
       MULTIALTEXPASQRVEC=19,
//       MULTIALTEXPEXPAVEC=20,
       MULTIALTEXPEXPEVEC=43,
//       MULTIALTEXPEXPAEXPEVEC=44,

       MULTIEXPMAT=3,
       MULTIEXPEXPEMAT=27,
       MULTIALTEXPMAT=6,
       MULTIALTEXPEXPEMAT=30,

       MULTIEXPBC=45,
       MULTIEXPASQRBC=46,
//       MULTIEXPEXPABC=47,
       MULTIEXPEXPEBC=48,
//       MULTIEXPEXPAEXPEBC=49,
       MULTIALTEXPBC=50,
       MULTIALTEXPASQRBC=51,
//       MULTIALTEXPEXPABC=52,
       MULTIALTEXPEXPEBC=53,
//       MULTIALTEXPEXPAEXPEBC=54,

       MULTIEXPVECBC=55,
       MULTIEXPASQRVECBC=56,
//       MULTIEXPEXPAVECBC=57,
       MULTIEXPEXPEVECBC=58,
//       MULTIEXPEXPAEXPEVECBC=59,
       MULTIALTEXPVECBC=60,
       MULTIALTEXPASQRVECBC=61,
//       MULTIALTEXPEXPAVECBC=62,
       MULTIALTEXPEXPEVECBC=63,
//       MULTIALTEXPEXPAEXPEVECBC=64,

       MULTIEXPASQREXPE=101,
       MULTIEXPASQREXPEBC=102,

       MULTIALTEXPASQREXPE=103,
       MULTIALTEXPASQREXPEBC=104,

       MULTIEXPASQREXPEVEC=105,
       MULTIEXPASQREXPEVECBC=106,

       MULTIALTEXPASQREXPEVEC=107,
       MULTIALTEXPASQREXPEVECBC=108,

       THREEPT2VARMULTIEXP=70,
       THREEPT2VARMULTIALTEXP=75,
       THREEPT2VARMULTIEXPVEC=170,
       THREEPT2VARMULTIALTEXPVEC=175,
       THREEPT2VARMULTIEXPEXPE=180,
       THREEPT2VARMULTIALTEXPEXPE=181,
       THREEPT2VARMULTIEXPEXPEVEC=182,
       THREEPT2VARMULTIALTEXPEXPEVEC=183,

       MULTIEXPNONSYMMAT=303,
       MULTIEXPEXPENONSYMMAT=304,
       MULTIALTEXPNONSYMMAT=305,
       MULTIALTEXPEXPENONSYMMAT=306,

       MULTIEXPCONST=400,
       MULTIEXPASQRCONST=401,
       MULTIEXPEXPECONST=402,
       MULTIALTEXPCONST=403,
       MULTIALTEXPASQRCONST=404,
       MULTIALTEXPEXPECONST=405,
       MULTIEXPVECCONST=406,
       MULTIEXPASQRVECCONST=407,
       MULTIEXPEXPEVECCONST=408,
       MULTIALTEXPVECCONST=409,
       MULTIALTEXPASQRVECCONST=410,
       MULTIALTEXPEXPEVECCONST=411,
       MULTIEXPBCCONST=412,
       MULTIEXPASQRBCCONST=413,
       MULTIEXPEXPEBCCONST=414,
       MULTIALTEXPBCCONST=415,
       MULTIALTEXPASQRBCCONST=416,
       MULTIALTEXPEXPEBCCONST=417,
       MULTIEXPVECBCCONST=418,
       MULTIEXPASQRVECBCCONST=419,
       MULTIEXPEXPEVECBCCONST=420,
       MULTIALTEXPVECBCCONST=421,
       MULTIALTEXPASQRVECBCCONST=422,
       MULTIALTEXPEXPEVECBCCONST=423,
       MULTIEXPASQREXPECONST=424,
       MULTIEXPASQREXPEBCCONST=425,
       MULTIALTEXPASQREXPECONST=426,
       MULTIALTEXPASQREXPEBCCONST=427,
       MULTIEXPASQREXPEVECCONST=428,
       MULTIEXPASQREXPEVECBCCONST=429,
       MULTIALTEXPASQREXPEVECCONST=430,
       MULTIALTEXPASQREXPEVECBCCONST=431,

       MULTIEXPMATII=500,
       MULTIEXPEXPEMATII=501,

       MULTIEXPMATIIUPPER=600,
       MULTIEXPEXPEMATIIUPPER=601,
       MULTIEXPMATUPPER=603,
       MULTIEXPEXPEMATUPPER=627


    }; // UPDATE mbf_load_save!

    model current_model;

    bool something_modified;
    bool need_to_fit;
    bool need_to_load_data_file;
    bool need_to_set_fit_data;
    bool need_to_reset_fitter;

    fitter* _fitter;
    fit_thread* fitthread;
    fit_dialog* fitdialog;
    bootstrap_thread* bootstrapthread;
    bootstrap_dialog* bootstrapdialog;
    multifit_thread* multifitthread;
    multifit_dialog* multifitdialog;
    gaussian_prior* _gaussian_prior;
    chisqr_extra_term* _chisqr_extra_term;
    abstract_model* _model;

    double last_chi_sqr;

    vector< vector< double > > file_arguments;
    vector< vector< vector< double > > > file_data;

    vector< vector< double > > fit_arguments;
    vector< vector< vector< double > > > fit_data;

    vector< vector< vector< double > > > plotting_data;
};

#endif
