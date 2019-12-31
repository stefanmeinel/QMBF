#ifndef FITSETTINGSWIDGET_H
#define FITSETTINGSWIDGET_H

#include <QtWidgets>

#include <iostream>
#include <fstream>

#include <gsl/gsl_rng.h>

#include "convert_qstring.h"
#include "fitter.h"
#include "newbootfiledialog.h"

class FitSettingsWidget: public QWidget
{
  Q_OBJECT

  public:
    FitSettingsWidget(bool bayes, bool num_diff, bool second_deriv_minimization, bool second_deriv_covariance, double num_diff_step, bool use_bse, bool restrict_data, int startdata, int stopdata, double startlambda, double lambdafac, double tolerance,
                      int svd, double svd_ratio, double svd_value, double rescale_value, int steps, int bin, int bssamples, QWidget* parent=0);

    void set_bayes(bool);
    bool get_bayes();

    void set_use_bse(bool);
    bool get_use_bse();

    void set_boot_prior(bool);
    bool get_boot_prior();

    void set_restrict_data(bool);
    bool get_restrict_data();

    void set_data_start(int);
    int get_data_start();

    void set_data_stop(int);
    int get_data_stop();

    void set_num_diff(bool);
    bool get_num_diff();

    void set_second_deriv_minimization(bool);
    bool get_second_deriv_minimization();

    void set_second_deriv_covariance(bool);
    bool get_second_deriv_covariance();

    void set_num_diff_step(double);
    double get_num_diff_step();

    void set_startlambda(double);
    double get_startlambda();

    void set_lambdafac(double);
    double get_lambdafac();

    void set_tolerance(double);
    double get_tolerance();

    void set_inversion_method(inversion_method);
    inversion_method get_inversion_method();

    void set_cov_normalization(cov_normalization);
    cov_normalization get_cov_normalization();

    void set_svd(int);
    int get_svd();

    void set_svd_ratio(double);
    double get_svd_ratio();

    void set_svd_value(double);
    double get_svd_value();

    void set_rescale_value(double);
    double get_rescale_value();

    void set_steps(int);
    int get_steps();

    void set_bin(int);
    int get_bin();

    void set_bssamples(int);
    int get_bssamples();

    std::string get_boot_file();
    void set_boot_file(std::string name);

    void reset();

  signals:
    void modified();
    void bayesianmodified(int newState);
    void bin_modified(int newsize);
    void svd_modified();
    void cov_normalization_modified();
    void datarange_modified();
    void numdiffmodified(int newState);
    void secondderivminimizationmodified(int newState);
    void secondderivcovariancemodified(int newState);

  private slots:
    void modified_slot();
    void bayesianmodified_slot(int newState);
    void usebsemodified_slot(int newState);
    void restrictdata_modified_slot(int newState);
    void datarange_modified_slot();
    void bin_modified_slot(int newsize);
    void svd_modified_slot();
    void inv_method_modified_slot(int newindex);
    void cov_normalization_modified_slot(int newindex);
    void select_bootfile();
    void new_bootfile();
    void numdiffmodified_slot(int newState);
    void secondderivminimizationmodified_slot(int newState);
    void secondderivcovariancemodified_slot(int newState);

  private:
    void createGadgets();

    QHBoxLayout* hlayout1;
    QHBoxLayout* hlayout2;
    QHBoxLayout* hlayout3;
    QGridLayout* layout;


    QCheckBox* bayes_cb;

    QCheckBox* num_diff_cb;
    QCheckBox* second_deriv_minimization_cb;
    QCheckBox* second_deriv_covariance_cb;

    QLineEdit* numdiffEd;
    QLabel* numdiffLb;

    QCheckBox* boot_prior_cb;

    QCheckBox* use_bse_cb;

    QCheckBox* restrict_data_cb;

    QLineEdit* startdataEd;
    QLabel* startdataLb;

    QLineEdit* stopdataEd;
    QLabel* stopdataLb;

    QLineEdit* startlambdaEd;
    QLabel* startlambdaLb;

    QLineEdit* lambdafacEd;
    QLabel* lambdafacLb;

    QLineEdit* toleranceEd;
    QLabel* toleranceLb;

    QLabel* invMethodLb;
    QComboBox* invMethodCb;

    QLabel* covNormalizationLb;
    QComboBox* covNormalizationCb;

    QLineEdit* svdEd;
    QLabel* svdLb;

    QLineEdit* svdRatioEd;
    QLabel* svdRatioLb;

    QLineEdit* svdValueEd;
    QLabel* svdValueLb;

    QLineEdit* rescaleValueEd;
    QLabel* rescaleValueLb;

    QLineEdit* stepsEd;
    QLabel* stepsLb;

    QSpinBox* binningSB;
    QLabel* binningLb;

    QLineEdit* bssamplesEd;
    QLabel* bssamplesLb;

    QLabel* bootfileLb;
    QLineEdit* bootfileEd;
    QToolButton* bootfilebutton;
    QToolButton* newbootfilebutton;

    bool start_bayes;
    bool start_num_diff;
    bool start_second_deriv_minimization;
    bool start_second_deriv_covariance;
    double start_num_diff_step;
    bool start_use_bse;
    bool start_restrict_data;
    double startlambda0;
    double lambdafac0;
    double tolerance0;
    int svd0;
    double svd_ratio0;
    double svd_value0;
    double rescale_value0;
    int steps0;
    int bin0;
    int bssamples0;
    int startdata0;
    int stopdata0;
};

#endif
