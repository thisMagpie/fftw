
#ifndef ___FFTW3_NMATRIX_H___
#define ___FFTW3_NMATRIX_H___

#include "nmatrix.h"

// in fftw3_nmatrix.c
VALUE fftw3_nm_to_fftw3_nmatrix(VALUE obj, VALUE nm);
fftw3_nmatrix* nm_to_fftw3(VALUE nm);

VALUE fftw3_nm_to_fftw3_nmatrix_complex(VALUE obj, VALUE nm);
fftw3_nmatrix_complex* nm_to_fftw3_complex(VALUE nm);

VALUE fftw3_nm_to_fftw3_nmatrix_int(VALUE obj, VALUE nm);
fftw3_nmatrix_int* nm_to_int(VALUE nm);

extern VALUE CNMatrix;
