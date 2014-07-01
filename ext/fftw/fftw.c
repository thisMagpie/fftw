#include <fftw3.h>
#include <ruby.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"

VALUE mFFTW;

/**
 * Define Real To Real Transform Method
 * TODO to be tested
 * Once this is done this should be able to take nmatrix objects of real data
 * so that the transformed array can be returned an nmatrix object which holds
 * a real to real transform in an nmatrix array
 * Usage could allow:
 * Y = fft(x)
 * Y = fft(X,n)
 * Y = fft(X,[],dim)
 * Y = fft(X,n,dim)
 */
static VALUE fftw_r2r(VALUE self, VALUE r2r)
{
  VALUE p;
  double *in, *out;
  in = calloc(r2r, sizeof(double));
  rb_need_block();
  rb_funcall(self, rb_intern("define_method"), 2, r2r, rb_block_proc());
  int i,j;
  for(i = 0; i < 10; i++)
  {
    for(j = 0; j < 10; j++)
    {
      out[i] += NUM2DBL(rb_ary_entry(r2r, (i * 10) + j));
    }
    out[i] /= 10;
    prinf(out[i]);
  }
  return &out;
}
void Init_fftw()
{
  mFFTW = rb_define_module("FFTW");
  rb_define_singleton_method(mFFTW,"fftw_r2r",fftw_r2r,1);
}
