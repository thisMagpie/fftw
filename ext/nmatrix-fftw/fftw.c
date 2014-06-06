#include <fftw3.h>
#include <ruby.h>

static VALUE rb_cFFTW3;

void Init_fftw()
{
  rb_cFFTW = rb_define_class("FFTW", rb_cObject);
}
