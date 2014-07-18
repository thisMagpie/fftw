require 'spec_helper'
require 'fftw'
require 'nmatrix'

describe FFTW, "nmatrix" do
  it "creates an nmatrix object" do
    n = NMatrix.new([2,2], [0,1,2,3], dtype: :int64)
    expect(n.dtype).to eq(:int64)
    expect(FFTW::NMatrix.new([2,2])).to eq(n)
  end
end