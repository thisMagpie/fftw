require './lib/fftw'

require 'bundler/setup'
Bundler.setup
require 'rspec'
require 'fftw'

RSpec.configure do |config|
   config.expect_with :rspec
end
