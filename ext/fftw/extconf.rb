require 'mkmf'
require 'colorize'
require 'rubygems'
require 'nmatrix'
puts "******************".colorize(:color => :light_blue,
                                   :background => :light_red)
puts "******************".light_blue
puts "* COMPILING FFTW *".cyan
puts "******************".light_blue
puts "******************".colorize(:color => :light_blue,
                                   :background => :light_red)

###############################################################################
#
# This only works if fftw3 headers are installed
#
# HEADERS == fftw_incdir
# LIBS    == fftw_libdir
#
###############################################################################
fftw_libdir    = RbConfig::CONFIG['libdir']
fftw_incdir    = RbConfig::CONFIG['includedir']

###############################################################################
#
# Make sure CXX is set to g++ and not clang or gcc by setting its value
#
###############################################################################


def gplusplus_version #:nodoc:
  cxxvar = proc { |n| `#{CONFIG['CXX']} -E -dM - </dev/null | grep #{n}`.chomp.split(' ')[2] }
  major = cxxvar.call('__GNUC__')
  minor = cxxvar.call('__GNUC_MINOR__')
  patch = cxxvar.call('__GNUC_PATCHLEVEL__')

  raise("unable to determine g++ version (match to get version was nil)") if major.nil? || minor.nil? || patch.nil?

  "#{major}.#{minor}.#{patch}"
end

if CONFIG['CXX'] == 'clang++'
  $CPP_STANDARD = 'c++11'
  $CPP_FLAGS = '-std=c++11'
  $CXX_FLAGS = '-std=gnu++11'
else
  version = gplusplus_version
  if version < '4.3.0' && CONFIG['CXX'] == 'g++'  # see if we can find a newer G++, unless it's been overridden by user
    if !find_newer_gplusplus
      raise("You need a version of g++ which supports -std=c++0x or -std=c++11. If you're on a Mac and using Homebrew, we recommend using mac-brew-gcc.sh to install a more recent g++.")
    end
    version = gplusplus_version
  end

  $CPP_STANDARD = 'c++11'
  puts "using C++ standard... #{$CPP_STANDARD}".cyan
  puts "g++ reports version... " + `#{CONFIG['CXX']} --version|head -n 1|cut -f 3 -d " "`.colorize(:color => :cyan )

end

###############################################################################
#
# TODO | Improve later:
# This assumes that gems are installed in $HOME.
#
###############################################################################
nmatrix_srcdir = ENV['GEM_HOME'] + '/gems/nmatrix'
nmatrix_libdir = nmatrix_srcdir + '/lib'
nmatrix_incdir = nmatrix_srcdir + '/ext/nmatrix'

###############################################################################
# 
# Seek out nmatrix.gemspec and if it is found search for the libraries.
###############################################################################
nm_gemspec = Gem::Specification.find_by_path('nmatrix.h')
if nm_gemspec then
  puts "nmatrix.gemspec found!".colorize(:color => :green,
                                         :background => :white)
  puts "nmatrix#{nm_gemspec.version}".colorize(:color => :black,
                                               :background =>  :green)

  puts "Searching for cblas and atlas...".white
  if have_library("cblas") and have_library("atlas")
    puts "******************************".colorize(:color => :black,
                                                   :background => :cyan)
    puts "CBLAS and ATLAS Status: Found!".green
    puts "******************************".colorize(:color => :black,
                                                   :background => :cyan)
    dir_config("cblas")
    dir_config("atlas")
  else
    puts "CBLAS and ATLAS Status: Not found!".red
  end

  puts "Searching for NMatrix...".white
  if have_library('nmatrix')
    puts "Library directories for NMatrix: Found!".colorize(:color => :geen,
                                                            :background => :white)
  end
  if find_header('nmatrix.h') and find_header('data.h')
    puts "Headers nmatrix.h and data.h are found!".colorize(:color => :geen,
                                                            :background => :white)
  end
end

fftw_incdir = ['/usr/local/include',
                fftw_incdir,
               '/usr/include',
               '/usr/include/atlas',
                nmatrix_incdir
              ]
fftw_libdir = [ fftw_libdir, nmatrix_libdir ]



###############################################################################
#
# README output messages for debugging.
#
###############################################################################
puts "nmatrix include directory:".white
puts "#{nmatrix_incdir}".green
puts "nmatrix library directories:".white
puts "#{nmatrix_libdir}".green

###############################################################################
#
# Configuration of directory named in first argument, i.e. The arguments of
# dir_config('fftw', HEADERS, LIBS) are as follows (in this case):
#
# HEADERS == fftw_incdir
# LIBS    == fftw_libdir
#
###############################################################################
incdir, libdir = dir_config('fftw', fftw_incdir, fftw_libdir)

if have_library("fftw3f") then
  $CFLAGS = [" -DFFTW3_HAS_SINGLE_SUPPORT -Wall -I #{fftw_incdir}"].join(" ")
end


if $warnflags then
  $warnflags.slice!('-Wdeclaration-after-statement')
  $warnflags.slice!('-Wimplicit-function-declaration')
end
create_makefile("fftw/fftw")