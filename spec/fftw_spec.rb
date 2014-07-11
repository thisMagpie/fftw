require 'spec_helper'
require 'fftw'
require 'nmatrix'

describe FFTW, "init" do
  it "creates an nmatrix object" do
    n = NMatrix.new([2,2], [0,1,2,3], dtype: :int64)
    expect(n.dtype).to eq(:int64)
  end
  it "creates an fftw object and checks its arguments" do
    n = NMatrix.new([2,2], [0,1,2,3], dtype: :int64)
    fft = FFTW.new(n) #=> create a new fftw instance
    expect(fft.shape).to eq(n.shape)
    expect(n.entries).to eq([0,1,2,3])
    expect { |fft| 0.tap(fft) }.to yield_with_args(1)
  end
end