//
// A fast fast fourier library for scientific computation in Ruby.
// FFTW3 is part of SciRuby.
// Please see LICENSE.txt for additional copyright notices.
//
// == Contributing
//
// By contributing source code to SciRuby, you agree to be bound by
// our Contributor Agreement:
//
// * https://github.com/SciRuby/sciruby/wiki/Contributor-Agreement
//
// == ruby_object.h
//
// Functions and classes for dealing with Ruby objects.

#ifndef RUBY_OBJECT_H
#define RUBY_OBJECT_H

/*
 * Standard Includes
 */
#include <ruby.h>
#include <iostream>
#include <type_traits>
/*
 * Classes and Functions
 */

namespace fftw3 {

template<typename T, typename U>
struct made_from_same_template : std::false_type {}; 
 
template<template<typename> class Templ, typename Arg1, typename Arg2>
struct made_from_same_template<Templ<Arg1>, Templ<Arg2>> : std::true_type {};

class RubyObject {
	public:
	VALUE rval;
	
	/*
	 * Value constructor.
	 */
	inline RubyObject(VALUE ref = Qnil) : rval(ref) {}
	
	/*
	 * Integer constructor.
	 *
	 * Does not work as a template.
	 */
	inline RubyObject(uint8_t other)  : rval(INT2FIX(other)) {}
	inline RubyObject(int8_t other)   : rval(INT2FIX(other)) {}
	inline RubyObject(int16_t other)  : rval(INT2FIX(other)) {}
	inline RubyObject(uint16_t other) : rval(INT2FIX(other)) {}
	inline RubyObject(int32_t other)  : rval(INT2FIX(other)) {}

	// there is no uint32_t here because that's a Ruby VALUE type,
  // and we need the compiler to treat that as a VALUE.
	inline RubyObject(int64_t other)  : rval(INT2FIX(other)) {}

	/*
	 * Float constructor.
	 *
	 * Does not work as a template. ???
	 */
	inline RubyObject(float other)   : rval(rb_float_new(other)) {}
	inline RubyObject(double other)  : rval(rb_float_new(other)) {}

  /*
   * Operators for converting RubyObjects to other C types.
   */
#define RETURN_OBJ2NUM(mac)   if (this->rval == Qtrue) return 1;
                              else if (this->rval == Qfalse) return 0;
                              else return mac(this->rval);

  inline operator int8_t()  const { RETURN_OBJ2NUM(NUM2INT)         }
  inline operator uint8_t() const { RETURN_OBJ2NUM(NUM2UINT)        }
  inline operator int16_t() const { RETURN_OBJ2NUM(NUM2INT)         }
  inline operator uint16_t() const { RETURN_OBJ2NUM(NUM2UINT)       }
  inline operator int32_t() const { RETURN_OBJ2NUM(NUM2LONG)        }
  inline operator int64_t() const { RETURN_OBJ2NUM(NUM2LONG)        }
  inline operator uint64_t() const { RETURN_OBJ2NUM(NUM2ULONG)      }
  inline operator double()   const { RETURN_OBJ2NUM(NUM2DBL)        }
  inline operator float()  const { RETURN_OBJ2NUM(NUM2DBL)          }

	////////////////////////////
	// RUBY-NATIVE OPERATIONS //
	////////////////////////////
	template <typename NativeType, typename = typename std::enable_if<std::is_arithmetic<NativeType>::value>::type>
	inline bool operator==(const NativeType& other) const {
		return *this == RubyObject(other);
	}

  template <typename NativeType, typename = typename std::enable_if<std::is_arithmetic<NativeType>::value>::type>
	inline bool operator!=(const NativeType& other) const {
		return *this != RubyObject(other);
	}

	/*
	 * Convert a Ruby object to an integer.
	 */
	template <typename IntType>
	inline typename std::enable_if<std::is_integral<IntType>::value, IntType>::type to(void) {
		return NUM2INT(this->rval);
	}
	
	/*
	 * Convert a Ruby object to a floating point number.
	 */
	template <typename FloatType>
	inline typename std::enable_if<std::is_floating_point<FloatType>::value, FloatType>::type to(void) {
		return NUM2DBL(this->rval);
	}


////////////////////////////
// NATIVE-RUBY OPERATIONS //
////////////////////////////

template <typename NativeType, typename = typename std::enable_if<std::is_arithmetic<NativeType>::value>::type>
inline RubyObject operator/(const NativeType left, const RubyObject& right) {
  return RubyObject(left) / right;
}

template <typename NativeType, typename = typename std::enable_if<std::is_arithmetic<NativeType>::value>::type>
inline bool operator==(const NativeType left, const RubyObject& right) {
  return RubyObject(left) == right;
}

template <typename NativeType, typename = typename std::enable_if<std::is_arithmetic<NativeType>::value>::type>
inline bool operator!=(const NativeType left, const RubyObject& right) {
  return RubyObject(left) != right;
}

template <typename NativeType, typename = typename std::enable_if<std::is_arithmetic<NativeType>::value>::type>
inline bool operator<=(const NativeType left, const RubyObject& right) {
  return RubyObject(left) <= right;
}

template <typename NativeType, typename = typename std::enable_if<std::is_arithmetic<NativeType>::value>::type>
inline bool operator>=(const NativeType left, const RubyObject& right) {
  return RubyObject(left) >= right;
}

template <typename NativeType, typename = typename std::enable_if<std::is_arithmetic<NativeType>::value>::type>
inline bool operator<(const NativeType left, const RubyObject& right) {
  return RubyObject(left) < right;
}

template <typename NativeType, typename = typename std::enable_if<std::is_arithmetic<NativeType>::value>::type>
inline bool operator>(const NativeType left, const RubyObject& right) {
  return RubyObject(left) > right;
}


} // end of namespace nm

namespace std {
  inline nm::RubyObject abs(const nm::RubyObject& obj) {
    return obj.abs();
  }


  inline nm::RubyObject sqrt(const nm::RubyObject& obj) {
    VALUE cMath = rb_const_get(rb_cObject, rb_intern("Math"));
    return nm::RubyObject(rb_funcall(cMath, rb_intern("sqrt"), 1, obj.rval));
  }
}

#endif // RUBY_OBJECT_H
