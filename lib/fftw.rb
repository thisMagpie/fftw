require 'rubygems'
require 'fftw'
require 'nmatrix'
require 'fftw/version.rb'

if File.exist?("lib/fftw.so")
  require 'fftw.so'1
end
