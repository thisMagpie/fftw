require 'bundler/setup'
Bundler.setup
require './lib/nmatrix_fftw'

RSpec.configure do |config|
    config.expect_with :rspec, :stdlib
end
