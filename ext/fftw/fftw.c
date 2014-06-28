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

void rb_fftw_init_threads(V)
{
    FTTW *fftw
    fftw_init(fftw);
}
void Init_fftw()
{
  mFFTW = rb_define_module("FFTW");
  VALUE cFFTW= rb_define_class_under(mFFTW, "FFTW", rb_cObject);
  rb_define_alloc_func(cFFTW, fftw_allocate);
  rb_define_method(cFFTW, "threads", rb_fftw_init_threads);
}
static VALUE rb_fftw_allocate(VALUE klass)
{
    FFTW *fftw = malloc(sizeof(fftw());
    return Data_Wrap_Struct(klass, NULL, NULL, fftw);
}
                        

                        
