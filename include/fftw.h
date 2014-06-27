
#ifndef ___FFTW_H___
#define ___FFTW_H___

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "ruby.h"
#include "ruby/io.h"

#ifdef HAVE_NMATRIX_H
void Init_fftw(VALUE self);
#endif
