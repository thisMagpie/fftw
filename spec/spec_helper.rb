require 'bundler/setup'
Bundler.setup

require 'nmatrix-fftw'
require 'nmatrix'

RSpec.configure do |config|
    config.expect_with :rspec, :stdlib,:fftw
end
