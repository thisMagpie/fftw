require 'mkmf'
require 'colorize'

LIBDIR = RbConfig::CONFIG['libdir']
INCLUDEDIR = RbConfig::CONFIG['includedir']
HEADER_DIRS = ['/opt/local/include',
               '/usr/local/include',
                INCLUDEDIR,
               '/usr/include',
               '/usr/include/atlas']

puts "Library directory:" + LIBDIR.yellow
puts "Include directory:" + INCLUDEDIR.yellow
puts "Headers directory:" + "#{HEADER_DIRS}".yellow.to_s

dir_config('fftw',HEADER_DIRS,LIBDIR)
if ( ! have_header("fftw3.h") && have_library("fftw3") ) then
   print <<-EOS
   ** configure error **

   Header fftw.h or the compiled fftw library is not found.
   If you have the library installed under /fftw-dir (that is, fftw3.h is
   in /fftw dir/include and the library in /fftw-dir/lib/),
   try the following:
   % ruby extconf.rb --with-fftw-dir=/fftw-dir

   Alternatively, you can specify the two directory separately
   with --include-dir and --fftw-dir
  EOS
  exit(-1)
end

def find_newer_gplusplus #:nodoc:
  print "checking for apparent GNU g++ binary with C++0x/C++11 support... "
  [9,8,7,6,5,4,3].each do |minor|
    ver = "4.#{minor}"
    gpp = "g++-#{ver}"
    result = `which #{gpp}`
    next if result.empty?
    CONFIG['CXX'] = gpp
    puts ver
    return CONFIG['CXX']
  end
  false
end

if have_library("fftw3f")
 $CFLAGS += ' -DFFTW3_HAS_SINGLE_SUPPORT -Wall -I #{INCLUDEDIR}'
end

# TODO FFTW for i866 or x86-64 Computers
#'--enable-float  --enable-threads  --enable-sse'

create_makefile("fftw")
