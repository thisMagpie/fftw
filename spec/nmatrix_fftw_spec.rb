require 'spec_helper'
require 'nmatrix'
require 'ffi'

class FFTW
  extend FFI::Library
  ffi_lib FFI::Library::LIBC
  attach_function :puts, [ :string ], :int
end
