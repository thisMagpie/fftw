require 'nmatrix'
require './lib/fftw'
require 'colorize'

unless File.respond_to?(:read)
  def File.read(fname)
    open(fname) {|f|
    	puts "opening #{fname} to read".green
      return f.read
    }
  end
end

if File.exist?("../lib/fftw/fftw.so")
  require 'fftw.so'
end

if File.exist?("../ext/fftw/fftw.o")
  require 'fftw.o'
end
