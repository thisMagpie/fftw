#include "ruby.h"
#include <fftw3.h>
#include <fftw.h>
#include <stdio.h>

struct fftw {
  long n;
  VALUE size;
  fftw_complex fc;
  fftw_complex out;
  VALUE nmatrix;
  VALUE ftw_r2c;
} FFTW;

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


  plan = fftw_plan_dft_r2c(self, nm, in, out, (const int) FFTW_ESTIMATE);
  /* TODO add plan */

  fftw_execute(plan);
  free(in);
  fftw_destroy_plan(plan);
  return self;
}

void
Init_fftw(void)
{

  VALUE cFFTW;
  cFFTW = rb_define_class("FFTW", cFFTW);
  rb_define_method(cFFTW, "ifft", fftw_complex, 0);
  rb_define_method(cFFTW, "r2c", fftw_r2c, 1);
  rb_define_method(cFFTW, "alloc", fftw_malloc, 0);
  rb_define_method(cFFTW, "free", fftw_free, 0);
}
