#ifndef NMatrixFFTW
#define NMatrixFFTW

#include <ruby.h>
#include <fftw3.h>

extern VALUE mFFTW;

#define FFTW_FORWARD (-1)
#define FFTW_BACKWARD (+1)
#endif
