require 'mkmf'
require 'rubygems'

dir_config('include','/usr/include',$sitearchdir)
if ( ! have_header("fftw3.h") && have_library("fftw3") ) then
   print <<EOS
   ** configure error **

   Header fftw3.h or the compiled fftw3 library is not found.
   If you have the library installed under /fftw3-dir (that is, fftw3.h is
   in /fftw3-dir/include and the library in /fftw3-dir/lib/),
   try the following:
   % ruby extconf.rb --with-fftw3-dir=/fftw3-dir

   Alternatively, you can specify the two directory separately
   with --include-dir and --fftw3-dir
EOS
  exit(-1)
end

if have_library("fftw3f")
 $CFLAGS += ' -DFFTW3_HAS_SINGLE_SUPPORT -Wall -I../usr/include'
end

create_makefile("nmatrix-fftw")
