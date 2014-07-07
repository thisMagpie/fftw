#ifndef FFTW_H
#define FFTW_H

#include "ruby.h"
#include <fftw3.h>
#include "fftw.h"
#include "nmatrix.h"

VALUE mFFTW;
extern VALUE cNMatrix;
void Init_nm_fftw();
#endif
