require 'nmatrix'
require 'colorize'

if File.exist?("../lib/fftw/fftw.so")
  require 'fftw.so'
end

if File.exist?("../ext/fftw/fftw.o")
  require 'fftw.o'
end

module FFTW
  class FFTW
    def self.v(sym, *argv)
      define_singleton_method()
    end
    def self.r2c(sym, *argv)
      define_singleton_method()
    end
    def missing(sym, *argv)
      define_singleton_method()
      method = sym.to_s.downcase
      if respond_to?(method) then
        send(method, *argv)
      else
        raise NotImplementedError, "no such method #{method}"
      end
    end
  end
end
