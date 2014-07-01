require './lib/fftw'
require "./lib/fftw/version"

require 'bundler/setup'
Bundler.setup
require 'rspec'
require 'nmatrix_spec'
require 'fftw'

RSpec.configure do |config|
   config.expect_with :rspec
end
