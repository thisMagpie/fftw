require 'nmatrix'
require 'colorize'
require 'fftw'

if File.exist?("../lib/fftw/fftw.so")
  require 'fftw.so'
end

if File.exist?("../ext/fftw/fftw.o")
  require 'fftw.o'
end

module FFTW
  class FFTW::NMatrix
    def self.r2c(sym, *argv)
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
end
