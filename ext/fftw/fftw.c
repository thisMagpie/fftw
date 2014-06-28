#include <fftw3.h>
#include <fftw_threads.h>
#include "ruby.h"
#include <fftw_mpi.h>

VALUE mFFTW;
VALUE cFFTW;
VALUE rb_fftw(int argc, VALUE* argv, VALUE self);

#define NM_DTYPE(val)           (NM_STORAGE(val)->dtype)
#define NM_SHAPE(val,i)         (NM_STORAGE(val)->shape[(i)])
#define NM_SHAPE0(val)          (NM_STORAGE(val)->shape[0])
#define NM_SHAPE1(val)          (NM_STORAGE(val)->shape[1])

void rb_fftw_init_threads()

void Init_fftw()
{
  mFFTW = rb_define_module("FFTW");
  cFFTW = rb_define_class("FFTW", rb_cObject);
  rb_define_method(cFFTW, "threads", rb_fftw_init_threads, -1);
}
