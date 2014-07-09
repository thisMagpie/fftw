#ifdef __FFTW_H__
#include __FFTW_H__
#endif
#include "fftw.h"

struct fftw
{
  size_t size;
  void *ptr;
};

/**
 * Free up memory taken up ßfrom fftw instance.
 */
void
fftw_free(void *p)
{
  struct fftw *ptr;

  if (ptr->size > 0)
    free(ptr->ptr);
}

/**
 * Allocate memory taken up ßfrom fftw instance.
 */

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

  ptr->ptr = malloc(requested);


  if (0 == requested)
    rb_raise(rb_eArgError, "unable to allocate 0 bytes");
  ptr->ptr = malloc(requested);

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
Init_fftw (void)
{
  mFFTW = rb_define_module("FFTW");
  cFFTW = rb_const_get(rb_cObject, rb_intern("FFTW"));

  rb_define_alloc_func(cFFTW, fftw_alloc);
  rb_define_singleton_method(mFFTW, "initialize", fftw_init, 1);
  rb_define_singleton_method(mFFTW, "free", fftw_release, 0);
}