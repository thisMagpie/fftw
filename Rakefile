require 'rspec/core/rake_task'
require 'rubygems'
require 'rdoc/task'
require 'rake/extensiontask'
require 'bundler/gem_tasks'

RSpec::Core::RakeTask.new(:spec) do |t|
  t.fail_on_error = false
end
task :test => :spec

begin
  Bundler.setup(:test, :default, :development)
  rescue Bundler::BundlerError => e
  $stderr.puts e.message
  $stderr.puts "Run `bundle install` to install missing gems"
  exit e.status_code
end

BASEDIR = Pathname( __FILE__ ).dirname.relative_path_from( Pathname.pwd )
SPECDIR = BASEDIR + 'spec'

gemspec = eval(IO.read("fftw.gemspec"))

Gem::PackageTask.new(gemspec).define
  desc "install the gem locally"
  task :install => [:package] do
    sh %{gem install pkg/fftw-#{FFTW::VERSION}.gem}
  end
  def run *cmd
    sh(cmd.join(" "))
  end
  namespace :clean do
    task :clean do |task|
      Dir['*~'].each {|fn| rm fn rescue nil}
        tmp_path = "tmp/#{RUBY_PLATFORM}/fftw/#{RUBY_VERSION}"
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
    manifest_files  = File.read("Manifest").split
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
  Rake::ExtensionTask.new do |ext|
    ext.name ='fftw'
    ext.ext_dir = 'ext/fftw'
    ext.lib_dir = 'lib/fftw'
    ext.source_pattern = "**/*.{c,cpp,h}"
  end
RDoc::Task.new do |rdoc|
  rdoc.main = "README.rdoc"
  rdoc.rdoc_files.include(%w{README.rdoc ChangeLog LICENSE.txt lib/*.rb ext/fftw/*.cpp ext/fftw/*.c ext/fftw/*.h})
end
