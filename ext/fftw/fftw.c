#include <fftw3.h>
#include <ruby.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef HAVE_NMATRIX_H
#include "nmatrix.h"

VALUE mFFTW;

static VALUE nm_fftw_r2r(int argc, VALUE* argv, VALUE self);

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
static VALUE nm_fftw_r2r(int argc, VALUE* argv, VALUE self)
{
  double *in, *out;
  const int m;
  /* get the size of the array*/
  struct NMatrix  *nm;
  int i, j;

  nm = to_nmatrix(argv[0]);
  for(i = 0; i < m; i++)
  {

  in =  malloc(sizeof(argv)) ;
    for(j = 0; j < m; j++)
    {
      printf("Enter in value\n");
      out[i] = in[(i * m) + j];
    }
    out[i] /= m;;
  }
  return out[m-1];
}

void Init_nm_fftw()
{
  mFFTW = rb_define_module("FFTW");
  rb_define_singleton_method(mFFTW,"r2r",nm_fftw_r2r,1);
}
#endif