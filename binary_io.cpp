#include "binary_io.h"

void write_double(std::ofstream& st, double* dptr)
{
  char buffer[8];
  memcpy(buffer, dptr, 8);
  st.write(buffer, 8);
}

void write_float(std::ofstream& st, float* fptr)
{
  char buffer[4];
  memcpy(buffer, fptr, 4);
  st.write(buffer, 4);
}

double read_double(std::ifstream& st)
{
  char buffer[8];
  st.read(buffer, 8);
  double result;
  memcpy(&result, buffer, 8);
  return result;
}

float read_float(std::ifstream& st)
{
  char buffer[4];
  st.read(buffer, 4);
  float result;
  memcpy(&result, buffer, 4);
  return result;
}
