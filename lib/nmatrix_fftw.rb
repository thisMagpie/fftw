require 'rubygems'
require 'nmatrix'
require 'fftw3/nmatrix-fftw'

if File.exist?("lib/nmatrix_fftw.so")
  require "nmatrix_fftw.so"
end

if File.exist?("ext/nmatrix_fftw.o")
  require "nmatrix_fftw.o"
end
