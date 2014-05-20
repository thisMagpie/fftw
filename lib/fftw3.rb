require 'rubygems'
require 'nmatrix'
require 'fftw3/fftw3'

if File.exist?("lib/fftw3.so")
  require "fftw3/fftw3.so"
end

if File.exist?("ext/fftw3.o")
  require "fftw3/fftw3.o"
end
