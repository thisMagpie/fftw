require 'nmatrix'
require './lib/fftw'
require 'colorize'

class FFTW
  def self.ifft()
    define_method()
  end
  def self.r2c()
    define_method()
  end
  def self.free()
    raise ArgumentError, "need a block" if !block
    define_method()
  end
  def self.alloc()
    define_method()
  end
  def method_missing(sym, *argv)
      method_name = sym.to_s.downcase
      if respond_to?(method_name) then
          send(method_name, *argv)
      else
          raise NotImplementedError, "no such method #{method_name}"
      end
  end
end

if File.exist?("../lib/fftw/fftw.so")
  require 'fftw.so'
end

if File.exist?("../ext/fftw/fftw.o")
  require 'fftw.o'
end
