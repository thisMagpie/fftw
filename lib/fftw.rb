require 'nmatrix'
require 'colorize'

def check_exists(str, req)
  if File.exist?(str)
    puts "#{str} found!".green #=>
  else
    puts "#{str} not found!".red #=>
  end
end

check_exists("./ext/fftw/fftw.o",'fftw.o')
check_exists("./lib/fftw/fftw.so",'fftw.so')


module FFTW
  class FFTW
    def self.v(sym, *argv)
      define_singleton_method()
    end
    def self.r2c(nmatrix)
      define_singleton_method()
      return nmatrix
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
