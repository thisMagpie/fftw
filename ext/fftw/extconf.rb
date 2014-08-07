require 'mkmf'
require 'colorize'

# INFO
def info()
  return " INFO: ".colorize(:color       => :black,
                            :background  => :yellow) + " "
end
# FAILURE
def failure()
  return " FAILURE: ".colorize(:color       => :black,
                               :background  => :red) + " "
end
# SUCCESS
def success()
  return " SUCCESS: ".colorize(:color       => :black,
                               :background  => :green) + " "
end
###############################################################################
#
# Install Message
#
###############################################################################
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

abort "missing malloc()".red unless have_func "malloc"
abort "missing free()".red   unless have_func "free"
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
###############################################################################
#
# README output messages for debugging.
#
###############################################################################
puts "#{info} Include directories #{fftw_incdir}"
puts "#{info} Library directories #{fftw_libdir}"
###############################################################################
#
# Make sure CXX is set to g++ and not clang or gcc by setting its value
#
###############################################################################
cxx_proc = proc { |n| `#{CONFIG['CXX']} -E -dM - </dev/null | grep #{n}`.chomp.split(' ')[2] }
major = cxx_proc.call('__GNUC__')
minor = cxx_proc.call('__GNUC_MINOR__')
patch = cxx_proc.call('__GNUC_PATCHLEVEL__')
puts "#{info} CXX = #{CONFIG['CXX']}"

$CPP_STANDARD = 'c++11'
$CPP_FLAGS = '-std=c++11'
$CXX_FLAGS = '-stc=++11 `Magick++-config --cppflags --cxxflags --ldflags --libs` `pkg-config fftw3 --libs` -g -Wall -O0'

puts info + `g++ --version`
puts "#{info} CPP_STANDARD is #{$CPP_STANDARD}"

`#{CONFIG['CXX']} --version|head -n 1|cut -f 3 -d " "`

# Configuration of directory named in first argument:
# cblas and atlas in this case.
def header_configs()
  puts "#{info} Searching for cblas and atlas..."
  if have_library("cblas") and have_library("atlas")
    puts "#{success} CBLAS and ATLAS Status Found!"
    dir_config("cblas")
    dir_config("atlas")
  else
    puts "#{failure} CBLAS and ATLAS not found!"
  end
end
header_configs

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
               '/usr/include/atlas',
              ]
incdir, libdir = dir_config('fftw', fftw_incdir, fftw_libdir)

puts "#{info} libdir=#{libdir}"
puts "#{info} incdir=#{incdir}"
###############################################################################
#
# Configuration of directory named in first argument, i.e. The arguments of
# dir_config('fftw', HEADERS, LIBS) are as follows (in this case):
#
# HEADERS == fftw_incdir
# LIBS    == _libdir
#
#############################################################################
flags = " -I#{incdir} --libdir=#{libdir} --enable-float"
if have_library("fftw3f") then
  $CFLAGS = [" -DFFTW3_HAS_SINGLE_SUPPORT #{flags}"].join(" ")
  puts "#{info} -DFFTW3_HAS_SINGLE_SUPPORT is being used..."
else
  $CFLAGS = [flags].join(" ")
end

if $warnflags then
  $warnflags.slice!('-Wdeclaration-after-statement')
  $warnflags.slice!('-Wimplicit-function-declaration')
  $warnflags.slice!('-Wshorten-64-to-32')
end

$CFLAGS   += " -static -O5"
$CPPFLAGS += " -O5"

print "#{info} creating fftw_config.h \n"
hfile = open('fftw_config.h', "w")
for line in $defs
  line =~ /^-D(.*)/
  hfile.printf "#define %s 1\n", $1
end
hfile.close

create_makefile("fftw/fftw")
