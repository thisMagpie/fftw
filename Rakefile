require 'rspec/core/rake_task'
require 'rubygems'
require 'bundler'
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

desc "install the gem locally"
task :install => [:package] do
  sh %{gem install pkg/fftw3-#{NMATRIX::FFTW::VERSION}.gem}
end

VALGRIND_OPTIONS = [
    "--tool=memcheck",
    "--num-callers=15",
    "--partial-loads-ok=yes",
    "--undef-value-errors=no"
]

CALLGRIND_OPTIONS = [
    "--tool=callgrind",
    "--dump-instr=yes",
    "--simulate-cache=yes",
    "--collect-jumps=yes"
]

VALGRIND_MEMORYFILL_OPTIONS = [
    "--freelist-vol=100000000",
    "--malloc-fill=6D",
    "--free-fill=66 ",
]

GDB_OPTIONS = []


task :console do |task|
  cmd = [ 'irb', "-r './lib/fftw3.rb'" ]
  run *cmd
end

task :pry do |task|
  cmd = [ 'pry', "-r './lib/fftw3.rb'" ]
  run *cmd
end

namespace :pry do
  task :valgrind => [ :compile ] do |task|
    cmd  = [ 'valgrind' ] + VALGRIND_OPTIONS
    cmd += ['ruby', '-Ilib:ext', "-r './lib/fftw3.rb'", "-r 'pry'", "-e 'binding.pry'"]
    run *cmd
  end
end

namespace :console do
  CONSOLE_CMD = ['irb', "-r './lib/fftw3.rb'"]
  desc "Run console under GDB."
  task :gdb => [ :compile ] do |task|
          cmd = [ 'gdb' ] + GDB_OPTIONS
          cmd += [ '--args' ]
          cmd += CONSOLE_CMD
          run( *cmd )
  end

  desc "Run console under Valgrind."
  task :valgrind => [ :compile ] do |task|
          cmd = [ 'valgrind' ] + VALGRIND_OPTIONS
          cmd += CONSOLE_CMD
          run( *cmd )
  end
end

task :default => :spec

def run *cmd
  sh(cmd.join(" "))
end

namespace :spec do
  # partial-loads-ok and undef-value-errors necessary to ignore
  # spurious (and eminently ignorable) warnings from the ruby
  # interpreter

  RSPEC_CMD = [ 'ruby', '-S', 'rspec', '-Ilib:ext', SPECDIR.to_s ]

  desc "Run specs under GDB."
  task :gdb => [ :compile ] do |task|
          cmd = [ 'gdb' ] + GDB_OPTIONS
    cmd += [ '--args' ]
    cmd += RSPEC_CMD
    run( *cmd )
  end

  desc "Run specs under cgdb."
  task :cgdb => [ :compile ] do |task|
    cmd = [ 'cgdb' ] + GDB_OPTIONS
    cmd += [ '--args' ]
    cmd += RSPEC_CMD
    run( *cmd )
  end

  desc "Run specs under Valgrind."
  task :valgrind => [ :compile ] do |task|
    cmd = [ 'valgrind' ] + VALGRIND_OPTIONS
    cmd += RSPEC_CMD
    run( *cmd )
  end

  desc "Run specs under Callgrind."
  task :callgrind => [ :compile ] do |task|
    cmd = [ 'valgrind' ] + CALLGRIND_OPTIONS
    cmd += RSPEC_CMD
    run( *cmd )
  end

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
