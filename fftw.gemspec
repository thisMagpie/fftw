lib = File.expand_path('../lib/', __FILE__)
$:.unshift lib unless $:.include?(lib)
require 'colorize'

Gem::Specification.new do |gem|
  gem.platform                      = Gem::Platform::RUBY
  gem.version                       = '0.0.9'
  gem.name                          = 'fftw'
  gem.license                       = 'BSD 2-clause'
  gem.authors                       = 'Magdalen Berns'
  gem.summary                       = "Ruby FFTW3 wrapper for performing FFTW operations on NMatrix objects."
  gem.description                   = gem.summary + "Fastest Fourier Transform in the West"
  gem.require_paths                 = [ 'lib/fftw', 'ext/fftw', '../nmatrix-0.1.0.rc4' ]
  gem.autorequire                   = [ 'fftw','nmatrix', 'colorize' ]
  gem.extensions                    = [ '../nmatrix/ext/nmatrix/extconf.rb', 'ext/fftw/extconf.rb' ]
  gem.files                         = `git ls-files`.split("\n")
  gem.executables                   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  gem.test_files                    = `git ls-files -- {spec}/*`.split("\n")
  gem.homepage                      = 'http://sciruby.com/fftw'
  gem.rubygems_version              = '2.2.2'
  gem.required_rubygems_version     = Gem::Requirement.new(">= 2.2.2")
  gem.email                         = 'm.berns@thismagpie.com'
  gem.post_install_message          =

  <<-EOF
  ****************************************************************
  *                                                              *
  *  Welcome to SciRuby: Tools for Scientific Computing in Ruby! *
  *                                                              *
  *  FFTW3 requires a C compiler, and has been tested only with  *
  *  GCC 4.8+.                                                   *
  *                                                              *
  *  We are happy to accept contributions which improve the      *
  *  portability of this project.                                *
  *                                                              *
  *  Thanks for trying out FFTW3! Happy coding!                  *
  *                                                              *
  ****************************************************************
  EOF
  puts gem.post_install_message.to_s.colorize(:color => :black,
                                         :background => :light_blue)
end
