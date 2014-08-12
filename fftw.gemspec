lib = File.expand_path('../lib/', __FILE__)
$:.unshift lib unless $:.include?(lib)
require 'colorize'
require 'fftw/version'

Gem::Specification.new do |gem|
  gem.platform                      = Gem::Platform::RUBY
  gem.version                       = VERSION
  gem.name                          = 'fftw'
  gem.license                       = 'BSD 2-clause'
  gem.authors                       = 'Magdalen Berns'
  gem.summary                       = "Ruby FFTW3 wrapper for performing FFTW operations on NMatrix objects."
  gem.description                   = gem.summary + "Fastest Fourier Transform in the West"
  gem.require_paths                 = [ 'lib/' + gem.name , 'ext/' + gem.name]
  gem.extensions                    = Dir['ext/**/extconf.rb']
  gem.files                         = `git ls-files`.split("\n")
  gem.executables                   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  gem.test_files                    = `git ls-files -- {spec}/*`.split("\n")
  gem.homepage                      = 'http://sciruby.com/fftw'
  gem.rubygems_version              = '2.2.2'
  gem.required_rubygems_version     = Gem::Requirement.new(">= #{gem.rubygems_version}")
  gem.email                         = 'm.berns@thismagpie.com'
end
