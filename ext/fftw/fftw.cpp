#include "ruby.h"
#include <fftw.h>
#include <fftw_config.h>
#include <stdio.h>
#include <iostream>
#include <fftw3.h>
using namespace std;

VALUE mFFTW;
VALUE cFFTW;

void fftw_print_nmatrix(int nmatrix[], int rows, int columns);

template<int rows, int columns>
void fftw_print_nmatrix(int (&nmatrix)[rows][columns])
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      cout << nmatrix[i][j] << " ";
    }
    cout << endl;
  }
}

/* From https://github.com/ruby/ruby/blob/trunk/include/ruby/ruby.h */
#if defined(cplusplus)
extern "C"
{
#endif

void fftw_1d(unsigned long n,
             fftw_complex *f,
             fftw_complex *fhat,
             int direction)
{
  fftw_plan plan = fftw_plan_dft_1d(n,f,fhat,
                                 direction<0?FFTW_BACKWARD:FFTW_FORWARD,
                                 FFTW_ESTIMATE
                                 );

  fftw_execute(plan);
  fftw_destroy_plan(plan);
  fftw_free(plan);

}

/**
  fftw_r2c
  @param self
  @param nm
  @return self

  With FFTW_ESTIMATE as a flag in the plan,
  the input and and output are not overwritten at runtime
  The plan will use a heuristic approach to picking plans
  rather than take measurements
*/
static VALUE
#ifdef FFTW3_HAS_SINGLE_SUPPORT
fftw_r2c_double(VALUE self, VALUE nmatrix)
  /* called by fftw_r2c */
#else
fftw_r2c(VALUE self, VALUE nmatrix)
  /* called directly */
#endif
{
  // Note that "defining" the NMatrix class will just retrieve it if it exists
  VALUE cNMatrix = rb_define_class("NMatrix", rb_cObject);
  // shape is a ruby array, e.g. [2, 2] for a 2x2 matrix
  VALUE shape = rb_funcall(nmatrix, rb_intern("shape"), 0);
  // size is the number of elements stored for a matrix with dimensions = shape
  int size = NUM2INT(rb_funcall(cNMatrix, rb_intern("size"), 1, shape));
  //Input: a 1D double array with enough elements for the whole matrix
  double* in = ALLOC_N(double, size);

  // This would need to be a nested loop for multidimensional matrices, or it
  // would need to use the size instead of the shape and figure out the indices
  // to pass to [] appropriately from that.
  for (int i = 0; i < FIX2INT(rb_ary_entry(shape, 0)); i++) {
      in[i] = NUM2DBL(rb_funcall(nmatrix, rb_intern("[]"), 1, INT2FIX(i)));
  }
  // Actual fourier transform stuff would go here.

  xfree(in);
  return self;
}

 //http://banisterfiend.wordpress.com/2008/10/06/metaprogramming-in-the-ruby-c-api-part-two-dynamic-methods/
VALUE fftw_missing(int argc, VALUE *argv, VALUE self)
{
  VALUE name;
  name = rb_funcall(rb_funcall(*argv,
                               rb_intern("to_s"),
                               0),
                    rb_intern("downcase"),0);

  if(rb_respond_to(self, rb_to_id(name)))
  {
    rb_funcall2(self, rb_to_id(name), --argc, ++argv);
  }
  else
  {
    rb_raise (rb_eNotImpError, "no such fftw: %s", StringValuePtr(name));
  }
  return Qnil;
}

void Init_fftw(void)
{
  mFFTW = rb_define_module("FFTW");

  #ifdef FFTW3_HAS_SINGLE_SUPPORT
    rb_define_singleton_method(mFFTW, "r2c",
                            (VALUE (*)(...)) fftw_r2c_double,
                             1);
  #else
    rb_define_singleton_method(mFFTW, "r2c",
                            (VALUE (*)(...)) fftw_r2c,
                             1);
  #endif

  rb_define_singleton_method(mFFTW, "v",
                            (VALUE (*)(...)) fftw_1d,
                             4);
  rb_define_singleton_method(mFFTW,
                             "missing",
                             (VALUE (*)(...)) fftw_missing,
                             -1);
}
#if defined(cplusplus)
}
#endif
