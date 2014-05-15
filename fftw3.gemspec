lib = File.expand_path('../lib/', __FILE__)
$:.unshift lib unless $:.include?(lib)

Gem::Specification.new do |gem|
  gem.name = fftw3
  gem.version = File.readlines('VERSION')[0].chomp

  gem.required_rubygems_version = Gem::Requirement.new(">= 2.2.2")
  gem.authors = ['T. Horinouchi',
                 'Yoshiki Tsunesada',
                 'David MacMahon',
                 'John Woods',
                 'Masaomi Hakateyama',
                 'Magdalen Berns'
                 ]

  gem.date = '2014-5-8'
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
  gem.extensions = ['ext/fftw3/extconf.rb']
  gem.files         = `git ls-files`.split("\n")
  gem.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  gem.homepage = 'http://thismagpie.com/key/sciruby'

  gem.require_paths = ['lib', 'ext']
  gem.include_path = ['include','/usr/include']
  gem.rubygems_version = '2.2.2'
  gem.summary = %q{Ruby FFTW3 wrapper}

  if gem.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    gem.specification_version = 4
    gem.add_runtime_dependency('nmatrix', ['>= 0.1.0'])
  end
end
