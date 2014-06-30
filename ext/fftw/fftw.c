#include <fftw3.h>
#include <ruby.h>

VALUE mFFTW;

static VALUE fftw(int argc, VALUE* argv, VALUE self);

void Init_fftw()
{
  mFFTW = rb_define_module("FFTW");
}
