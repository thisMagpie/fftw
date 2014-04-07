require 'rubygems'
require 'narray-nmatrix'
require 'fftw3/fftw3'

module FFTW3
  def self.fft(*args)
    args.insert(1, -1)
    fft_raw(*args)
  end

  def self.ifft(*args)
    args.insert(1, 1)
    fft_raw(*args)
  end

  # source http://www.gregfjohnson.com/fftruby/ 
  # This function is not yet made fully compatible with narry but seems a sensible starting point to get it going
  # With Narray and then worry about nmatrix after that.
 
  def vec_fft(vec)
    return vec if vec.size <= 1

    even = fft(NArray.new(vec.size / 2) { |i| vec[2 * i] })
    odd  = fft(NArray.new(vec.size / 2) { |i| vec[2 * i + 1] })

    fft_even.concat(fft_even)
    fft_odd.concat(fft_odd)

    NArray.new(vec.size) {|i| fft_even[i] + fft_odd [i] * (Math::E ** Complex(0, 2 * Math::PI * -i / vec.size))} #FIXME this doesn't worqk because it's not actually using the narray functions yet
  end

  module_function :fft
  module_function :ifft
  module_function :vec_fft
end
