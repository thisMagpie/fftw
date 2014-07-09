#ifdef __FFTW_H__
#include __FFTW_H__
#endif
#include "fftw.h"

struct fftw_nm
{
  size_t size;
  void *ptr;
};

/**
 * Free up memory taken up ßfrom fftw_nm instance.
 */
static void
fftw_nm_free(void *p)
{
  struct fftw_nm *ptr;

  if (ptr->size > 0)
    free(ptr->ptr);
}

/**
 * Allocate memory taken up ßfrom fftw_nm instance.
 */

static VALUE
fftw_nm_alloc(VALUE klass)
{
  VALUE obj;
  struct fftw_nm *ptr;
  obj = Data_Make_Struct(klass,
                         struct fftw_nm,
                         NULL,
                         fftw_nm_free,
                         ptr);
  ptr->size = 0;
  ptr->ptr  = NULL;
  return obj;
}

static VALUE
fftw_nm_init(VALUE self, VALUE size)
{
  struct fftw_nm *ptr;
  size_t requested = NUM2SIZET(size);
  rb_const_get(rb_cObject, rb_intern("FFTW"));
  Data_Get_Struct(self, struct fftw_nm, ptr);

  ptr->ptr = malloc(requested);


  if (0 == requested)
    rb_raise(rb_eArgError, "unable to allocate 0 bytes");
  ptr->ptr = malloc(requested);

  if (NULL == ptr->ptr)
      rb_raise(rb_eNoMemError, "Unable to allocate %ld bytes", requested);

  ptr->size = requested;

  return self;
}

static VALUE
fftw_nm_release(VALUE self)
{
  struct fftw_nm *ptr;

  Data_Get_Struct(self, struct fftw_nm, ptr);

  if (0 == ptr->size)
      return self;

  ptr->size = 0;
  free(ptr->ptr);

  return self;
}
void
Init_fftw_nm (void)
{
  mFFTW = rb_define_module("FFTW");
  cFFTW = rb_const_get(rb_cObject, rb_intern("FFTW"));
}