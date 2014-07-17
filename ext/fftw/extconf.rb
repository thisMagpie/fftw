require 'mkmf'
require 'colorize'
require 'nmatrix'
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
puts "Include directories:".white
puts "#{fftw_incdir}".green
puts "Library directories:".white
puts "#{fftw_libdir}".green
###############################################################################
#
# Make sure CXX is set to g++ and not clang or gcc by setting its value
#
###############################################################################
cxx_proc = proc { |n| `#{CONFIG['CXX']} -E -dM - </dev/null | grep #{n}`.chomp.split(' ')[2] }
puts "#{cxx_proc}"
major = cxx_proc.call('__GNUC__')
minor = cxx_proc.call('__GNUC_MINOR__')
patch = cxx_proc.call('__GNUC_PATCHLEVEL__')
puts "CXX= #{CONFIG['CXX']}".cyan

$CPP_STANDARD = 'c++11'
$CPP_FLAGS = '-std=c++11'
$CXX_FLAGS = '-stc=++11'

puts `g++ --version`.colorize(:color => :black,
                              :background => :white)
puts "CPP_STANDARD is #{$CPP_STANDARD}".cyan

`#{CONFIG['CXX']} --version|head -n 1|cut -f 3 -d " "`.colorize(:color => :black,
                                                                :background => :white)

puts "Searching for cblas and atlas...".cyan
if have_library("cblas") and have_library("atlas")
  puts "******************************".colorize(:color => :black,
                                                 :background => :cyan)
puts "CBLAS and ATLAS Status: Found!".green
puts "******************************".colorize(:color => :black,
                                              :background => :cyan)
dir_config("cblas")
dir_config("atlas")
else
  puts "CBLAS and ATLAS Status: Not found!".colorize(:color => :black,
                                                     :background => :cyan)
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
               '/usr/include/atlas',
              ]
incdir, libdir = dir_config('fftw', fftw_incdir, fftw_libdir)

if have_library("fftw3f") then
  $CFLAGS = [" -DFFTW3_HAS_SINGLE_SUPPORT -I#{fftw_incdir} --libdir=#{fftw_libdir}"].join(" ")
else
  $CFLAGS = [" -I#{fftw_incdir} --libdir=#{fftw_libdir}"].join(" ")
end

# if $warnflags then
#   $warnflags.slice!('-Wdeclaration-after-statement')
#   $warnflags.slice!('-Wimplicit-function-declaration')
#   $warnflags.slice!('-Wshorten-64-to-32')
# end

$CFLAGS   += " -static -O3"
$CPPFLAGS += " -O3"

print "creating fftw_config.h\n"
hfile = open('fftw_config.h', "w")
for line in $defs
  line =~ /^-D(.*)/
  hfile.printf "#define %s 1\n", $1
end
hfile.close

create_makefile("fftw/fftw")
