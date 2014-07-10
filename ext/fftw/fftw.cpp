#ifdef __FFTW_H__
#include __FFTW_H__
#endif
#include "fftw.h"
#include "ruby.h"
#include <fftw3.h>

#ifdef __NMATRIX_H__
  #include __NMATRIX_H__
#endif

VALUE mFFTW;
VALUE cFFTW;

/*! fftw_malloc only ever needs to be used for real and
    complex arrays. Two convenient wrapper routines:

      1. fftw_alloc_real(N)
      2. fftw_alloc_complex(N)

    1. and 2. are equivalent to:
      (double*)fftw_malloc(sizeof(double) * N)
    More information:
  <http://www.fftw.org/doc/SIMD-alignment-and-fftw_005fmalloc.html> */

struct fftw
{
  size_t size;
  void *ptr;
};

 /*! fftw_free
  @param *p: pointer reference to object
              that is taking up space which
              needs to be freed. */
void
fftw_free(void *p)
{
  struct fftw *ptr;

  /*! Check pointer reference has size > 0
     If so, detach pointer from referenced object
     to free up space. */
  if (ptr->size > 0)
    free(ptr->ptr);
}

/**
*

 //! fftw_alloc: function to llocate memory taken
                by an fftw object.
   @param klass: pointer reference to object
                 that is taking up space which
                 needs to be freed. */
static VALUE
fftw_alloc(VALUE klass)
{
  VALUE obj;
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

VALUE
fftw_init(VALUE self, VALUE size)
{
  struct fftw *ptr;
  size_t requested = NUM2SIZET(size);
  rb_const_get(rb_cObject, rb_intern("FFTW"));
  Data_Get_Struct(self, struct fftw, ptr);

  ptr->ptr = fftw_malloc(requested);

  if (0 == requested)
    rb_raise(rb_eArgError, "unable to allocate 0 bytes");
  ptr->ptr = fftw_malloc(requested);

  if (NULL == ptr->ptr)
      rb_raise(rb_eNoMemError, "Unable to allocate %ld bytes", requested);

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

void
Init_fftw(void)
{
  mFFTW = rb_define_module("FFTW");
  cFFTW = rb_const_get(rb_cObject, rb_intern("FFTW"));

  rb_define_alloc_func(cFFTW, fftw_alloc );
  rb_define_singleton_method(mFFTW, "initialize", fftw_init,1);
  rb_define_singleton_method(mFFTW, "free", fftw_free, 0);
  rb_define_singleton_method(mFFTW, "release", fftw_release, 0);
}
int main (int argc, char **argv)
{
  void Init_fftw();
}