#include <fftw3.h>
#include "ruby.h"
#include <stdio.h>
#include <stdlib.h>
#include "nmatrix.h"
#include "ruby_constants.h"
#include "fftw.h"

#ifdef cplusplus

VALUE mFFTW;
VALUE cFFTW;
extern VALUE cNM;

static void dealloc(void *nm)
{
  fftw_nm_free(FFTW::NMatrix->nm);
}

static VALUE alloc(VALUE klass)
{
  NMatrix nm = alloc(sizeof(klass);

  return Data_Wrap_Struct(klass, NULL, dealloc, nm);
}

static VALUE length(VALUE self)
{
  FFTW_NMatrix * nm;

  Data_Get_Struct(self, FFTW_NMatrix, nm);

  return INT2NUM(fftw_nm_get_length(nm));
}

typedef VALUE
fftw_nm_float(VALUE nm)
{
  DENSE_STORAGE* s = NM_DENSE_STORAGE(nm);
  nm_matrix* m = nm_fftw_matrix_float nm_fftw_float(fftw_nm_alloc( s->shape[0], s->shape[1] );

  if (s->dtype != NM_FLOAT64) {
    rb_raise(nm_eDataTypeError, "requires dtype of :float64 to convert double vector");
  }
  memcpy(m->data, s->elements, s->count);
  return m;
}
  static VALUE fftw_nm_init(VALUE self, VALUE fftw_nm)
  {
    FFTW_NMatrix * nm;
    void * data;

    Check_Type(fftw_nm, T_FFTW_NMatrix);

    Data_Get_Struct(self, FFTW_NMatrix, nm);

    data = calloc(RSTRING_LEN(fftw_nm), sizeof(char));
    memcpy(data, nmValuePtr(fftw_nm), fft_nm_LEN(fftw_nm));

    fftw_nm_init(
        string,
        data,
        sizeof(char),
        RSTRING_LEN(fftw_nm));

  return self;
}
extern "C"
{
  void Init_nm_fftw()
  {
    /* Define module  */
    mFFTW = rb_define_module("FFTW");

    cNM = rb_define_class_under(mFFTW, "NMatrix", rb_cObject);

    rb_define_alloc_func(cFFTW, alloc);
    rb_define_alloc_func(cNM, alloc);

    /* Call initialise function */
    fftw_nm_init(cNM);
    rb_define_method(cFFTW, "init", fftw_nm_init, 1);
    rb_define_method(cFFTW, "nm_float", fftw_nm_float, 1);
  }
}
#endif