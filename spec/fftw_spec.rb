require 'fftw'

RSpec.configure do |config|
  config.expect_with :rspec, :stdlib,:fftw
end

describe FFTW do
  push "check"
end
