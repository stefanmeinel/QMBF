#include "convert_qstring.h"

double QString_to_double(const QString& qstr)
{
  if(qstr.toStdString().empty())
  {
    return empty_double;
  }
  else
  {
    std::stringstream sstr;
    sstr << qstr.toStdString();
    double val;
    sstr >> val;
    return val;
  }
}


QString double_to_QString(double val)
{
  std::stringstream sstr;
  sstr.precision(15);
  sstr << val;
  return QString(sstr.str().c_str());
}


int QString_to_int(const QString& qstr)
{
  if(qstr.toStdString().empty())
  {
    return empty_int;
  }
  else
  {
    std::stringstream sstr;
    sstr << qstr.toStdString();
    int val;
    sstr >> val;
    return val;
  }
}


QString int_to_QString(int val)
{
  std::stringstream sstr;
  sstr << val;
  return QString(sstr.str().c_str());
}
