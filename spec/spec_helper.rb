require 'bundler/setup'
Bundler.setup
require './lib/nmatrix-fftw'
require 'rspec'

RSpec.configure do |config|
    config.expect_with :rspec, :test_unit
end
