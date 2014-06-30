#include <fftw3.h>
#include <ruby.h>
#include <stdio.h>
#include <stdlib.h>


VALUE mFFTW;

/**
 * get_doubles
 * Get index double data from an array
 */
void get_doubles(double *out, VALUE in, long n, long m){
  VALUE num;
  int i,j;

  for(i = 0; i < n; i++){
    for(j = 0; j < m; j++){
      out[i] += NUM2DBL(rb_ary_entry(in, (i * m) + j));
    }
    out[i] /= m;
  }
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
  fftw_plan ftp;
  int n;

  in = calloc(n, sizeof(double));
  rb_need_block();
  rb_funcall(self, rb_intern("define_method"), 2, r2r, rb_block_proc());
  get_doubles(in, r2r);
  return Qnil; /* todo return an array */
}
void Init_fftw()
{
  mFFTW = rb_define_module("FFTW");
  rb_define_singleton_method(mFFTW,"fft",fftw_fft,1);
}
