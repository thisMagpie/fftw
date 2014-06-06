require 'mkmf'
require 'rubygems'
require 'nmatrix'

nmatrix_config = dir_config('../nmatrix',$sitearchdir,$sitearchdir)
begin
  nm_gemspec=Gem::Specification.find_by_path('nmatrix.h')
  if nm_gemspec
    nmatrix_config = File.join(nm_gemspec.full_gem_path, nm_gemspec.require_path)
    $CPPFLAGS = " -I#{nmatrix_config} "+$CPPFLAGS
  end
rescue LoadError
end
have_nmatrix_h = have_header("nmatrix.h")
if nmatrix_config
  if RUBY_PLATFORM =~ /cygwin|mingw/
    have_library("nmatrix")
  end
end

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
 $CFLAGS += ' -DFFTW3_HAS_SINGLE_SUPPORT -Wall -I../include'
end

if /cygwin|mingw/ =~RUBY_PLATFORM
   have_library("nmatrix") || raise("ERROR: nmatrix library is not found")
end

create_makefile("fftw")
