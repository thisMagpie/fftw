

#include "ruby.h"
#include <fftw3.h>
#include <stdio.h>
#include <iostream>
#include <fftw.hpp>
#include "fftw_config.h"

#ifdef __NMATRIX_H__
  #include __NMATRIX_H__
  VALUE cNMatrix;
#endif

VALUE mFFTW;
VALUE cFFTW;

fftw_complex *fftw_complex_alloc(long n);

using namespace std;

/* The guru interface introduces one basic new nmatrix structure
   fftw_iodim, that is used to specify sizes and strides for
   multi-dimensional transforms and vectors
   http://www.fftw.org/doc/Guru-vector-and-transform-sizes.html
   The structure contains member functions of FFTW
   */
typedef struct fftw_nm{
  long n;
  VALUE size;
  fftw_complex fc;
  fftw_complex out;
  VALUE nmatrix;
} FFTW;

static void fftw_nm_free(void * nmatrix){
  fftw_complex* fc;

  free(((FFTW *)nmatrix)->fc);
  free(nmatrix);
}

VALUE
fftw_alloc(VALUE klass){
  long n;
  fftw_complex* fc;

  FFTW *nmatrix;
  nmatrix = ALLOC(FFTW);
  nmatrix->n = n;
  (FFTW *)nmatrix->fc = fc;
  return Data_Wrap_Struct(klass, 0, free, nmatrix);
}

VALUE fftw_r2c(int argc, VALUE* argv, VALUE self) {
  VALUE nmatrix, opts, size;
  double *in;
  fftw_complex out;
  fftw_plan plan;
  long n;

  rb_scan_args(argc, argv, "11", &nmatrix, &opts);
  in = (double *) fftw_malloc(sizeof(double) * n);  
  /*out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (n/2+1)); */
  /* TODO add plan */
  return self;
}

void
Init_fftw(void)
{
  long fftw_size(VALUE v);
  rb_define_singleton_method(mFFTW, "fft_complex", fftw_complex, 1);
  rb_define_singleton_method(mFFTW, "r2c", fftw_r2c, -1);
  rb_define_singleton_method(mFFTW, "alloc", fftw_alloc, 0);
  rb_define_singleton_method(mFFTW, "free", fftw_free, 0);
}