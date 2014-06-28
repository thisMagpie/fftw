require 'spec_helper'
require 'fftw'
require 'nmatrix'

describe FFTW do
  it "creates a matrix with the new constructor" do
    n = NMatrix.new([2,2], [0,1,2,3], dtype: :int64)
    expect(n.shape).to eq([2,2])
    expect(n.entries).to eq([0,1,2,3])
    expect(n.dtype).to eq(:int64)
  end
end


