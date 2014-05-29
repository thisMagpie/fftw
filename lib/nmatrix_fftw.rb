require 'rubygems'
require 'nmatrix'
require 'fftw3/nmatrix-fftw'

if File.exist?("lib/nmatrix-fftw.so")
  require "fftw3/nmatrix-fftw.so"
end

if File.exist?("ext/nmatrix-fftw.o")
  require "fftw3/nmatrix-fftw.o"
end
