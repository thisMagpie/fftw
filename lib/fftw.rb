require 'rubygems'
require 'nmatrix'
require './lib/fftw'

if File.exist?("../lib/fftw.so")
  require 'fftw.so'
end
