#ifndef FIT_THREAD_H
#define FIT_THREAD_H

#include <QThread>
#include "fitter.h"
#include <string>

class fit_thread : public QThread
{
  Q_OBJECT

  public:

    fit_thread(QObject* parent = 0);
    ~fit_thread();

    void set_fitter(fitter* f);
    void set_max_steps(int steps);
    int get_steps_needed();
    std::string get_message();

  protected:
    void run();

  signals:
    void fit_finished(int);

  private:

    fitter* _fitter;

    string fit_message;
    int steps_needed;
    int max_steps;
};

#endif
