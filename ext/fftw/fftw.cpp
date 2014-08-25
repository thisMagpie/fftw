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

VALUE fftw_complex_to_nm_complex(fftw_complex* in) {
    double real = ((double (*)) in)[1];
    double imag = ((double (*)) in)[2];
    VALUE mKernel = rb_define_module("Kernel");
    return rb_funcall(mKernel,
                      rb_intern("Complex"),
                      2,
                      rb_float_new(real),
                      rb_float_new(imag));
}

static VALUE
fftw_shape(VALUE self)
{
  // shape is a ruby array, e.g. [2, 2] for a 2x2 matrix
  return rb_funcall(cNMatrix, rb_intern("shape"), 0);
}

static const int
fftw_size(VALUE self, VALUE shape)
{
  // size is the number of elements stored for a matrix with dimensions = shape
  return NUM2INT(rb_funcall(cNMatrix, rb_intern("size"), 1, shape));
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
fftw_r2c_one(VALUE self, VALUE nmatrix)
{

  fftw_plan plan;

  const int rank = rb_iv_set(self, "@rank", 1);

  VALUE shape = fftw_shape(self);
  const int size = fftw_size(self, shape);

  double* in = ALLOC_N(double, size);
  fftw_complex* out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * size * size);

  for (int i = 0; i < size; i++)
  {
    in[i] = NUM2DBL(rb_funcall(nmatrix, rb_intern("[]"), 1, INT2FIX(i)));;
  }

  plan = fftw_plan_dft_r2c(1,&size, in, out, FFTW_ESTIMATE);
  fftw_execute(plan);

  for (int i = 0; i < 2; i++)
  {
    rb_funcall(nmatrix, rb_intern("[]="), 2, INT2FIX(i), fftw_complex_to_nm_complex(out + i));
  }

  // INFO: http://www.fftw.org/doc/New_002darray-Execute-Functions.html#New_002darray-Execute-Functions
  fftw_destroy_plan(plan);

  xfree(in);
  fftw_free(out);
  return nmatrix;
}

void
Init_fftw(void)
{
  mFFTW = rb_define_module("FFTW");
  cFFTW = rb_define_class_under(mFFTW, "FFTW", rb_cObject);

  rb_define_singleton_method(mFFTW, "r2c_one",
                            (VALUE (*)(...)) fftw_r2c_one,
                             1);

  rb_define_singleton_method(mFFTW,
                             "Z",
                             (VALUE (*)(...))fftw_complex_to_nm_complex,
                             1);
  rb_define_singleton_method(mFFTW,
                             "shape",
                             (VALUE (*)(...))fftw_shape,
                             0);
  rb_define_singleton_method(mFFTW,
                             "size",
                             (VALUE (*)(...))fftw_shape,
                             1);
}
#if defined(cplusplus)
}
#endif
