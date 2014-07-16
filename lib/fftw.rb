require 'nmatrix'
require 'colorize'

module FFTW
  class NMatrix
    def self.nmatrix()
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

if File.exist?("../lib/fftw/fftw.so")
  require 'fftw.so'
end

if File.exist?("../ext/fftw/fftw.o")
  require 'fftw.o'
end
