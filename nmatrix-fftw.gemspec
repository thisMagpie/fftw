lib = File.expand_path('../lib/', __FILE__)
$:.unshift lib unless $:.include?(lib)
require 'nmatrix-fftw/version'

Gem::Specification.new do |gem|
  gem.name = 'nmatrix-fftw'
  gem.version = FFTW::VERSION::STRING
  gem.platform    = Gem::Platform::RUBY
  gem.require_paths = ["."]
  gem.add_dependency 'nmatrix', '~> 0.1.0'
  gem.add_dependency 'rdoc', '>=4.0.1'
  gem.add_dependency 'rake-compiler', '~>0.8.1'
  gem.add_development_dependency 'rspec', '~> 2.14.1'
  gem.add_development_dependency 'rake'
  gem.add_development_dependency 'bundler'
  gem.add_development_dependency 'rspec-longrun'
  gem.add_development_dependency 'pry'
  gem.add_development_dependency 'rake-compiler', '~>0.8.1'
  gem.add_runtime_dependency 'nmatrix', '~> 0.1.0'

  gem.extensions = ['ext/fftw3/extconf.rb']
  gem.files         = `git ls-files`.split("\n")
  gem.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  gem.test_files    = `git ls-files -- {spec}/*`.split("\n")
  gem.homepage = 'http://thismagpie.com/key/sciruby'
  gem.require_paths = ['lib', 'ext', '../nmatrix/lib']
  gem.rubygems_version = '2.2.2'
  gem.summary = 'Ruby FFTW3 wrapper for performing FFT operations on NMatrix objects.'
  gem.required_rubygems_version = Gem::Requirement.new(">= 2.2.2")
  gem.authors = ['T. Horinouchi',
                 'Yoshiki Tsunesada',
                 'David MacMahon',
                 'John Woods',
                 'Masaomi Hakateyama',
                 'Magdalen Berns'
                 ]
  gem.description = 'Ruby wrapper for performing FFTW3 with NMatrix'
  gem.email = 'm.berns@thismagpie.com'
  gem.post_install_message = <<-EOF
  ***********************************************************
  Welcome to SciRuby: Tools for Scientific Computing in Ruby!

  FFTW3 requires a C compiler, and has been tested only
  with GCC 4.8+. We are happy to accept contributions
  which improve the portability of this project.

  Thanks for trying out FFTW3! Happy coding!

  ***********************************************************
  EOF
end
