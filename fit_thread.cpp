#include "fit_thread.h"

fit_thread::fit_thread(QObject* parent)
{
  max_steps = 100;
  steps_needed = 100;
  fit_message="";
}

fit_thread::~fit_thread()
{
}

void fit_thread::set_fitter(fitter* f)
{
  _fitter = f;
}

void fit_thread::set_max_steps(int steps)
{
  max_steps = steps;
}

int fit_thread::get_steps_needed()
{
  return steps_needed;
}


std::string fit_thread::get_message()
{
  return fit_message;
}


void fit_thread::run()
{
  steps_needed = _fitter -> fit(max_steps, fit_message);
  emit fit_finished(0);
}
