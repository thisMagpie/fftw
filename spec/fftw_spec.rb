require File.expand_path(File.dirname(__FILE__)+"/spec_helper.rb")

describe FFTW do
  it "creates an NMatrix object" do
    n = NMatrix.new([2,2], dtype: :int64)
    expect(n.dtype).to eq(:int64)
  end
  it "Fills dense with individual assignments" do
    n = NMatrix.new([2,2],
                    [-3.105036184601418e+231,
                     -1.730600617294159e-77,
                      1.0e-323,
                      2.83811411759045e-309],
                      dtype: :float64)
    expect(n[0,0]).to eq(-3.105036184601418e+231)
    expect(n[0,1]).to eq(-1.730600617294159e-77)
    expect(n[1,0]).to eq(1.0e-323)
    expect(n[1,1]).to eq(2.83811411759045e-309)
  end
  it "Creates a new FFTW::FFTW.r2c object and takes an NMatrix as its argument" do
    n = NMatrix.new([4], [3.10, 1.73, 1.04, 2.83])
    fftw = FFTW.r2c(n)
    expect(fftw).to eq(fftw)
  end
end
