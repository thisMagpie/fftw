#include <fftw3.h>
#include <ruby.h>

VALUE mFFTW;

void Init_fftw()
{
  rb_define_module("FFTW");
}
