require 'mkmf'
require 'colorize'

###############################################################################
# INFO
# debug method for displaying info log messages
# messages print to the console and are highlighted
# with black font on yellow background
###############################################################################
def info() #=> TODO put somewhere global
  return " INFO: ".colorize(:color       => :black,
                            :background  => :yellow) + " "
end
###############################################################################
# FAILURE
# debug method for displaying failure log messages
# messages print to the console and are highlighted
# with black font on red background
###############################################################################
def failure() #=> TODO put somewhere global
  return " FAILURE: ".colorize(:color       => :black,
                               :background  => :red) + " "
end
###############################################################################
# SUCCESS
# debug method for displaying success log messages
# messages print to the console and are highlighted
# with black font on red background
###############################################################################
def success() #=> TODO put somewhere global
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
puts "#{info} src directory #{fftw_srcdir}\n\n"
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
puts "#{info} src directory #{fftw_srcdir}"


$CPP_STANDARD = 'c++11'
$CPP_FLAGS = '-std=c++11'
$CXX_FLAGS = '-std=c++11 ` --cppflags --cxxflags --ldflags --libs` `pkg-config -lfftw3l --libs` -g -Wall'

puts info + `g++ --version`

`#{CONFIG['CXX']} --version|head -n 1|cut -f 3 -d " "`

puts `cd #{fftw_srcdir}/fftw3; ./configure --prefix=#{fftw_srcdir} --include=#{fftw_incdir} --libdir=#{fftw_libdir} --enable-debug; make; make install`

dir_config('fftw')

# Configuration of directory named in first argument:
# @param name: name of library to be found
# @return whether the library was found
def header_configs(name)
  puts "#{info} Searching for #{name}..."
  if have_library(name)
    puts "#{success} #{name} Found!"
    dir_config(name)
    return true
  else
    puts "#{failure} #{name} not found!"
    return false
  end
end
header_configs("blas")
header_configs("atlas")

print "#{info} creating fftw_config.h \n"
hfile = open('fftw_config.h', "w")
for line in $defs
  line =~ /^-D(.*)/
  hfile.printf "#define %s 1\n", $1
end
hfile.close

create_makefile("fftw/fftw")
