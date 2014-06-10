#include "fftw3.h"
#include <ruby.h>

VALUE mFFTW;

fftw_plan_r2r_1d(int n, double *in, double *out,
                                fftw_r2r_kind kind, unsigned flags);
void Init_fftw()
{
  rb_define_module("FFTW");
}
