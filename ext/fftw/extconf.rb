require 'mkmf'
require 'colorize'
require 'rubygems'

puts "******************".colorize(:color => :light_blue,
                                   :background => :light_red)
puts "******************".light_blue
puts "* COMPILING FFTW *".cyan
puts "******************".light_blue
puts "******************".colorize(:color => :light_blue,
                                              :background => :light_red)

# This only works if fftw3 headers are installed.
fftw_libdir    = RbConfig::CONFIG['libdir']
fftw_incdir    = RbConfig::CONFIG['includedir']

# TODO | Improve later:
# This assumes that gems are installed in $HOME.
nmatrix_srcdir = ENV['GEM_HOME'] + '/gems/nmatrix'
nmatrix_libdir = nmatrix_srcdir + '/lib/nmatrix'
nmatrix_incdir = nmatrix_srcdir + '/ext/nmatrix'

nm_gemspec = Gem::Specification.find_by_path('nmatrix.h')
if nm_gemspec then
  puts "nmatrix.gemspec found!".green
  puts "**********************".cyan
  puts "Searching for cblas and atlas...".yellow
  puts "CBLAS and ATLAS Status:".yellow
  if have_library("cblas") and have_library("atlas")
    puts "******".cyan # TODO stars always need to be same length as string.
    puts "Found!".green
    puts "******".cyan # TODO stars always need to be same length as string.
    dir_config("cblas")
    dir_config("atlas")
  else
    puts "Not found!".red
  end

  # Searching for nmatrix.
  if (have_header('nmatrix.h') && have_library('nmatrix'))

    puts "NMatrix Status:".yellow
    puts "Found!".green
    puts "******".cyan
  else
    puts "NMatrix Not found!".red
  end
end

headers = ['/usr/local/include',
                fftw_incdir,
               '/usr/include',
               '/usr/include/atlas',
                nmatrix_incdir
              ]
fftw_libdir = [ fftw_libdir, nmatrix_libdir ]

puts "*********************************************************".cyan
puts "nmatrix include directory:"
puts "#{nmatrix_incdir}".green
puts "nmatrix library directories:"
puts "#{nmatrix_libdir}".green
puts "*********************************************************".cyan

dir_config('nmatrix', nmatrix_incdir, nmatrix_libdir)
incdir, libdir = dir_config('fftw', fftw_incdir, fftw_libdir)

if /mingw/ =~ RUBY_PLATFORM then
  FFTW_CONFIG = "sh fftw"
else
  FFTW_CONFIG = "fftw"
end

# set up configurtio
puts "Library directory:" + libdir.to_s.green
puts "Headers directory:" + "#{incdir}".green.to_s

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
  $CFLAGS = [" -DFFTW3_HAS_SINGLE_SUPPORT -Wall -I #{fftw_incdir}"].join(" ")
end

if $warnflags
  $warnflags.slice!('-Wdeclaration-after-statement')
  $warnflags.slice!('-Wimplicit-function-declaration')
end

create_makefile("fftw/fftw")
