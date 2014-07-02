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

if /mingw/ =~ RUBY_PLATFORM then
  FFTW_CONFIG = "sh fftw"
else
  FFTW_CONFIG = "fftw"
end

def fftw_config()
  print("checking fftw_cflags... ")
  IO.popen("#{FFTW_CONFIG} --cflags") do |f|
    cflags = f.gets.chomp
    puts(cflags)
    $CFLAGS += " " + cflags
  end
  # This is for nmatrix stuff late on TODO
  IO.popen("#{FFTW_CONFIG} --libs") do |f|
    libs = f.gets.chomp
    dir_config("cblas")
    dir_config("atlas")
    if have_library("cblas") and have_library("atlas")
      libs.gsub!("-lfftwfblas", "-lcblas -latlas")
      $LOCAL_LIBS += " " + libs.gsub(" -lfftwcblas", "")
      print("checking fftw libs... ")
      puts(libs)
    end
  end

  if have_library("fftw3f")
    $CFLAGS = [" -DFFTW3_HAS_SINGLE_SUPPORT -Wall -I #{INCLUDEDIR}"].join(" ")
  end
  common_flags ="-lfftw3 -Wall -Werror=return-type"
  $CFLAGS   = [common_flags,$CFLAGS].join(" ")
  $CXXFLAGS = [common_flags,$CXXFLAGS].join(" ")
  $CPPFLAGS = [common_flags,$CPPFLAGS].join(" ")

  # set up configurtion
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
# TODO FFTW for i866 or x86-64 Computers
#'--enable-float  --enable-threads  --enable-sse'
end

create_makefile("fftw/fftw")
