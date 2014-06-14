#include <fftw3.h>
#include <ruby.h>

VALUE cNMatrixFFTW;
VALUE rb_fftw_1d(int argc, VALUE* argv, VALUE self);

void Init_nmatrix_fftw()
{
  cNMatrixFFTW = rb_define_class("NMatrixFFTW", rb_cObject);
  rb_define_method(cNMatrixFFTW, "fftw_1d", rb_fftw_1d, -1);
}
