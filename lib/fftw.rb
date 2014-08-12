require 'nmatrix'
require 'colorize'

# Check the library .so file exists
# @param str 
#            the path to the file
# @param     the file to be required
# @return 	 void
def check_exists(str, req)
  if File.exist?(str)
  	require str #=> Only require './lib/fftw/fftw.so' if found first
    puts "#{str} found!".green #=>
  else
    puts "#{str} not found!".red #=>
  end
end
check_exists("./lib/fftw/fftw.so",'fftw.so')
