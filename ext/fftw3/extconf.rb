require "mkmf"
require "rubygems"

nmatrix_dir = File.dirname(Gem.find_files("nmatrix.h").first) rescue $sitearchdir
dir_config('nmatrix', nmatrix_dir, nmatrix_dir)
dir_config('fftw3','/usr/local')

if ( ! have_header("nmatrix.h") && have_header("nmatrix_config.h") ) then
   print <<-EOS
   ** configure error **

   Header nmatrix.h or nmatrix_config.h was not found. If you have these files in
   /nmatrixdir/include, try the following:

   % ruby extconf.rb --with-nmatrix-include=/nmatrix-dir/include

EOS
   exit(-1)
end

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
  $CFLAGS += ' -DFFTW3_HAS_SINGLE_SUPPORT'
end

if /cygwin|mingw/ =~ RUBY_PLATFORM
   have_library("nmatrix") || raise("ERROR: nmatrix library is not found")
end

create_makefile("fftw3/fftw3")
