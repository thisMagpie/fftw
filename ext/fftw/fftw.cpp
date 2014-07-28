#include "ruby.h"
#include <fftw3.h>
#include <fftw.h>
#include <fftw_config.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
using namespace std;

/* From https://github.com/ruby/ruby/blob/trunk/include/ruby/ruby.h */
#if defined(cplusplus)
extern "C"
{
#endif

VALUE mFFTW;
VALUE cNMatrix;

void fftw_1d(unsigned long n,
             fftw_complex *f,
             fftw_complex *fhat,
             int direction)
{
  fftw_plan p = fftw_plan_dft_1d(n,f,fhat,
                                 direction<0?FFTW_BACKWARD:FFTW_FORWARD,
                                 FFTW_ESTIMATE
                                 );

  fftw_execute(p);
  fftw_destroy_plan(p);
}

/**
  fftw_r2c
  @param self
  @param nm
  @return

  With FFTW_ESTIMATE as a flag in the plan,
  the input and and output are not overwritten at runtime
  The plan will use a heuristic approach to picking plans
  rather than take measurements
*/

static VALUE
#ifdef FFTW3_HAS_SINGLE_SUPPORT
fftw_r2c_double(int argc, VALUE *argv, VALUE self)
  /* called by fftw_r2c */
#else
fftw_r2c(int argc, VALUE *argv, VALUE self)
  /* called directly */
#endif
{
  VALUE nm, direction;
  int rank = argc - 2;
  const int *n;
  double *in;
  int i, j;
  double nmatrix;
  fftw_complex *out;
  fftw_plan plan;

  for(i = 0; i < rank; i++)
  {
    for(j = 0; j < rank; j++)
    {
      nmatrix<i,j>= NUM2DBL(rb_funcall(argv[i], rb_intern( "[]" ), j ));
      printf("%g",&nmatrix);
    }
  }

  //In place input
  in = (double*)malloc(sizeof(double)*(self * self));
  out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) *  self * (self/2 + 1));
  plan = fftw_plan_dft_r2c(rank, n, in, out, FFTW_ESTIMATE); //estimate is rough
  fftw_execute(plan);
  free(in);
  fftw_destroy_plan(plan);
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
  rb_global_variable(&mFFTW);
  cNMatrix = rb_define_class ("NMatrix",mFFTW);
  rb_define_singleton_method(cNMatrix, "r2c",
                             (VALUE (*)(...)) fftw_r2c,
                             -1);
  rb_define_singleton_method(cNMatrix,
                             "missing",
                             (VALUE (*)(...)) fftw_missing,
                             -1);

}
#if defined(cplusplus)
}
#endif
