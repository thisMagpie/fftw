#ifdef __FFTW_H__
#include __FFTW_H__
#endif
#include "fftw.h"
#include "ruby.h"
#include <fftw3.h>
#include <stdio.h>
#include <iostream>

#ifdef __NMATRIX_H__
  #include __NMATRIX_H__
#endif

using namespace std;
/* The guru interface introduces one basic new data structure
   fftw_iodim, that is used to specify sizes and strides for
   multi-dimensional transforms and vectors
   http://www.fftw.org/doc/Guru-vector-and-transform-sizes.html
   */
struct fftw {
    int n;
    int is;
    int os;
    size_t size;
    void *ptr;
};

/*
  fftw_free
  @param *p: 
  pointer reference to object
  that is taking up space which
  needs to be freed.*/
static void nm_free(void *p)
{
  struct fftw *ptr = p;

  if (ptr->size > 0)
      free(ptr->ptr);
  }
};

/*
 * fftw_alloc: function to llocate memory taken
                by an fftw object.
   @param klass: pointer reference to object
                 that is taking up space which
                 needs to be freed. */
VALUE
fftw_alloc(VALUE klass)
{
  struct fftw *ptr;
  obj = Data_Make_Struct(klass,
                         struct fftw,
                         NULL,
                         fftw_free,
                         ptr);
  ptr->size = 0;
  ptr->ptr  = NULL;
  return obj;

}

static VALUE
fftw_init(VALUE self, VALUE size) {
struct fftw *ptr;
size_t requested = NUM2SIZET(size);

if (0 == requested)
    rb_raise(rb_eArgError, "unable to allocate 0 bytes");

Data_Get_Struct(self, struct fftw, ptr);

ptr->ptr = malloc(requested);

if (NULL == ptr->ptr)
    rb_raise(rb_eNoMemError, "unable to allocate %ld bytes", requested);

ptr->size = requested;

return self;
}

VALUE
fftw_release(VALUE self)
{
  struct fftw *ptr;
  Data_Get_Struct(self, struct fftw, ptr);

  if (0 == ptr->size)
      return self;

  ptr->size = 0;
  free(ptr->ptr);

  return self;
}

#ifdef __cplusplus
  extern "C"
{
void
Init_fftw(void)
  {
    VALUE mFFTW_NMatrix;
    VALUE cFFTW;
    mFFTW_NMatrix = rb_define_module("NMatrix");
    cFFTW = rb_define_module_under(mFFTW_NMatrix, "FFTW");
    rb_define_singleton_method(cFFTW, "initialize", fftw_init, -1);
    rb_define_singleton_method(cFFTW, "alloc", fftw_alloc, 0);
    rb_define_singleton_method(cFFTW, "free", fftw_free, 0);
    rb_define_singleton_method(cFFTW, "release", fftw_release, 0);
  }
}
 /* extern "C" */
#endif /* __cplusplus */