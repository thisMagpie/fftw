#include "ruby.h"
#include <fftw.h>
#include <fftw_config.h>
#include <stdio.h>
#include <iostream>
#include <fftw3/api/fftw3.h>
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
extern "C" {
#endif

static int
fftw_rank(VALUE self, int size, VALUE shape)
{
  return size - FIX2INT(rb_ary_entry(shape, 0));
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
fftw_r2c_one(VALUE self, VALUE nmatrix)
{
 /**
  Define and initialise the NMatrix class:
  The initialisation rb_define_class will
  just retrieve the NMatrix class that already exists
  or define a new class altogether if it does not
  find NMatrix. */
  VALUE cNMatrix = rb_define_class("NMatrix", rb_cObject);

  // URL: http://www.fftw.org/fftw2_doc/fftw_2.html#SEC11
  char *wisdom_string;

  fftw_plan plan;

  // shape is a ruby array, e.g. [2, 2] for a 2x2 matrix
  VALUE shape = rb_funcall(nmatrix, rb_intern("shape"), 0);

  // size is the number of elements stored for a matrix with dimensions = shape
  const int size = NUM2INT(rb_funcall(cNMatrix, rb_intern("size"), 1, shape));

  //Input: a 1D double array with enough elements for the whole matrix
  double* in = ALLOC_N(double, size);

  int rank = fftw_rank(self, size, shape);
  printf("Rank: %d \n", rank);

  // This would need to be a nested loop for multidimensional matrices, or it
  // would need to use the size instead of the shape and figure out the indices
  // to pass to [] appropriately from that.
  for (int i = 0; i < (size / 2); i++)
  {
    // TODO 2D array NUM2DBL(rb_funcall(nmatrix, rb_intern("[]"), 2, INT2FIX(i),INT2FIX(j)));
    in[i] = NUM2DBL(rb_funcall(nmatrix, rb_intern("real"), 1, INT2FIX(i)));
    printf("IN[%d]: in[%.2f] \n", i, in[i]);
  }
  fftw_complex* out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (size / 2.0) + 0.5);
 
  // forward transfer
  plan = fftw_plan_dft_r2c_1d(size, in, out, FFTW_ESTIMATE);
  fftw_execute(plan);
  printf("Cost: %f \n",fftw_cost(plan));

  // INFO: http://www.fftw.org/doc/New_002darray-Execute-Functions.html#New_002darray-Execute-Functions
  fftw_execute_dft_r2c(plan, in, out);
  fftw_destroy_plan(plan);
  xfree(in);
  fftw_free(out);
  return nmatrix;
}

void
Init_fftw(void)
{
  mFFTW = rb_define_module("FFTW");

  rb_define_singleton_method(mFFTW, "r2c_one",
                            (VALUE (*)(...)) fftw_r2c_one,
                             1);

  rb_define_singleton_method(mFFTW,
                             "rank",
                             (VALUE (*)(...)) fftw_rank,
                             2);
}
#if defined(cplusplus)
}
#endif
