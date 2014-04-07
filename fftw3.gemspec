lib = File.expand_path('../lib/', __FILE__)
$:.unshift lib unless $:.include?(lib)

Gem::Specification.new do |gem|
  gem.name = fftw3
  gem.version = "0.3"

  gem.required_rubygems_version = Gem::Requirement.new(">= 1.2") if gem.respond_to? :required_rubygems_version=
  gem.authors = ["T. Horinouchi, Evan Weaver"]
  gem.date = %q{2009-10-21}
  gem.description = %q{Gem version of T. Horinouchi's ruby-fftw3.}
  gem.email = %q{}
  gem.post_install_message = <<-EOF
***********************************************************
Welcome to SciRuby: Tools for Scientific Computing in Ruby!

Happy coding!

***********************************************************
EOF
  gem.extensions = ["ext/extconf.rb"]
  gem.files = ["ChangeLog", "Manifest", "README", "Rakefile", "ToDo", "ext/extconf.rb", "ext/na_fftw3.c", "lib/fftw3.rb", "test/test_fftw3.rb", "fftw3.gemspec"]
  gem.homepage = %q{http://blog.evanweaver.com/files/doc/fauna/ruby-fftw3/}
  gem.rdoc_options = ["--line-numbers", "--inline-source", "--title", "Fftw3", "--main", "README"]
  gem.require_paths = ["lib"]
  gem.rubyforge_project = %q{fauna}
  gem.rubygems_version = %q{1.3.5}
  gem.summary = %q{Gem version of T. Horinouchi's ruby-fftw3.}
  gem.test_files = ["test/test_fftw3.rb"]

  if gem.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    gem.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
      gem.add_runtime_dependency(%q<narray-nmatrix>, [">= 0.6.1"])
    else
      gem.add_dependency(%q<narray-nmatrix>, [">= 0.6.1"])
    end
  else
    gem.add_dependency(%q<narray-nmatrix>, [">= 0.6.1"])
  end
end
