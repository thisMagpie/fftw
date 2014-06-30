#include <fftw3.h>
#include <ruby.h>
#include <stdio.h>

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
static VALUE fftw_fft(VALUE self, VALUE r2r)
{
   VALUE p;
   rb_need_block();
   p = rb_block_proc();
   rb_funcall(self, rb_intern("define_method"), 2, r2r, p);
   return Qnil;
}
void Init_fftw()
{
  mFFTW = rb_define_module("FFTW");
  rb_define_singleton_method(mFFTW,"fft",fftw_fft,1);
}
