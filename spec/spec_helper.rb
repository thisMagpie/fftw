require 'bundler/setup'
Bundler.setup
require 'nmatrix-fftw'

RSpec.configure do |config|
    config.expect_with :rspec, :stdlib
end
