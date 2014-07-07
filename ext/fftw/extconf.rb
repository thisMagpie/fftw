require 'mkmf'
require 'colorize'

puts "####################".colorize(:color => :cyan,
                                     :background => :white)
puts "####################".colorize(:color => :light_blue,
                                     :background => :light_red)
puts "## COMPILING FFTW ##".colorize(:color => :white,
                                     :background => :blue)
puts "####################".colorize(:color => :light_blue,
                                     :background => :light_red)
puts "####################".colorize(:color => :cyan,
                                     :background => :white)
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
# README output messages for debugging.
#
###############################################################################
puts "Include directories:".white
puts "#{fftw_incdir}".green
puts "Library directories:".white
puts "#{fftw_libdir}".green

###############################################################################
#
# Make sure CXX is set to g++ and not clang or gcc by setting its value
#
###############################################################################
def gplusplus_version
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
  $CXX_FLAGS = '-stc=gnu++11'
  puts "using C++ standard... #{$CPP_STANDARD}".cyan
else
  version = gplusplus_version
  if version < '4.3.0' && CONFIG['CXX'] == 'g++'  # see if we can find a newer G++, unless it's been overridden by user
    if !find_newer_gplusplus
      raise("g++ which supports the flags -std=c++0x or -std=c++11. \
             If you're on a Mac and using Homebrew, we recommend using \
             mac-brew-gcc.sh to install a more recent g++.")
    end
    version = gplusplus_version
  end
  puts "g++ reports version... " +
  `#{CONFIG['CXX']} --version|head -n 1|cut -f 3 -d " "`.colorize(:color => :cyan )

end

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
  puts "Library directories for NMatrix: Found!".colorize(:color => :green,
                                                          :background => :white)
end
if find_header('nmatrix/nmatrix.h') and find_header('data.h')
  puts "Headers nmatrix.h and data.h are found!".colorize(:color => :geen,
                                                          :background => :white)
end

fftw_incdir = ['/usr/local/include',
                fftw_incdir,
               '/usr/include',
               '/usr/include/atlas'
              ]

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
else
  $CFLAGS = ["-Wall -I #{fftw_incdir} --libdir=fftw_libdir"].join(" ")
end

if $warnflags then
  $warnflags.slice!('-Wdeclaration-after-statement')
  $warnflags.slice!('-Wimplicit-function-declaration')
  $warnflags.slice!('-Wshorten-64-to-32')
end

$CFLAGS   += " -static -O3"
$CPPFLAGS += " -O3"

create_makefile("fftw/fftw")