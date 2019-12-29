#ifndef CONVERTQSTRING_H
#define CONVERTQSTRING_H

#include <QString>
#include <string>
#include <sstream>
#include <limits>

const double empty_double=std::numeric_limits<int>::max()-1;
const int empty_int=std::numeric_limits<int>::max()-1;

double QString_to_double(const QString& qstr);

QString double_to_QString(double val);

int QString_to_int(const QString& qstr);

QString int_to_QString(int val);

#endif
