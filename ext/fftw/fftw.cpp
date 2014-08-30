#include <ruby.h>
#include <fftw.h>
#include <fftw_config.h>
#include <stdio.h>
#include <iostream>
#include <fftw3/api/fftw3.h>
using namespace std;

VALUE mFFTW;
VALUE cFFTW;

 /**
  Define and initialise the NMatrix class:
  The initialisation rb_define_class will
  just retrieve the NMatrix class that already exists
  or define a new class altogether if it does not
  find NMatrix. */
VALUE cNMatrix = rb_define_class("NMatrix", rb_cObject);

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

/**
 * fftw_complex_to_nm_complex:
 *
 * @self:
 *              The FFTW module.
 * @in:         
 *              The fftw_complex pointer to be cast to an nmatrix complex type
 *
 **/
VALUE fftw_complex_to_nm_complex(VALUE self, fftw_complex* in)
{
    return rb_funcall(rb_define_module("Kernel"),
                      rb_intern("Complex"),
                      2,
                      rb_float_new(((double (*)) in)[0]),
                      rb_float_new(((double (*)) in)[1]));
}

/**
 * fftw_shape:
 *              A ruby array, e.g. [2, 2] for a 2x2 matrix.
 * @self:
 *              The FFTW module.
 * @nmatrix:
 *              the nmatrix object which shape is called on  
 **/
static VALUE
fftw_shape(VALUE self, VALUE nmatrix)
{
  return rb_funcall(nmatrix, rb_intern("shape"), 0);
}

/**
 * size:
 *        The number of elements stored for a matrix with dimensions = shape
 * @self:
 *              The FFTW module.
 * @nmatrix:
 *              the nmatrix object which size is called on  
 */
static const int
fftw_size(VALUE self, VALUE nmatrix)
{

  return NUM2INT(rb_funcall(nmatrix, rb_intern("size"), 0));
}
/**
  fftw_r2c
  @param self
  @param nmatrix
  @return nmatrix
  With FFTW_ESTIMATE as a flag in the plan,
  the input and and output are not overwritten at runtime
  The plan will use a heuristic approach to picking plans
  rather than take measurements
*/
static VALUE
fftw_r2c_one(VALUE self, VALUE in_nmatrix, VALUE out_nmatrix)
{

  fftw_plan plan;

  VALUE shape = fftw_shape(self, in_nmatrix);

  const int in_size = fftw_size(self, in_nmatrix);

  double* in = ALLOC_N(double, in_size);
  fftw_complex* out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * in_size * in_size);

  for (int i = 0; i < in_size; i++)
  {
    in[i] = NUM2DBL(rb_funcall(in_nmatrix, rb_intern("[]"), 1, INT2FIX(i)));
  }

  plan = fftw_plan_dft_r2c(1, &in_size, in, out, FFTW_ESTIMATE);
  fftw_execute(plan);
  // INFO: http://www.fftw.org/doc/New_002darray-Execute-Functions.html#New_002darray-Execute-Functions
  fftw_destroy_plan(plan);

  // Assign the output to the proper locations in the output nmatrix
  for (int i = 0; i < fftw_size(self, out_nmatrix); i++)
  {
    rb_funcall(out_nmatrix, rb_intern("[]="), 2, INT2FIX(i), fftw_complex_to_nm_complex(self, &out[i]));
  }

  xfree(in);
  fftw_free(out);
  return out_nmatrix;
}

void
Init_fftw(void)
{
  mFFTW = rb_define_module("FFTW");
  cFFTW = rb_define_class_under(mFFTW, "FFTW", rb_cObject);

  rb_define_singleton_method(mFFTW, "r2c_one",
                            (VALUE (*)(...)) fftw_r2c_one,
                             2);

  rb_define_singleton_method(mFFTW,
                             "Z",
                             (VALUE (*)(...))fftw_complex_to_nm_complex,
                             1);
  rb_define_singleton_method(mFFTW,
                             "shape",
                             (VALUE (*)(...))fftw_shape,
                             1);
  rb_define_singleton_method(mFFTW,
                             "size",
                             (VALUE (*)(...))fftw_size,
                             2);
}
#if defined(cplusplus)
}
#endif
