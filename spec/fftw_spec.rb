require 'spec_helper'
require 'fftw'
require 'nmatrix'
require 'rspec/expectations'

describe FFTW do
  it "creates an nmatrix object" do
    n = NMatrix.new([2,2], [0,1,2,3], dtype: :int64)e
    expect(n.shape).to eq([2,2])
    expect(n.entries).to eq([0,1,2,3])
    expect(n.dtype).to eq(:int64)
  end
  it "creates an fftw object and checks its arguments" do
    fftw = FFTW.new() #=> create a new fftw unstance
    #expect { |fftw| 0.tap(fftw) }.to yield_with_args(0)
  end
end
