require 'rubygems'
require 'nmatrix'
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
  def vec_fft(vec)
    return if vec.size <= 1

    even = fft(NMatrix.new(vec.size / 2) { |i| vec[2 * i] })
    odd  = fft(NMatrix.new(vec.size / 2) { |i| vec[2 * i + 1] })

    fft_even.concat(fft_even)
    fft_odd.concat(fft_odd)

  end

  module_function :fft
  module_function :ifft
  module_function :vec_fft
end
