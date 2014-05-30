require 'rspec/core/rake_task'
require 'rubygems'
require 'bundler'
require 'rubygems/package_task'
RSpec::Core::RakeTask.new('spec')
task :default => :spec

begin
  Bundler.setup(:default, :development)
rescue Bundler::BundlerError => e
  $stderr.puts e.message
  $stderr.puts "Run `bundle install` to install missing gems"
  exit e.status_code
end

BASEDIR = Pathname( __FILE__ ).dirname.relative_path_from( Pathname.pwd )
SPECDIR = BASEDIR + 'spec'

gemspec = eval(IO.read("nmatrix-fftw.gemspec"))

Gem::PackageTask.new(gemspec).define

desc "install the gem locally"
task :install => [:package] do
  sh %{gem install pkg/nmatrix-fftw-#{NMatrixFFTW::VERSION::STRING}.gem}
end

def run *cmd
  sh(cmd.join(" "))
end

namespace :clean do
  task :so do |task|
    tmp_path = "tmp/#{RUBY_PLATFORM}/fftw3/#{RUBY_VERSION}"
    chdir tmp_path do
      if RUBY_PLATFORM =~ /mswin/
        `nmake soclean`
      else
        mkcmd = ENV['MAKE'] || %w[gmake make].find { |c| system("#{c} -v >> /dev/null 2>&1") }
        `#{mkcmd} soclean`
      end
    end
  end
end


desc "Check the manifest for correctness"
task :check_manifest do |task|
  manifest_files  = File.read("Manifest.txt").split

  git_files       = `git ls-files |grep -v 'spec/'`.split
  ignore_files    = %w{.gitignore .rspec}

  possible_files  = git_files - ignore_files

  missing_files   = possible_files - manifest_files
  extra_files     = manifest_files - possible_files

  unless missing_files.empty?
    STDERR.puts "The following files are in the git repo but not the Manifest:"
    missing_files.each { |f| STDERR.puts " -- #{f}"}
  end

  unless extra_files.empty?
    STDERR.puts "The following files are in the Manifest but may not be necessary:"
    extra_files.each { |f| STDERR.puts " -- #{f}"}
  end

  if extra_files.empty? && missing_files.empty?
    STDERR.puts "Manifest looks good!"
  end

end

require 'rdoc/task'
RDoc::Task.new do |rdoc|
  rdoc.main = "README.rdoc"
  rdoc.rdoc_files.include(%w{README.rdoc History.txt LICENSE.txt CONTRIBUTING.md lib/*.rb ext/fftw3/**/*.cpp ext/fftw3/**/*.c include/*.h})
end
