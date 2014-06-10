
#ifndef ___RB_FFTW_H___
#define ___RB_FFTW_H___

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "ruby.h"
#ifdef RUBY_1_9_LATER
#include "ruby/io.h"
#else
#include "ruby/io.h"
#endif

#ifdef HAVE_NMATRIX_H
void Init_nmatrix_fftw(VALUE module);
#endif
