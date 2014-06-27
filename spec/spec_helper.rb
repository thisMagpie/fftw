require './lib/fftw'
require "./lib/fftw/version"

require 'bundler/setup'
Bundler.setup
require 'nmatrix_spec'
require 'rspec'

RSpec.configure do |config|
   config.expect_with :rspec
end
