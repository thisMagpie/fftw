#ifndef __FFTW_H__
#define __FFTW_H__
#endif

#include "ruby.h"
#include <fftw3.h>
#ifdef __NMATRIX_H__
#include __NMATRIX_H__
#endif
VALUE mFFTW;
VALUE cFFTW;
void Init_fftw_nm();
