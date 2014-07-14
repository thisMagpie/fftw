#include "ruby.h"
#include <fftw3.h>
#include <fftw.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
using namespace std;

// Prototype for ruby to call
void Init_fftw();

VALUE mFFTW;
VALUE cNMatrix;
int i = 0;

static VALUE fftw_nmatrix(int argc, VALUE* argv, VALUE self)
{
  VALUE in;
  VALUE nmatrix, size;
  rb_scan_args(argc, argv, "11", &nmatrix, &size);

  if (NIL_P(size))        // if no size was given...
    size = INT2NUM(0);  // use the default value

  rb_iv_set(self, "in", in);

  //for (int i = 0; i< length; i++){
  // double nm = NUM2DBL(rb_funcall(args, rb_intern("[]") 2));
  //}

 // rb_yield(Qnil);
  printf("TEST\n");
  return self;
}

  /* 

VALUE fftw_r2c(VALUE self, VALUE nmatrix)
{
  int n;
  VALUE size;
  double *in;
  fftw_complex *out;
  fftw_plan plan;
  int nm[]={2, 2};

  //In place inpute 
  in = (double*)malloc(sizeof(double)*(nm[0] * nm[1]));
  out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) *  nm[0] * (nm[1]/2 + 1));

   //TODO add plan
   //Expected arguments
   //fftw_plan_dft_r2c(int rank, const int *n, double *in, fftw_complex *out, unsigned flags);
   plan = fftw_plan_dft_r2c(...n, out, FFTW_ESTIMATE);

  fftw_execute(plan);
  free(in);
  fftw_destroy_plan(plan);


  return self;
}
*/
/*
void Init_fftw(void)
{
  mFFTW = rb_define_module("FFTW");
  cNMatrix = rb_define_class("NMatrix",mFFTW);
  rb_define_singleton_method(cNMatrix, "nmatrix", fftw_nmatrix, 0);
}*/