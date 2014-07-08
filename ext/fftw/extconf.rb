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
fftw_libdir = RbConfig::CONFIG['libdir']
fftw_incdir = RbConfig::CONFIG['includedir']

def create_conf_h(file)
  print "creating #{file}\n"
  hfile = open(file, "w")
  for line in $defs
    line =~ /^-D(.*)/
    hfile.printf "#define %s 1\n", $1
  end
  hfile.close
end
###############################################################################
#
# README output messages for debugging.
#
###############################################################################
puts "Include directories:".white
puts "#{fftw_incdir}".green
puts "Library directories:".white
puts "#{fftw_libdir}".green

have_type("u_int8_t", fftw_incdir)
have_type("uint8_t", fftw_incdir)
have_type("int16_t", fftw_incdir)
have_type("int32_t", fftw_incdir)
have_type("u_int32_t", fftw_incdir)
have_type("uint32_t", fftw_incdir)
###############################################################################
#
# Make sure CXX is set to g++ and not clang or gcc by setting its value
#
###############################################################################
def gplusplus_version
  cxxvar = proc { |n| `#{CONFIG['CXX']} -E -dM - </dev/null | grep #{n}`.chomp.split(' ')[2] }
  puts "cxxvar:  #{cxxvar}".colorize(:color => :cyan,
                                     :background => :white)

  major = cxxvar.call('__GNUC__')
  minor = cxxvar.call('__GNUC_MINOR__')
  patch = cxxvar.call('__GNUC_PATCHLEVEL__')

  if CONFIG['CXX'] == 'clang++'
    puts "CXX= #{CONFIG['CXX']}"
    $CPP_STANDARD = 'c++11'
    $CPP_FLAGS = '-std=c++11'
    $CXX_FLAGS = '-stc=gnu++11'
    puts "using C++ standard... #{$CPP_STANDARD}".cyan
  else
    version = gplusplus_version
    puts "g++ version is #{version}"
    if version < '4.3.0' && CONFIG['CXX'] == 'g++'  # see if we can find a newer G++, unless it's been overridden by user
      if ! find_newer_gplusplus
        raise("g++ which supports the flags -std=c++0x or -std=c++11. \
               If you're on a Mac and using Homebrew, we recommend using \
               mac-brew-gcc.sh to install a more recent g++.")
      end
      version = gplusplus_version
    end
    puts "g++ reports version... " +
    `#{CONFIG['CXX']} --version|head -n 1|cut -f 3 -d " "`.colorize(:color => :cyan ) 
  end
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
nmatrix_config = dir_config('nmatrix','ext/nmatrix','lib/nmatrix')
if nmatrix_config then

  puts "nmatrix_config found in #{nmatrix_config}".green
  unless have_library 'nmatrix', 'nmatrix/version'  %w(nmatrix/header.h)
    puts "Library directories for NMatrix: Not Found!".colorize(:color => :red,
                                                                :background => :white)
  end
  unless find_header('nmatrix.h')
    abort "nmatrix is missing.  please install nmatrix".red
  end
  unless find_header('data.h')
    puts "Headers nmatrix.h and data.h are found!".colorize(:color => :red,
                                                            :background => :white)
  end
end
###############################################################################
#
# Configuration of directory named in first argument, i.e. The arguments of
# dir_config('fftw', HEADERS, LIBS) are as follows (in this case):
#
# HEADERS == fftw_incdir
# LIBS    == fftw_libdir
#
###############################################################################
fftw_incdir = ['/usr/local/include',
                fftw_incdir,
               '/usr/include',
               '/usr/include/atlas'
              ]
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
