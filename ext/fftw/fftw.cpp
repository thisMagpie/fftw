#include "ruby.h"
#include <fftw3.h>
#include <fftw.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
using namespace std;

// From https://github.com/ruby/ruby/blob/trunk/include/ruby/ruby.h
#if defined(cplusplus)
extern "C"
{ /* extern "C" { */
#endif

// VALUE fftw_nmatrix(int argc, VALUE* argv, VALUE self)
// {
//   VALUE in;
//   VALUE nmatrix, size;

//   rb_funcall(rb_funcall(*argv, rb_intern("to_s"), 0),
//              rb_intern("downcase"),0);

//   rb_scan_args(argc, argv, "11", &nmatrix, &size);

//   if (NIL_P(size))        // if no size was given...
//     size = INT2NUM(0);  // use the default value

//   rb_iv_set(self, "in", in);

//   //for (int i = 0; i< length; i++){ 
//   //literal constructor  == []
//   // double nm = NUM2DBL(rb_funcall(args, rb_intern("[]") 2));
//   //}

//  // rb_yield(Qnil);
//   printf("TEST\n");
//   return 0;
// }

// Prototype for ruby to call

VALUE mFFTW;
VALUE cNMatrix;
int i = 0;

void Init_fftw(void);

VALUE fftw_r2c(VALUE self, VALUE nm)
{
  int rank;
  const int *n;
  double *in;
  fftw_complex *out;
  fftw_plan plan;

  //In place inpute 
  in = (double*)malloc(sizeof(double)*(nm * nm));
  out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) *  nm * (nm/2 + 1));

  plan = fftw_plan_dft_r2c(rank, n, in, out, FFTW_ESTIMATE);

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
  cNMatrix = rb_define_class("NMatrix",mFFTW);
 // rb_define_singleton_method(cNMatrix, "missing", fftw_missing, 1);
}

#if defined(cplusplus)
}  /* extern "C" { */
#endif
