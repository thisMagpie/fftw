/*
  Ruby/GSL: Ruby extension library for GSL (GNU Scientific Library)
    (C) Copyright 2001-2004 by Yoshiki Tsunesada

  Ruby/GSL is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License.
  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FFTNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef ___FFTW3_H___
#define ___FFTW3_H___

#include <stdio.h>
#include <math.h>
#include "ruby.h"
#include <fftw3/fftw3_errno.h>
#include <fftw3/fftw3.h>
#include <fftw3/fftw3_complex.h>
#include <fftw3/fftw3_real.h>
#include <fftw3/fftw3_halfcomplex.h>
#include "nmatrix.h"

#include "complex.h"
#ifdef HAVE_NMATRIX_H
#include "nmatrix.h"
#endif

typedef struct
{
  size_t n;
  size_t nf;
  size_t factor[64];
  fftw3_complex *twiddle[64];
  fftw3_complex *trig;
} FFTW3_Wavetable;

typedef struct
{
  size_t n;
  double *scratch;
} FFTW3_Workspace;

enum {
  FFTW3_INPLACE,
  FFTW3_COPY,
};

EXTERN VALUE mfftw3;
EXTERN VALUE cfftw3_wavetable;
EXTERN VALUE cfftw3_wavetable_factor;
EXTERN VALUE cfftw3_complex_wavetable, cgsl_fft_complex_workspace;
EXTERN VALUE cfftw3_real_wavetable, cgsl_fft_halfcomplex_wavetable;
EXTERN VALUE cfftw3_real_workspace;

#endif
