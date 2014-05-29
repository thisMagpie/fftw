require 'mkmf'
require 'rubygems'

# Function derived from NArray's extconf.rb.
def have_type(type, header=nil)
  printf "checking for %s... ", type
  STDOUT.flush

  src = <<"SRC"
#include <ruby.h>
SRC


  src << <<"SRC" unless header.nil?
#include "#{header}"
SRC

  r = try_link(src + <<"SRC")
  int main() { return 0; }
  int t() { #{type} a; return 0; }
SRC

  unless r
    print "no\n"
    return false
  end

  $defs.push(format("-DHAVE_%s", type.upcase))

  print "yes\n"

  return true
end


module NMatrix_FFTW
  class Version
    def initialize(str)
      @str = str
      @ary = str.split(".").collect { |elm| elm.to_i }
    end
    def to_s; @str; end
    def inspect; @str; end
    def >=(ver)
      ary2 = ver.split(".").collect { |elm| elm.to_i }
      if @ary[0] > ary2[0]; return true; end
      if @ary[0] < ary2[0]; return false; end
      if @ary[1] > ary2[1]; return true; end
      if @ary[1] < ary2[1]; return false; end
      if @ary.size < ary2.size; return false; end
      if @ary.size == 3 and ary2.size == 3
        if @ary[2] < ary2[2]; return false; end
      end
      return true
    end
    def <(ver)
      ary2 = ver.split(".").collect { |elm| elm.to_i }
      if @ary[0] >= ary2[0]; return false; end
      if @ary[0] >= ary2[0]; return false; end
      return true
    end
  end
end

def fftw3_config()
  print("checking fftw cflags... ")
  IO.popen('#{NMatrix_FFTW_CONFIG} --cflags') do |f|
    cflags = f.gets.chomp
    puts(cflags)
    $CFLAGS += ' ' + cflags
  end

  IO.popen('#{NMatrix_FFTW_CONFIG} --libs') do |f|
    libs = f.gets.chomp
    dir_config("cblas")
    dir_config("atlas")
    if have_library('cblas') and have_library('atlas')
      libs.gsub!('-lfftw3cblas', '-lcblas -latlas')
      $LOCAL_LIBS += ' ' + libs.gsub(' -lfftw3cblas', '')
      print("checking fftw3 libs... ")
      puts(libs)
    else
      print("checking fftw3 libs... ")
      puts(libs)
      $LOCAL_LIBS += " " + libs
    end
  end
end
# Function derived from NArray's extconf.rb.
def create_conf_h(file) #:nodoc:
  print "creating #{file}\n"
  File.open(file, 'w') do |hfile|
    header_guard = file.upcase.sub(/\s|\./, '_')

    hfile.puts "#ifndef #{header_guard}"
    hfile.puts "#define #{header_guard}"
    hfile.puts

    for line in $defs
      line =~ /^-D(.*)/
      hfile.printf "#define %s 1\n", $1
    end

    hfile.puts
    hfile.puts "#endif"
  end
end

nmatrix_config = dir_config('../nmatrix',$sitearchdir,$sitearchdir)
begin
  require 'rubygems'
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

dir_config('fftw3/include','/usr/include',$sitearchdir)

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

if /cygwin|mingw/ =~ RUBY_PLATFORM
   have_library("nmatrix") || raise("ERROR: nmatrix library is not found")
end

create_makefile("fftw3/fftw3")
