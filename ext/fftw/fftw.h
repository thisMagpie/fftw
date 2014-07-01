#ifndef FFTW_H
#define FFTW_H

#include <ruby.h>
#include <fftw3.h>

#ifdef HAVE_NMATRIX_H
#include "nmatrix.h"
#endif

extern VALUE mFFTW;

void Init_fftw();
#endif
