# At the moment it does not do much nor is is properly checked but see:
# http://stackoverflow.com/questions/15031544/extract-fast-fourier-transform-data-from-file
# and http://rubydoc.info/gems/ruby-audio/1.6.1/frames 
# for more info on where it is going

require 'rubygems'
require 'nmatrix'
require "numru/fftw3"
require 'fftw3/fftw3'
require 'ruby-audio'
include NumRu

print "\n** audio IO ** \n\n"

buffer = RubyAudio::buffer.float(1024)

RubyAudio::Sound.open(ARGV[0]) do |snd|
  while snd.read(buffer) != 0
      NArray.to_na(buffer.to_a)
      buffer.close()
  end
end



