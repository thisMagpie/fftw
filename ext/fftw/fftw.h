#ifndef FFTW_H
#define FFTW_H

#include <ruby.h>
#include <ifftw.h>
#include <fftw3.h>

extern VALUE mFFTW;
extern VALUE cFFTW;

void Init_fftw();
static VALUE ffw_allocate();
static VALUE fftw_initialize();
#endif
