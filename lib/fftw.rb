require 'nmatrix'
require 'colorize'

module FFTW
  class NMatrix
    def singleton_method?self.missing(sym, *argv)
      name = sym.to_s.downcase
      if respond_to?(name) then
        send(name, *argv)
      else
        raise NotImplementedError, "no such method #{name}"
      end
    end
    def singleton_method?self.r2c(sym, *argv)
      name = sym.to_s.downcase
      if respond_to?(name) then
        send(name, *argv)
      else
        raise NotImplementedError, "no such method #{name}"
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
