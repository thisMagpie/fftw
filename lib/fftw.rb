require 'rubygems'
require 'nmatrix'
require 'fftw/fftw.rb'

if File.exist?("lib/fftw.so")
  require 'fftw.so'
end
