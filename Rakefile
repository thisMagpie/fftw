require 'rspec/core/rake_task'
require 'rubygems'
require 'rake/file_utils'
require 'rake/extensiontask'
require 'bundler/gem_tasks'
require 'colorize'

def get_stdin(message)
  print message
  STDIN.gets.chomp
end

desc "Set to not fail on error by default".red
RSpec::Core::RakeTask.new(:spec) do |t|
  t.fail_on_error = true
end
task :default => :spec

RSpec::Core::RakeTask.new(:build) do |build|
  Dir.chdir('ext/fftw') do
    puts `ruby extconf.rb`
    puts `make`
  end
end
task :default => :build

require 'rdoc/task'
RDoc::Task.new(:rdoc) do |rdoc|
  rdoc.main = "README.rdoc"
  rdoc.rdoc_files.include(%w{README.rdoc
                             Rakefile
                             fftw.gemspec
                             MANUAL.md
                             lib/fftw/**/*.rb
                             ext/fftw/*.cpp
                             ext/fftw/*.h})
end

Rake::ExtensionTask.new "fftw" do |ext|
  ext.name ='fftw'
  ext.ext_dir = 'ext/fftw'
  ext.lib_dir = 'lib/fftw'
  ext.source_pattern = "**/*.{c,cpp,h}"
end

desc "Install the gem locally".cyan
task :install => [:package] do
  sh %{gem install pkg/fftw-#{FFTW::VERSION}.gem}
end
def run *cmd
  sh(cmd.join(" "))
end

namespace :clean do
  task :clean => :environment do |task|
    Dir['*~'].each {|fn| rm fn rescue nil}
      tmp_path = "tmp/#{RUBY_PLATFORM}/fftw/#{RUBY_VERSION}"
      chdir tmp_path do
      if RUBY_PLATFORM =~ /mswin/
        `nmake soclean`
      else
        mkcmd = ENV['MAKE'] || %w[gmake make].find { |c| system("#{c} -v >> /dev/null 2>&1") }
        `#{mkcmd} distclean`
      end
    end
  end
end
desc "Check the manifest for correctness".cyan
task :check_manifest do |task|
  manifest_files  = File.read("Manifest").split
  git_files       = `git ls-files |grep -v 'spec/'`.split
  ignore_files    = %w{.gitignore .rspec}
  possible_files  = git_files - ignore_files
  missing_files   = possible_files - manifest_files
  extra_files     = manifest_files - possible_files

  unless missing_files.empty?
    puts "WARNING: following files are missing from the Manifest:".yellow
    missing_files.each { |f| puts "#{f}"}
  end

  unless extra_files.empty?
    puts "WARNING: The following files are in the Manifest but should be removed".yellow
    extra_files.each { |f| puts "#{f}"}
  end

  if extra_files.empty? && missing_files.empty?
    STDERR.puts "Manifest looks good!".green
  end
end # end task :check_manifest .... done!

desc "List tasks".cyan
task :list do
  puts "Tasks: #{(Rake::Task.tasks - [Rake::Task[:list]]).join(' ')}"
  puts "(type rake -T for more detail)\n\n"
end

ruby_path = File.dirname(__FILE__)
project_path = File.expand_path( File.join(ruby_path, '..') )
binary_path = File.join(project_path, VERSION)

pattern = File.join(binary_path, "*.{so,bundle,o}")
Dir.glob(pattern).each do |library|
  require library
end

begin
  Bundler.setup(:test,:default,:clean,:development,:build,:list)
  rescue Bundler::BundlerError => e
  $stderr.puts e.message
  $stderr.puts "Run `bundle install` to install missing gems".red
  exit e.status_code
end
