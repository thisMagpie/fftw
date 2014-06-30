#include <fftw3.h>
#include <ruby.h>
#include <stdio.h>
#include <stdlib.h>

VALUE mFFTW;

fftw_complex * allocate_fftw_complex(long n){
  fftw_complex * fc;
  return ALLOC_N(fftw_complex, n);;
}

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
static VALUE fftw_fft(VALUE self, VALUE r2r)
{
  VALUE p;
  double *in;
  fftw_complex *out;
  fftw_plan ftp;
  int n;

  in = calloc(n, sizeof(double));

  rb_need_block();
  rb_funcall(self, rb_intern("define_method"), 2, r2r, rb_block_proc());
  return get_doubles_from_data(in, r2r);
}
void Init_fftw()
{
  mFFTW = rb_define_module("FFTW");
  rb_define_singleton_method(mFFTW,"fft",fftw_fft,1);
}
