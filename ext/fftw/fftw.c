#include <fftw3.h>
#include <ruby.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
static VALUE fftw_r2r(double *r2r, int m)
{
  double *in, *out;
  int n;
  printf("%d ",&m);
  /* get the size of the array*/
  size_t size = (sizeof r2r / sizeof r2r[0]);

  int i, j;

  for(i = 0; i < m; i++)
  {

  in =  malloc(sizeof(r2r)) ;
    for(j = 0; j < m; j++)
    {
      printf("Enter in value\n");
      out[i] = in[(i * m) + j];
    }
    out[i] /= m;;
  }
  return out[m-1];
}

void Init_fftw()
{
  mFFTW = rb_define_module("FFTW");
  rb_define_singleton_method(mFFTW,"r2r",fftw_r2r,1);
}
