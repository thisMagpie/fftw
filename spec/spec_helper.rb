$:.unshift(File.dirname(__FILE__)+"/../lib")
require 'bundler/setup'
Bundler.setup
require 'fftw'
require 'rspec'
require 'nmatrix'
require './lib/fftw'
require 'rspec/core'
require 'rspec/core/formatters/base_formatter'
