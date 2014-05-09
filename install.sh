#!/bin/bash

ruby ext/fftw3/extconf.rb --with-nmatrix-include='../nmatrix-0.1.0.rc3/ext/nmatrix' --with-fftw3-dir='../fftw3'
make
make install


