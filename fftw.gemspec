lib = File.expand_path('../lib/', __FILE__)
$:.unshift lib unless $:.include?(lib)
require 'fftw/version'
require 'rubygems'

Gem::Specification.new do |gem|
  gem.name                          = 'fftw'
  gem.version                       =  FFTW::VERSION::STRING
  gem.license                       = 'BSD 2-clause'
  gem.authors                       = 'Magdalen Berns'
  gem.summary                       = "Ruby FFTW3 wrapper for performing FFT operations on NMatrix objects."
  gem.description                   = <<-EOF
                                        Ruby FFTW3 wrapper for performing FFT operations on NMatrix objects.
                                        FFTW3 requires a C compiler, and has been tested only with GCC 4.8+.
                                        We are happy to accept contributions which improve the portability
                                        of this project.
                                      EOF
  gem.platform                      = Gem::Platform::RUBY
  gem.require_paths                 = ['lib', 'ext', '../nmatrix/lib']
  gem.autorequire                   = 'fftw'
  gem.extensions                    = 'ext/fftw/extconf.rb'
  gem.files                         = `git ls-files`.split("\n")
  gem.executables                   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  gem.test_files                    = `git ls-files -- {spec}/*`.split("\n")
  gem.homepage                      = 'http://thismagpie.com/keyword/sciruby'
  gem.rubygems_version              = '2.2.2'
  gem.required_rubygems_version     = Gem::Requirement.new(">= 2.2.2")
  gem.email                         = 'm.berns@thismagpie.com'
  gem.post_install_message          = <<-EOF
  ******************************************************************

      Welcome to SciRuby: Tools for Scientific Computing in Ruby!

      FFTW3 requires a C compiler, and has been tested only
      with GCC 4.8+. We are happy to accept contributions
      which improve the portability of this project.

      Thanks for trying out FFTW3! Happy coding!

  ******************************************************************
  EOF

  gem.add_development_dependency 'rake_tasks', '~> 2.0', '>= 2.0.6'
  gem.add_development_dependency 'rdoc', '~> 4.0', '>= 4.0.1'
  gem.add_development_dependency 'rake-compiler', '~> 0.8', '>= 0.8.1'
  gem.add_development_dependency 'ffi', '~> 1.9', '>= 1.9.3'
  gem.add_development_dependency 'rspec', '~> 2.14', '>= 2.14.1'
  gem.add_development_dependency 'bundler', '~> 1.6', '>= 1.6.1'
  gem.add_development_dependency 'rspec-longrun', '~> 1.0', '>= 1.0.1'
  gem.add_development_dependency 'pry', '~> 0'

  gem.add_runtime_dependency 'rake', '~> 10.3', '>= 10.3.2'
  gem.add_runtime_dependency 'nmatrix', '0.1.0.rc3'
end
