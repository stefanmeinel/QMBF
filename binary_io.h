#ifndef BINARY_IO_H
#define BINARY_IO_H

#include <fstream>
#include <cstdio>
#include <cstring>

// open file as follows: ofstream st(filename, ios::out | ios::binary);
void write_double(std::ofstream& st, double* dptr);
void write_float(std::ofstream& st, float* fptr);

// open file as follows: ifstream st(filename, ios::in | ios::binary);
double read_double(std::ifstream& st);
float read_float(std::ifstream& st);

#endif
