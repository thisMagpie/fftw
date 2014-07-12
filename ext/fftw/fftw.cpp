

#include "ruby.h"
#include <fftw3.h>
#include <stdio.h>
#include <fftw.h>
#include <iostream>
#include "fftw_config.h"

VALUE mFFTW;
VALUE cFFTW;

fftw_complex *fftw_complex_alloc(long n);

using namespace std;

#ifdef __cplusplus
extern "C" {
  typedef struct fftw
  {
    long n;
    VALUE size;
    fftw_complex fc;
    fftw_complex out;
    VALUE nmatrix;
    VALUE ftw_r2c;
  } FFTW;

  static void fftw_nm_free(void * nmatrix){
    fftw_complex* fc;

    free(((FFTW *)nmatrix)->fc);
    free(nmatrix);
  }

  VALUE
  fftw_alloc(VALUE klass)
  {
    long n;
    fftw_complex* fc;

    FFTW *nmatrix;
    nmatrix = ALLOC(FFTW);

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


     plan = fftw_plan_dft_r2c(2,
                              nm,
                              in,
                              (fftw_complex*)in,
                              FFTW_MEASURE
                              );
    /* TODO add plan */
    return self;
  }

  void
  Init_fftw(void)
  {
    rb_define_module("FFTW");
    rb_define_singleton_method(mFFTW, "ifftw", fftw_complex, 1);
    rb_define_singleton_method(mFFTW, "r2c", fftw_r2c, 1);
    rb_define_singleton_method(mFFTW, "alloc", fftw_alloc, 0);
    rb_define_singleton_method(mFFTW, "free", fftw_free, 0);
  }
}