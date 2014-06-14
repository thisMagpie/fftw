#include "fftw3.h"
#include <ruby.h>

VALUE cNMatrixFFTW;
VALUE rb_fftw_1d(int argc, VALUE* argv, VALUE self);

<<<<<<< HEAD
fftw_plan_r2r_1d(int n, double *in, double *out,
                                fftw_r2r_kind kind, unsigned flags);
void Init_fftw()
=======
void Init_nmatrix_fftw()
>>>>>>> 6ad096235c5f672e7914912f518c5ae1ca3cd9f1
{
  cNMatrixFFTW = rb_define_class("NMatrixFFTW", rb_cObject);
  rb_define_method(cNMatrixFFTW, "fftw_1d", rb_fftw_1d, -1);
}
