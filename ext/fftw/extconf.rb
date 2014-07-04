require 'mkmf'
require 'colorize'

LIBDIR = RbConfig::CONFIG['libdir']
INCLUDEDIR = RbConfig::CONFIG['includedir']
NMATRIX_DIR = ENV['GEM_HOME'] + '/gems/nmatrix-0.1.0.rc3/'
NMATRIX_LIBDIR = NMATRIX_DIR + 'lib/nmatrix/'
NMATRIX_INCLUDEDIR = NMATRIX_DIR + 'ext/nmatrix/'

HEADER_DIRS = ['/usr/local/include',
                INCLUDEDIR,
               '/usr/include',
               '/usr/include/atlas',
               NMATRIX_INCLUDEDIR
              ]
LIB_DIRS = [LIBDIR,NMATRIX_LIBDIR]

dir_config('fftw',HEADER_DIRS,LIBDIR)

if /mingw/ =~ RUBY_PLATFORM then
  FFTW_CONFIG = "sh fftw"
else
  FFTW_CONFIG = "fftw"
end

# set up configurtion
puts "Library directory:" + LIBDIR.green
puts "Include directory:" + INCLUDEDIR.green
puts "Headers directory:" + "#{HEADER_DIRS}".green.to_s
puts '#{sitearchdir}'.red

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

if have_library("fftw3f")
  $CFLAGS = [" -DFFTW3_HAS_SINGLE_SUPPORT -Wall -I #{INCLUDEDIR}"].join(" ")
end

require 'rubygems'
nm_gemspec = Gem::Specification.find_by_path('nmatrix.h')
if nm_gemspec then
  if have_header('nmatrix.h')
    dir_config("cblas")
    dir_config("atlas")
    if have_library("cblas") and have_library("atlas")
      puts "found cblas and atlas".green
    end
  end
end


create_makefile("fftw/fftw")
