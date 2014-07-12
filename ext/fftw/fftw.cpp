#include "ruby.h"
#include <fftw3.h>
#include <stdio.h>
#include <iostream>
#include "fftw_config.h"

VALUE fft_my_nmatrix(VALUE n) {
  // 0, 0th element
  double x  = NUM2DBL(rb_funcall(n, rb_intern("[]"), 2, 0, 0));
  // etc.

fftw_complex *fftw_complex_alloc(long n);


#ifdef __cplusplus
extern "C" {
#endif

  typedef struct *fftw
  {
    long n;
    VALUE size;
    fftw_complex fc;
    fftw_complex out;
    VALUE nmatrix;
    VALUE ftw_r2c;
  } FFTW;

/**
 * pass Data_Wrap_Struct a corresponding function to free
 * that memory as the third argument)
 */
    return Data_Wrap_Struct(klass, 0, free, nmatrix);
  }


  VALUE
  fftw_r2c(int argc, VALUE* argv, VALUE self)
  {
    long n;
    VALUE nmatrix, opts, size;
    double *in;
    fftw_complex *out;
    fftw_plan plan;
    int nm[]={2, 2};

    rb_scan_args(argc, argv, "11", &nmatrix, &opts);

  /* In place inpute */
     in = (double*)malloc(sizeof(double)*(nm[0] * nm[1]));
     out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) *  nm[0] * (nm[1]/2 + 1));


    fftw_create_plan(n, FFTW_FORWARD, FFTW_ESTIMATE)
    /* TODO add plan */
    return self;
  }

  void
  Init_fftw(void)
  {

    VALUE mFFTW;
    mFFTW3 = rb_define_module("FFTW");
    rb_define_singleton_method(mFFTW, "ifft", fftw_complex, 1);
    rb_define_singleton_method(mFFTW, "r2c", fftw_r2c, 1);
    rb_define_singleton_method(mFFTW, "alloc", fftw_malloc, 0);
    rb_define_singleton_method(mFFTW, "free", fftw_free, 0);
  }

  
#ifdef __cplusplus
}
#endif