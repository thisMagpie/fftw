require 'rubygems'
require 'nmatrix-fftw'
require 'nmatrix'
require 'nmatrix-fftw/version.rb'

if File.exist?("lib/nmatrix-fftw.so")
  require 'nmatrix-fftw.so'
end
