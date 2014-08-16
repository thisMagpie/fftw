require 'mkmf'
require 'colorize'

###############################################################################
# INFO
# debug method for displaying info log messages
# messages print to the console and are highlighted
# with black font on yellow background
# TODO put somewhere global
###############################################################################
def info()
  return " INFO: ".colorize(:color       => :black,
                            :background  => :yellow) + " "
end
###############################################################################
# FAILURE
# debug method for displaying failure log messages
# messages print to the console and are highlighted
# with black font on red background
# TODO put somewhere global
###############################################################################
def failure()
  return " FAILURE: ".colorize(:color       => :black,
                               :background  => :red) + " "
end
###############################################################################
# SUCCESS
# debug method for displaying success log messages
# messages print to the console and are highlighted
# with black font on red background
# TODO put somewhere global
###############################################################################
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
fftw_srcdir = RbConfig::CONFIG['srcdir']
###############################################################################
#
# README output messages for debugging.
#
###############################################################################
puts "#{info} Include directories #{fftw_incdir}"
puts "#{info} Library directories #{fftw_libdir}"
puts "#{info} src directory #{fftw_srcdir}"
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
$CXX_FLAGS = '-std=c++11 ` --cppflags --cxxflags --ldflags --libs` `pkg-config fftw3 --libs` -g -Wall'

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

incdir = ['/usr/local/include',
                fftw_incdir,
               '/usr/include',
               '/usr/include/atlas',
              ]

flags = " --include=#{fftw_incdir} --libdir=#{fftw_libdir}"

if have_library("fftw3") then
  $CFLAGS += [" -lfftw3 -lm #{$CFLAGS} #{$flags}"].join(" ")
  puts "#{success} fftw3 found... Adding '-lfftw3 -lm' to cflags"
  puts info + $CFLAGS
else
  puts "#{failure} fftw3 not found #{$CFLAGS}"
end

if have_library("fftw3f") then
  $CFLAGS = [" -lfftw3f #{$CFLAGS}"].join(" ")
  puts "#{success} fftw3 found... Adding '-lfftw3f' to cflags"
  puts info + $CFLAGS
else
  $CFLAGS = ["#{$CFLAGS}"].join(" ")
  puts "#{failure} fftw3f not found #{$CFLAGS}"
end

puts "#{info} CFLAGS : #{$CFLAGS}"
puts "#{info} flags : #{$flags}"
puts `cd #{fftw_srcdir}/fftw3; echo $PWD; ./configure;make;make install`
dir_config('fftw', fftw_incdir, fftw_libdir)


print "#{info} creating fftw_config.h \n"
hfile = open('fftw_config.h', "w")
for line in $defs
  line =~ /^-D(.*)/
  hfile.printf "#define %s 1\n", $1
end
hfile.close

create_makefile("fftw/fftw")