require 'bundler/setup'
Bundler.setup
require "codeclimate-test-reporter"
CodeClimate::TestReporter.start
require 'fftw'
require 'rspec'
require './lib/fftw'


