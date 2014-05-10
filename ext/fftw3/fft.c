/*
  fftw3_test.c
  Ruby/fftw3: Ruby extension library for fftw3 (GNU Scientific Library)
    (C) Copyright 2004 by Yoshiki Tsunesada

  Ruby/fftw3 is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License.
  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY.
*/

#include "nmatrix_config.h"
#include "fftw3.h"

VALUE mfftw3_fft;
VALUE cfftw3_wavetable;
VALUE cfftw3_complex_wavetable, cfftw3_complex_workspace;
VALUE cfftw3_real_wavetable, cfftw3_halfcomplex_wavetable;
VALUE cfftw3_real_workspace;

extern VALUE cfftw3_vector_int;
extern VALUE cfftw3_vector_complex;

static void fftw3_FFT_Workspace_free(fftw3_FFT_Workspace *space);

static VALUE fftw3_complex_wavetable_new(VALUE klass, VALUE n)
{
  CHECK_FIXNUM(n);
  return Data_Wrap_Struct(cfftw3_complex_wavetable, 0, 
			  fftw3_complex_wavetable_free,
			  fftw3_complex_wavetable_alloc(FIX2INT(n)));
}

static VALUE fftw3_real_wavetable_new(VALUE klass, VALUE n)
{
  CHECK_FIXNUM(n);
  return Data_Wrap_Struct(klass, 0, fftw3_real_wavetable_free,
			  fftw3_real_wavetable_alloc(FIX2INT(n)));
}

static VALUE fftw3_halfcomplex_wavetable_new(VALUE klass, VALUE n)
{
  CHECK_FIXNUM(n);
  return Data_Wrap_Struct(klass, 0, fftw3_halfcomplex_wavetable_free,
			  fftw3_halfcomplex_wavetable_alloc(FIX2INT(n)));
  
}

static void fftw3_FFT_Wavetable_free(fftw3_FFT_Wavetable *table)
{
  fftw3_complex_wavetable_free((fftw3_complex_wavetable *) table);
}

static VALUE fftw3_complex_workspace_new(VALUE klass, VALUE n)
{
  CHECK_FIXNUM(n);
  return Data_Wrap_Struct(klass, 0, fftw3_complex_workspace_free,
			  fftw3_complex_workspace_alloc(FIX2INT(n)));
}

static VALUE fftw3_real_workspace_new(VALUE klass, VALUE n)
{
  CHECK_FIXNUM(n);
  return Data_Wrap_Struct(klass, 0, fftw3_real_workspace_free,
			  fftw3_real_workspace_alloc(FIX2INT(n)));
}

static void fftw3_FFT_Workspace_free(fftw3_FFT_Workspace *space)
{
  fftw3_complex_workspace_free((fftw3_complex_workspace *) space);
}
  
/*
   The FFT methods used to allow passing stride and n values as optional
   parameters to control which elements get transformed.  This created problems
    for Views which can have their own stride, so support for stride and n
parameters to the transform methods is being dropped.  This method used to
be called to determine the stride and n values to use based on the
parameters and/or the vector itself (depending on how many parameters were
passed). Now this function is somewhat unneceesary, but to simplify the code
 refactoring, it has been left in place for the time being.  Eventually it
can be refactored away completely.*/
static VALUE get_complex_stride_n(VALUE obj,
			       fftw3_vector_complex **vin,
			       fftw3_complex_packed_array *data, size_t *stride, size_t *n)
{
  fftw3_vector_complex *v = NULL;

  // obj must be a fftw3::Vector::Complex
  CHECK_VECTOR_COMPLEX(obj);
  Data_Get_Struct(obj, fftw3_vector_complex, v);

  if(vin) *vin = v;
  *data = (fftw3_complex_packed_array) v->data;
  *stride = v->stride;
  *n = v->size;
  return obj;
}

static VALUE fft_complex_radix2(VALUE obj,
                                int (*trans)(fftw3_complex_packed_array, size_t, size_t),
                                int flag) {
  size_t stride, n;

  fftw3_complex_packed_array data;
  fftw3_vector_complex *vin, *vout;

  VALUE ary;

  ary = get_complex_stride_n(obj, &vin, &data, &stride, &n);

  if (flag == RB_fftw3_FFT_COPY) {
    vout = fftw3_vector_complex_alloc(n);
    fftw3_vector_complex_memcpy(vout, vin);
    (*trans)(vout->data, vout->stride, vout->size);
    return Data_Wrap_Struct(cfftw3_vector_complex, 0, fftw3_vector_complex_free, vout);
  } else {
    (*trans)(data, stride, n);
    return ary;
  }
}

static VALUE fftw3_complex_radix2_forward(VALUE obj)
{
  return fft_complex_radix2(obj, fftw3_complex_radix2_forward,
			       RB_fftw3_FFT_COPY);
}

static VALUE fftw3_complex_radix2_forward2(VALUE obj)
{
  return fft_complex_radix2(obj, fftw3_complex_radix2_forward,
			       RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_complex_radix2_transform(VALUE obj, VALUE val_sign)
{
  size_t stride, n;
  fftw3_complex_packed_array data;
  fftw3_direction sign;
  fftw3_vector_complex *vin, *vout;
  sign = NUM2INT(val_sign);
  get_complex_stride_n(obj, &vin, &data, &stride, &n);
  vout = fftw3_vector_complex_alloc(n);
  fftw3_vector_complex_memcpy(vout, vin); 
  fftw3_complex_radix2_transform(vout->data, vout->stride /*1*/, vout->size /*n*/, sign);
  return Data_Wrap_Struct(cfftw3_vector_complex, 0, fftw3_vector_complex_free, vout);
}

static VALUE fftw3_complex_radix2_transform2(VALUE obj, VALUE val_sign)
{
  size_t stride, n;
  fftw3_complex_packed_array data;
  fftw3_direction sign;
  VALUE ary;
  sign = NUM2INT(val_sign);
  ary = get_complex_stride_n(obj, NULL, &data, &stride, &n);
  fftw3_complex_radix2_transform(data, stride, n, sign);
  return ary;
}

static VALUE fftw3_complex_radix2_backward(VALUE obj)
{
  return fft_complex_radix2(obj, fftw3_complex_radix2_backward,
			       RB_fftw3_FFT_COPY);
}

static VALUE fftw3_complex_radix2_inverse(VALUE obj)
{
  return fft_complex_radix2(obj, fftw3_complex_radix2_inverse,
			       RB_fftw3_FFT_COPY);
}

static VALUE fftw3_complex_radix2_dif_forward(VALUE obj)
{
  return fft_complex_radix2(obj, 
			       fftw3_complex_radix2_dif_forward,
			       RB_fftw3_FFT_COPY);
}

static VALUE fftw3_complex_radix2_backward2(VALUE obj)
{
  return fft_complex_radix2(obj, fftw3_complex_radix2_backward,
			       RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_complex_radix2_inverse2(VALUE obj)
{
  return fft_complex_radix2(obj, fftw3_complex_radix2_inverse,
			       RB_fftw3_FFT_INPLACE);
}


static VALUE fftw3_complex_radix2_dif_forward2(VALUE obj)
{
  return fft_complex_radix2(obj, fftw3_complex_radix2_dif_forward,
RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_complex_radix2_dif_transform(VALUE obj, VALUE val_sign)
{
  size_t stride, n;
  fftw3_complex_packed_array data;
  fftw3_vector_complex *vin, *vout;
  fftw3_direction sign;
  sign = NUM2INT(val_sign);
  get_complex_stride_n(obj, &vin, &data, &stride, &n);
  vout = fftw3_vector_complex_alloc(n);
  fftw3_vector_complex_memcpy(vout, vin);
  fftw3_complex_radix2_dif_transform(vout->data, vout->stride, vout->size, sign);
  return Data_Wrap_Struct(cfftw3_vector_complex, 0, fftw3_vector_complex_free, vout);
}

/* in-place */
static VALUE fftw3_complex_radix2_dif_transform2(VALUE obj, VALUE val_sign)
{
  size_t stride, n;
  fftw3_complex_packed_array data;
  fftw3_direction sign;
  VALUE ary;
  sign = NUM2INT(val_sign);
  ary = get_complex_stride_n(obj, NULL, &data, &stride, &n);
  fftw3_complex_radix2_dif_transform(data, stride, n, sign);
  return ary;
}

static VALUE fftw3_complex_radix2_dif_backward(VALUE obj)
{
  return fft_complex_radix2(obj, 
			       fftw3_complex_radix2_dif_backward,
			       RB_fftw3_FFT_COPY);
}
static VALUE fftw3_complex_radix2_dif_inverse(VALUE obj)
{
  return fft_complex_radix2(obj, 
			       fftw3_complex_radix2_dif_inverse,
			       RB_fftw3_FFT_COPY);
}

static VALUE fftw3_complex_radix2_dif_backward2(VALUE obj)
{
  return fft_complex_radix2(obj, 
			       fftw3_complex_radix2_dif_backward,
			       RB_fftw3_FFT_INPLACE);
}
static VALUE fftw3_complex_radix2_dif_inverse2(VALUE obj)
{
  return fft_complex_radix2(obj, 
			       fftw3_complex_radix2_dif_inverse,
			       RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_FFT_Wavetable_n(VALUE obj)
{
  fftw3_FFT_Wavetable *table;
  Data_Get_Struct(obj, fftw3_FFT_Wavetable, table);
  return INT2FIX(table->n);
}

static VALUE fftw3_FFT_Wavetable_nf(VALUE obj)
{
  fftw3_FFT_Wavetable *table;
  Data_Get_Struct(obj, fftw3_FFT_Wavetable, table);
  return INT2FIX(table->nf);
}

static VALUE fftw3_FFT_Wavetable_factor(VALUE obj)
{
  fftw3_FFT_Wavetable *table;
  fftw3_vector_int *v;
  size_t i;
  Data_Get_Struct(obj, fftw3_FFT_Wavetable, table);
  v = fftw3_vector_int_alloc(table->nf);
  for (i = 0; i < table->nf; i++) fftw3_vector_int_set(v, i, table->factor[i]);
  return Data_Wrap_Struct(cfftw3_vector_int, 0, fftw3_vector_int_free, v);
}

enum {
  NONE_OF_TWO = 0,
  ALLOC_SPACE = 1,
  ALLOC_TABLE = 2,
  BOTH_OF_TWO = 3,
} FFTComplexStructAllocFlag;

static void fftw3_free(int flag, fftw3_FFT_Wavetable *table,
			 fftw3_FFT_Workspace *space);

// Parse argc, argv.  obj must be fftw3::Vector::Complex.
// This can be simplified at some point.
// See comments preceding get_complex_stride_n()
static int fftw3_get_argv_complex(int argc, VALUE *argv, VALUE obj,
			    fftw3_vector_complex ** vin,
			    fftw3_complex_packed_array *data, size_t *stride,
			    size_t *n, fftw3_complex_wavetable **table,
			    fftw3_complex_workspace **space)
{
  int flag = NONE_OF_TWO, flagtmp, i, itmp = argc, itmp2 = 0, ccc;
  int flagw = 0;

  CHECK_VECTOR_COMPLEX(obj);

  ccc = argc;
  flagtmp = 0;
  flagw = 0;
  for (i = argc-1; i >= itmp2; i--) {
    if (obj_is_kind_of(argv[i], cfftw3_complex_workspace)) {
      Data_Get_Struct(argv[i], fftw3_complex_workspace, *space);
      flagtmp = 1;
      flagw = 1;
      itmp = i;
      ccc--;
      break;
    }
  }
  flagtmp = 0;
  for (i = itmp-1; i >= itmp2; i--) {
    if (obj_is_kind_of(argv[i], cfftw3_complex_wavetable)) {
      Data_Get_Struct(argv[i], fftw3_complex_wavetable, *table);
      flagtmp = 1;
      ccc--;
      break;
    }
  }
  get_complex_stride_n(obj, vin, data, stride, n);
  if (flagw == 0) {
    *space = fftw3_complex_workspace_alloc(*n);
    flag += ALLOC_SPACE;
  }
  if (flagtmp == 0) {
    *table = fftw3_complex_wavetable_alloc(*n);
    flag += ALLOC_TABLE;
  }
  if (*table == NULL) {
    raise(eRuntimeError, "something wrong with wavetable");
  }
  if (*space == NULL) {
    raise(eRuntimeError, "something wrong with workspace");
  }
  return flag;
}

// Parse argc, argv.  obj must be fftw3::Vector of real data
static int fftw3_get_argv_real(int argc, VALUE *argv, VALUE obj,
			     double **ptr, size_t *stride,
			     size_t *n, fftw3_real_wavetable **table,
			     fftw3_real_workspace **space, int *naflag)
{
  int flag = NONE_OF_TWO, flagtmp, i, itmp = argc, itmp2 = 0, ccc;
  int flagw = 0;
  *naflag = 0;

  *ptr = get_ptr_double3(obj, n, stride, naflag);

  ccc = argc;
  flagtmp = 0;
  flagw = 0;
  for (i = argc-1; i >= itmp2; i--) {
    if (obj_is_kind_of(argv[i], cfftw3_real_workspace)) {
      Data_Get_Struct(argv[i], fftw3_real_workspace, *space);
      flagtmp = 1;
      flagw = 1;
      itmp = i;
      ccc--;
      break;
    }
  }
  flagtmp = 0;
  for (i = itmp-1; i >= itmp2; i--) {
    if (obj_is_kind_of(argv[i], cfftw3_real_wavetable)) {
      Data_Get_Struct(argv[i], fftw3_real_wavetable, *table);
      flagtmp = 1;
      ccc--;
      break;
    }
  }
  if (flagw == 0) {
    *space = fftw3_real_workspace_alloc(*n);
    flag += ALLOC_SPACE;
  }
  if (flagtmp == 0) {
    *table = fftw3_real_wavetable_alloc(*n);
    flag += ALLOC_TABLE;
  }
  if (*table == NULL) {
    raise(eRuntimeError, "something wrong with wavetable");
  }
  if (*space == NULL) {
    raise(eRuntimeError, "something wrong with workspace");
  }
  return flag;
}

// Parse argc, argv.  obj must be fftw3::Vector of halfcomplex data
static int fftw3_get_argv_halfcomplex(int argc, VALUE *argv, VALUE obj,
			   double **ptr, size_t *stride,
			    size_t *n, fftw3_halfcomplex_wavetable **table,
			    fftw3_real_workspace **space, int *naflag)
{
  int flag = NONE_OF_TWO, flagtmp, i, itmp = argc, itmp2 = 0, ccc;
  int flagw = 0;

  *ptr = get_ptr_double3(obj, n, stride, naflag);

  ccc = argc;
  flagtmp = 0;
  flagw = 0;
  for (i = argc-1; i >= itmp2; i--) {
    if (obj_is_kind_of(argv[i], cfftw3_real_workspace)) {
      Data_Get_Struct(argv[i], fftw3_real_workspace, *space);
      flagtmp = 1;
      flagw = 1;
      itmp = i;
      ccc--;
      break;
    }
  }
  flagtmp = 0;
  for (i = itmp-1; i >= itmp2; i--) {
    if (obj_is_kind_of(argv[i], cfftw3_halfcomplex_wavetable)) {
      Data_Get_Struct(argv[i], fftw3_halfcomplex_wavetable, *table);
      flagtmp = 1;
      ccc--;
      break;
    }
  }
  if (flagw == 0) {
    *space = fftw3_real_workspace_alloc(*n);
    flag += ALLOC_SPACE;
  }
  if (flagtmp == 0) {
    *table = fftw3_halfcomplex_wavetable_alloc(*n);
    flag += ALLOC_TABLE;
  }
  if (*table == NULL) {
    raise(eRuntimeError, "something wrong with wavetable");
  }
  if (*space == NULL) {
    raise(eRuntimeError, "something wrong with workspace");
  }
  return flag;
}

static void fftw3_free(int flag, fftw3_FFT_Wavetable *table,
			 fftw3_FFT_Workspace *space)
{
  switch (flag) {
  case ALLOC_TABLE:
    fftw3_FFT_Wavetable_free(table);
    break;
  case ALLOC_SPACE:
    fftw3_FFT_Workspace_free(space);
    break;
  case BOTH_OF_TWO:
    fftw3_FFT_Wavetable_free(table);
    fftw3_FFT_Workspace_free(space);
    break;
  default:
    /* never happens */
    break;
  }
}

static VALUE fft_complex_trans(int argc, VALUE *argv, VALUE obj,
				  int (*transform)(fftw3_complex_packed_array, 
						   size_t, size_t, 
						   const fftw3_complex_wavetable *,
						   fftw3_complex_workspace *),
				  int sss)
{
  int flag = 0;
  // local variable "status" was defined and set, but never used
  //int status;
  size_t stride, n;
  fftw3_complex_packed_array data;
  fftw3_vector_complex *vin, *vout;
  fftw3_complex_wavetable *table = NULL;
  fftw3_complex_workspace *space = NULL;
  flag = fftw3_get_argv_complex(argc, argv, obj, &vin, &data, &stride, &n, &table, &space);
  if (sss == RB_fftw3_FFT_COPY) {
    vout = fftw3_vector_complex_alloc(n);
    fftw3_vector_complex_memcpy(vout, vin);
    /*status =*/ (*transform)(vout->data, vout->stride /*1*/, vout->size /*n*/, table, space);
    fftw3_free(flag, (fftw3_FFT_Wavetable *) table, (fftw3_FFT_Workspace *) space);
    return Data_Wrap_Struct(cfftw3_vector_complex, 0, fftw3_vector_complex_free, vout);
  } else {    /* in-place */
    /*status =*/ (*transform)(data, stride, n, table, space);
    fftw3_free(flag, (fftw3_FFT_Wavetable *) table, (fftw3_FFT_Workspace *) space);
    return obj;
  }
}

static VALUE fftw3_complex_forward(int argc, VALUE *argv, VALUE obj)
{
  return fft_complex_trans(argc, argv, obj, fftw3_complex_forward,
			      RB_fftw3_FFT_COPY);
}

static VALUE fftw3_complex_forward2(int argc, VALUE *argv, VALUE obj)
{
  return fft_complex_trans(argc, argv, obj, fftw3_complex_forward,
			      RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_complex_transform(int argc, VALUE *argv, VALUE obj)
{
  int flag = 0;
  // local variable "status" was defined and set, but never used
  //int status;
  size_t stride, n;
  fftw3_vector_complex *vin, *vout;
  fftw3_direction sign;
  fftw3_complex_packed_array data;
  fftw3_complex_wavetable *table = NULL;
  fftw3_complex_workspace *space = NULL;
  CHECK_FIXNUM(argv[argc-1]);
  sign = FIX2INT(argv[argc-1]);
  flag = fftw3_get_argv_complex(argc-1, argv, obj, &vin, &data, &stride, &n, &table, &space);
  vout = fftw3_vector_complex_alloc(n);
  fftw3_vector_complex_memcpy(vout, vin);
  /*status =*/ fftw3_complex_transform(vout->data, stride, n, table, space, sign);
  fftw3_free(flag, (fftw3_FFT_Wavetable *) table, (fftw3_FFT_Workspace *) space);
  return Data_Wrap_Struct(cfftw3_vector_complex, 0, fftw3_vector_complex_free, vout);
}

/* in-place */
static VALUE fftw3_complex_transform2(int argc, VALUE *argv, VALUE obj)
{
  int flag = 0;
  // local variable "status" was defined and set, but never used
  //int status;
  size_t stride, n;
  fftw3_direction sign;
  fftw3_complex_packed_array data;
  fftw3_complex_wavetable *table = NULL;
  fftw3_complex_workspace *space = NULL;
  CHECK_FIXNUM(argv[argc-1]);
  sign = FIX2INT(argv[argc-1]);
  flag = fftw3_get_argv_complex(argc-1, argv, obj, NULL, &data, &stride, &n, &table, &space);
  /*status =*/ fftw3_complex_transform(data, stride, n, table, space, sign);
  fftw3_free(flag, (fftw3_FFT_Wavetable *) table, (fftw3_FFT_Workspace *) space);
  return obj;
}

static VALUE fftw3_complex_backward(int argc, VALUE *argv, VALUE obj)
{
  return fft_complex_trans(argc, argv, obj, fftw3_complex_backward,
			      RB_fftw3_FFT_COPY);
}

static VALUE fftw3_complex_backward2(int argc, VALUE *argv, VALUE obj)
{
  return fft_complex_trans(argc, argv, obj, fftw3_complex_backward,
			      RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_complex_inverse(int argc, VALUE *argv, VALUE obj)
{
  return fft_complex_trans(argc, argv, obj, fftw3_complex_inverse,
			      RB_fftw3_FFT_COPY);
}

static VALUE fftw3_complex_inverse2(int argc, VALUE *argv, VALUE obj)
{
  return fft_complex_trans(argc, argv, obj, fftw3_complex_inverse,
			      RB_fftw3_FFT_INPLACE);
}

// The FFT methods used to allow passing stride and n values as optional
// parameters to control which elements get transformed.  This created problems
// for Views which can have their own stride, so support for stride and n
// parameters to the transform methods is being dropped.  This method used to
// be called to determine the stride and n values to use based on the
// parameters and/or the vector itself (depending on how many parameters were
// passed). Now this function is somewhat unneceesary, but to simplify the code
// refactoring, it has been left in place for the time being.  Eventually it
// can be refactored away completely.
//
// obj must be fftw3::Vector of real or halfcomplex data
static VALUE get_ptr_stride_n(VALUE obj,
			     double **ptr, size_t *stride, size_t *n, int *flag)
{
  *flag = 0;
  *ptr = get_ptr_double3(obj, n, stride, flag);
  return obj;
}

static VALUE fft_radix2(VALUE obj,
			   int (*trans)(double [], size_t, size_t),
			   int sss)
{
  size_t stride, n;
  fftw3_vector *vnew;
  fftw3_vector_view vv;
  double *ptr1, *ptr2;
  int flag;
#ifdef HAVE_NARRAY_H
  int shape[1];
#endif
  VALUE ary;
  get_ptr_stride_n(obj, &ptr1, &stride, &n, &flag);
  if (flag == 0) {
    if (sss == RB_fftw3_FFT_COPY) {
      vnew = fftw3_vector_alloc(n);
      vv.vector.data = ptr1;
      vv.vector.stride = stride;
      vv.vector.size = n;
      fftw3_vector_memcpy(vnew, &vv.vector);
      ptr2 = vnew->data;
      stride = 1;
      ary = Data_Wrap_Struct(cfftw3_vector, 0, fftw3_vector_free, vnew);
    } else {
      ary = obj;
      ptr2 = ptr1;
    }
#ifdef HAVE_NARRAY_H
  } else if (flag == 1) {
    if (sss == RB_fftw3_FFT_COPY) {
      shape[0] = n;
      ary = na_make_object(NA_DFLOAT, 1, shape, cNArray);
      ptr2 = NA_PTR_TYPE(ary, double*);
      memcpy(ptr2, ptr1, sizeof(double)*n);
      stride = 1;
    } else {
      ary = obj;
      ptr2 = NA_PTR_TYPE(ary, double*);
    }
#endif
  } else {
    raise(eRuntimeError, "something wrong");
  }
  (*trans)(ptr2, stride, n);
  return ary;
}

static VALUE fftw3_real_radix2_transform(VALUE obj)
{
  return fft_radix2(obj, fftw3_real_radix2_transform,
		       RB_fftw3_FFT_COPY);
}

static VALUE fftw3_real_radix2_transform2(VALUE obj)
{
  return fft_radix2(obj, fftw3_real_radix2_transform,
		       RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_halfcomplex_radix2_inverse(VALUE obj)
{
  return fft_radix2(obj, fftw3_halfcomplex_radix2_inverse,
		       RB_fftw3_FFT_COPY);
}

static VALUE fftw3_halfcomplex_radix2_inverse2(VALUE obj)
{
  return fft_radix2(obj, fftw3_halfcomplex_radix2_inverse,
		       RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_halfcomplex_radix2_backward(VALUE obj)
{
  return fft_radix2(obj, fftw3_halfcomplex_radix2_backward,
		       RB_fftw3_FFT_COPY);
}

static VALUE fftw3_halfcomplex_radix2_backward2(VALUE obj)
{
  return fft_radix2(obj, fftw3_halfcomplex_radix2_backward,
		       RB_fftw3_FFT_INPLACE);
}

/*****/

static VALUE fft_real_trans(int argc, VALUE *argv, VALUE obj,
			       int (*trans)(double [], size_t, size_t, 
					    const fftw3_real_wavetable *, 
					    fftw3_real_workspace *),
			       int sss)
{
  int flag = 0, naflag = 0;
  // local variable "status" was defined and set, but never used
  //int status;
  size_t stride, n;
  fftw3_vector *vnew;
  fftw3_vector_view vv;
  double *ptr1, *ptr2;
#ifdef HAVE_NARRAY_H
  int shape[1];
#endif
  fftw3_real_wavetable *table = NULL;
  fftw3_real_workspace *space = NULL;
  VALUE ary;
  flag = fftw3_get_argv_real(argc, argv, obj, &ptr1, &stride, &n, &table, &space, &naflag);
  if (naflag == 0) {
    if (sss == RB_fftw3_FFT_COPY) {
      vnew = fftw3_vector_alloc(n);
      vv.vector.data = ptr1;
      vv.vector.stride = stride;
      vv.vector.size = n;
      fftw3_vector_memcpy(vnew, &vv.vector);
      ptr2 = vnew->data;
      stride = 1;
      ary = Data_Wrap_Struct(cfftw3_vector, 0, fftw3_vector_free, vnew);
    } else {
      ptr2 = ptr1;
      ary = obj;
    }
#ifdef HAVE_NARRAY_H
  } else if (naflag == 1) {
    if (sss == RB_fftw3_FFT_COPY) {
      shape[0] = n;
      ary = na_make_object(NA_DFLOAT, 1, shape, cNArray);
      ptr2 = NA_PTR_TYPE(ary, double*);
      memcpy(ptr2, ptr1, sizeof(double)*n);
      stride = 1;
    } else {
      ptr2 = ptr1;
      ary = obj;
    }
#endif
  } else {
    raise(eRuntimeError, "something wrong");
  }
  /*status =*/ (*trans)(ptr2, stride, n, table, space);
  fftw3_free(flag, (fftw3_FFT_Wavetable *) table, (fftw3_FFT_Workspace *) space);
  return ary;
}

static VALUE fftw3_real_transform(int argc, VALUE *argv, VALUE obj)
{
  return fft_real_trans(argc, argv, obj, fftw3_real_transform,
			   RB_fftw3_FFT_COPY);
}

static VALUE fftw3_real_transform2(int argc, VALUE *argv, VALUE obj)
{
  return fft_real_trans(argc, argv, obj, fftw3_real_transform,
			   RB_fftw3_FFT_INPLACE);
}

static VALUE fft_halfcomplex_trans(int argc, VALUE *argv, VALUE obj,
				      int (*trans)(double [], size_t, size_t, 
						   const fftw3_halfcomplex_wavetable *, fftw3_real_workspace *),
				      int sss)
{
  int flag = 0, naflag = 0;
  // local variable "status" was defined and set, but never used
  //int status;
  size_t stride, n;
  fftw3_vector *vnew;
  fftw3_vector_view vv;
  double *ptr1, *ptr2;
#ifdef HAVE_NARRAY_H
  int shape[1];
#endif
  fftw3_halfcomplex_wavetable *table = NULL;
  fftw3_real_workspace *space = NULL;
  VALUE ary;
  flag = fftw3_get_argv_halfcomplex(argc, argv, obj, &ptr1, &stride, &n, 
			   &table, &space, &naflag);
  if (naflag == 0) {
    if (sss == RB_fftw3_FFT_COPY) {
      vnew = fftw3_vector_alloc(n);
      vv.vector.data = ptr1;
      vv.vector.stride = stride;
      vv.vector.size = n;
      fftw3_vector_memcpy(vnew, &vv.vector);
      ptr2 = vnew->data;
      stride = 1;
      ary = Data_Wrap_Struct(cfftw3_vector, 0, fftw3_vector_free, vnew);
    } else {
      ptr2 = ptr1;
      ary = obj;
    }
#ifdef HAVE_NARRAY_H
  } else if (naflag == 1) {
    if (sss == RB_fftw3_FFT_COPY) {
      shape[0] = n;
      ary = na_make_object(NA_DFLOAT, 1, shape, cNArray);
      ptr2 = NA_PTR_TYPE(ary, double*);
      memcpy(ptr2, ptr1, sizeof(double)*n);
      stride = 1;
    } else {
      ptr2 = ptr1;
      ary = obj;
    }
#endif
  } else {
    raise(eRuntimeError, "something wrong");
  }
  /*status =*/ (*trans)(ptr2, stride, n, table, space);
  fftw3_free(flag, (fftw3_FFT_Wavetable *) table, (fftw3_FFT_Workspace *) space);
  return ary;
}

static VALUE fftw3_halfcomplex_transform(int argc, VALUE *argv, VALUE obj)
{
  return fft_halfcomplex_trans(argc, argv, obj, 
				  fftw3_halfcomplex_transform,
				  RB_fftw3_FFT_COPY);
}

static VALUE fftw3_halfcomplex_transform2(int argc, VALUE *argv, VALUE obj)
{
  return fft_halfcomplex_trans(argc, argv, obj, 
				  fftw3_halfcomplex_transform,
				  RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_halfcomplex_backward(int argc, VALUE *argv, VALUE obj)
{
  return fft_halfcomplex_trans(argc, argv, obj, 
				  fftw3_halfcomplex_backward,
				  RB_fftw3_FFT_COPY);
}

static VALUE fftw3_halfcomplex_backward2(int argc, VALUE *argv, VALUE obj)
{
  return fft_halfcomplex_trans(argc, argv, obj, 
				  fftw3_halfcomplex_backward,
				  RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_halfcomplex_inverse(int argc, VALUE *argv, VALUE obj)
{
  return fft_halfcomplex_trans(argc, argv, obj, 
				  fftw3_halfcomplex_inverse,
				  RB_fftw3_FFT_COPY);
}

static VALUE fftw3_halfcomplex_inverse2(int argc, VALUE *argv, VALUE obj)
{
  return fft_halfcomplex_trans(argc, argv, obj, 
				  fftw3_halfcomplex_inverse,
				  RB_fftw3_FFT_INPLACE);
}

static VALUE fftw3_real_unpack(VALUE obj)
{
  fftw3_vector *v;
  fftw3_vector_complex *vout;

  CHECK_VECTOR(obj);
  Data_Get_Struct(obj, fftw3_vector, v);

  vout = fftw3_vector_complex_alloc(v->size);
  fftw3_real_unpack(v->data, (fftw3_complex_packed_array) vout->data, v->stride, v->size);
  return Data_Wrap_Struct(cfftw3_vector_complex, 0, fftw3_vector_complex_free, vout);
}

static VALUE fftw3_halfcomplex_unpack(VALUE obj)
{
  fftw3_vector *v;
  fftw3_vector_complex *vout;

  CHECK_VECTOR(obj);
  Data_Get_Struct(obj, fftw3_vector, v);

  vout = fftw3_vector_complex_alloc(v->size);
  fftw3_halfcomplex_unpack(v->data, (fftw3_complex_packed_array) vout->data, v->stride, v->size);
  return Data_Wrap_Struct(cfftw3_vector_complex, 0, fftw3_vector_complex_free, vout);
}

/* Convert a halfcomplex data to Numerical Recipes style */
static VALUE fftw3_halfcomplex_to_nrc(VALUE obj)
{
  fftw3_vector *v, *vnew;
  size_t i, k;

  CHECK_VECTOR(obj);
  Data_Get_Struct(obj, fftw3_vector, v);

  vnew = fftw3_vector_alloc(v->size);
  fftw3_vector_set(vnew, 0, fftw3_vector_get(v, 0));  /* DC */
  fftw3_vector_set(vnew, 1, fftw3_vector_get(v, v->size/2));  /* Nyquist freq */
  for (i = 2, k = 1; i < vnew->size; i+=2, k++) {
    fftw3_vector_set(vnew, i, fftw3_vector_get(v, k));
    fftw3_vector_set(vnew, i+1, -fftw3_vector_get(v, v->size-k));
  }
  return Data_Wrap_Struct(cfftw3_vector, 0, fftw3_vector_free, vnew);
}

static VALUE fftw3_halfcomplex_amp_phase(VALUE obj)
{
  fftw3_vector *v;
  fftw3_vector *amp, *phase;
  double re, im;
  VALUE vamp, vphase;
  size_t i;
  CHECK_VECTOR(obj);
  Data_Get_Struct(obj, fftw3_vector, v);
  amp = fftw3_vector_alloc(v->size/2);
  phase = fftw3_vector_alloc(v->size/2);
  fftw3_vector_set(amp, 0, fftw3_vector_get(v, 0));
  fftw3_vector_set(phase, 0, 0);  
  fftw3_vector_set(amp, amp->size-1, fftw3_vector_get(v, v->size-1));
  fftw3_vector_set(phase, phase->size-1, 0);    
  for (i = 1; i < v->size-1; i+=2) {
    re = fftw3_vector_get(v, i);
    im = fftw3_vector_get(v, i+1);    
    fftw3_vector_set(amp, i/2+1, sqrt(re*re + im*im));
    fftw3_vector_set(phase, i/2+1, atan2(im, re));
  }
  vamp = Data_Wrap_Struct(VECTOR_ROW_COL(obj), 0, fftw3_vector_free, amp);
  vphase = Data_Wrap_Struct(VECTOR_ROW_COL(obj), 0, fftw3_vector_free, phase);
  return ary_new3(2, vamp, vphase);
}

void Init_fftw3_fft(VALUE module)
{
  mfftw3_fft = define_module_under(module, "FFT");

  /*****/

  define_const(mfftw3_fft, "Forward", INT2FIX(forward));
  define_const(mfftw3_fft, "FORWARD", INT2FIX(forward));
  define_const(mfftw3_fft, "Backward", INT2FIX(backward));
  define_const(mfftw3_fft, "BACKWARD", INT2FIX(backward));

  /* Transforms for complex vectors */
  define_method(cfftw3_vector_complex, "radix2_forward", 
		   fftw3_complex_radix2_forward, 0);
  define_method(cfftw3_vector_complex, "radix2_transform", 
		   fftw3_complex_radix2_transform, 1);
  define_method(cfftw3_vector_complex, "radix2_backward", 
		   fftw3_complex_radix2_backward, 0);
  define_method(cfftw3_vector_complex, "radix2_inverse", 
		   fftw3_complex_radix2_inverse, 0);
  define_method(cfftw3_vector_complex, "radix2_dif_forward", 
		   fftw3_complex_radix2_dif_forward, 0);
  define_method(cfftw3_vector_complex, "radix2_dif_transform", 
		   fftw3_complex_radix2_dif_transform, 1);
  define_method(cfftw3_vector_complex, "radix2_dif_backward", 
		   fftw3_complex_radix2_dif_backward, 0);
  define_method(cfftw3_vector_complex, "radix2_dif_inverse", 
		   fftw3_complex_radix2_dif_inverse, 0);

  /* In-place radix-2 transforms for complex vectors */
  define_method(cfftw3_vector_complex, "radix2_forward!", 
		   fftw3_complex_radix2_forward2, 0);
  define_method(cfftw3_vector_complex, "radix2_transform!",
		   fftw3_complex_radix2_transform2, 1);
  define_method(cfftw3_vector_complex, "radix2_backward!", 
		   fftw3_complex_radix2_backward2, 0);
  define_method(cfftw3_vector_complex, "radix2_inverse!", 
		   fftw3_complex_radix2_inverse2, 0);
  define_method(cfftw3_vector_complex, "radix2_dif_forward!", 
		   fftw3_complex_radix2_dif_forward2, 0);
  define_method(cfftw3_vector_complex, "radix2_dif_transform!",
		   fftw3_complex_radix2_dif_transform2, 1);
  define_method(cfftw3_vector_complex, "radix2_dif_backward!", 
		   fftw3_complex_radix2_dif_backward2, 0);
  define_method(cfftw3_vector_complex, "radix2_dif_inverse!", 
		   fftw3_complex_radix2_dif_inverse2, 0);

  // class fftw3::FFT::Wavetable < fftw3::Object
  //
  // Useful since some functionality is shared among
  // fftw3::FFT::Complex::Wavetable
  // fftw3::FFT::Real::Wavetable
  // fftw3::FFT::HalfComplex::Wavetable
  cfftw3_wavetable = define_class_under(mfftw3_fft, "Wavetable", cfftw3_Object);
  // No alloc
  // TODO Make fftw3::FFT::Wavetable#initialize private?
  define_method(cfftw3_wavetable, "n",
			     fftw3_FFT_Wavetable_n, 0);
  define_method(cfftw3_wavetable, "nf",
			     fftw3_FFT_Wavetable_nf, 0);
  define_method(cfftw3_wavetable, "factor",
		   fftw3_FFT_Wavetable_factor, 0);

  // class fftw3::FFT::ComplexWavetable < fftw3::FFT::Wavetable
  cfftw3_complex_wavetable = define_class_under(mfftw3_fft, "ComplexWavetable",
						     cfftw3_wavetable);
  define_singleton_method(cfftw3_complex_wavetable, "alloc",
			     fftw3_complex_wavetable_new, 1);

  // class fftw3::FFT::ComplexWorkspace < fftw3::Object
  cfftw3_complex_workspace = define_class_under(mfftw3_fft, "ComplexWorkspace", 
						     cfftw3_Object);
  define_singleton_method(cfftw3_complex_workspace, "alloc",
			     fftw3_complex_workspace_new, 1);

  define_method(cfftw3_vector_complex, "forward", fftw3_complex_forward, -1);
  define_method(cfftw3_vector_complex, "transform", fftw3_complex_transform, -1);
  define_method(cfftw3_vector_complex, "backward", fftw3_complex_backward, -1);
  define_method(cfftw3_vector_complex, "inverse", fftw3_complex_inverse, -1);

  define_method(cfftw3_vector_complex, "forward!", fftw3_complex_forward2, -1);
  define_method(cfftw3_vector_complex, "transform!", fftw3_complex_transform2, -1);
  define_method(cfftw3_vector_complex, "backward!", fftw3_complex_backward2, -1);
  define_method(cfftw3_vector_complex, "inverse!", fftw3_complex_inverse2, -1);

  /*****/

  // TODO Do these method names need the "real_" and "halfcomplex_" prefixes?
  define_method(cfftw3_vector, "real_radix2_transform", 
		   fftw3_real_radix2_transform, 0);
  define_alias(cfftw3_vector, "radix2_transform", "real_radix2_transform");
  define_alias(cfftw3_vector, "radix2_forward", "real_radix2_transform");
  define_method(cfftw3_vector, "real_radix2_inverse", 
		   fftw3_halfcomplex_radix2_inverse, 0);
  define_alias(cfftw3_vector, "radix2_inverse", "real_radix2_inverse");
  define_alias(cfftw3_vector, "halfcomplex_radix2_inverse", 
		  "real_radix2_inverse");
  define_method(cfftw3_vector, "real_radix2_backward", 
		   fftw3_halfcomplex_radix2_backward, 0);
  define_alias(cfftw3_vector, "radix2_backward", "real_radix2_backward");
  define_alias(cfftw3_vector, "halfcomplex_radix2_backward", 
		  "real_radix2_backward");

  // TODO Do these method names need the "real_" and "halfcomplex_" prefixes?
  define_method(cfftw3_vector, "real_radix2_transform!", 
		   fftw3_real_radix2_transform2, 0);
  define_alias(cfftw3_vector, "radix2_transform!", "real_radix2_transform!");
  define_alias(cfftw3_vector, "radix2_forward!", "real_radix2_transform!");
  define_method(cfftw3_vector, "real_radix2_inverse!", 
		   fftw3_halfcomplex_radix2_inverse2, 0);
  define_alias(cfftw3_vector, "radix2_inverse!", "real_radix2_inverse!");
  define_alias(cfftw3_vector, "halfcomplex_radix2_inverse!", 
		  "real_radix2_inverse!");
  define_method(cfftw3_vector, "real_radix2_backward!", 
		   fftw3_halfcomplex_radix2_backward2, 0);
  define_alias(cfftw3_vector, "radix2_backward!", "real_radix2_backward!");
  define_alias(cfftw3_vector, "halfcomplex_radix2_backward!", 
		  "real_radix2_backward!");

  /*****/

  // class fftw3::FFT::RealWavetable < fftw3::FFT::Wavetable
  cfftw3_real_wavetable = define_class_under(mfftw3_fft, "RealWavetable", 
						  cfftw3_wavetable);
  define_singleton_method(cfftw3_real_wavetable, "alloc",
			     fftw3_real_wavetable_new, 1);

  // class fftw3::FFT::HalfComplexWavetable < fftw3::FFT::Wavetable
  cfftw3_halfcomplex_wavetable = define_class_under(mfftw3_fft, 
							 "HalfComplexWavetable", cfftw3_wavetable);
  define_singleton_method(cfftw3_halfcomplex_wavetable, "alloc",
			     fftw3_halfcomplex_wavetable_new, 1);

  /*****/

  // class fftw3::FFT::RealWorkspace < fftw3::Object
  cfftw3_real_workspace = define_class_under(mfftw3_fft, "RealWorkspace", 
						  cfftw3_Object);
  define_singleton_method(cfftw3_real_workspace, "alloc",
			     fftw3_real_workspace_new, 1);

  /*****/

  // TODO Do these method names need the "real_" and "halfcomplex_" prefixes?
  define_method(cfftw3_vector, "real_transform", fftw3_real_transform, -1);
  define_alias(cfftw3_vector, "transform", "real_transform");
  define_alias(cfftw3_vector, "forward", "real_transform");
  define_alias(cfftw3_vector, "fft_forward", "real_transform");
  define_alias(cfftw3_vector, "fft", "real_transform");
  define_method(cfftw3_vector, "halfcomplex_transform", 
		   fftw3_halfcomplex_transform, -1);
  define_method(cfftw3_vector, "halfcomplex_backward", 
		   fftw3_halfcomplex_backward, -1);
  define_alias(cfftw3_vector, "backward", "halfcomplex_backward");
  define_alias(cfftw3_vector, "fft_backward", "halfcomplex_backward");
  define_method(cfftw3_vector, "halfcomplex_inverse", 
		   fftw3_halfcomplex_inverse, -1); 
  define_alias(cfftw3_vector, "fft_inverse", "halfcomplex_inverse");
  define_alias(cfftw3_vector, "ifft", "halfcomplex_inverse");
  define_alias(cfftw3_vector, "inverse", "halfcomplex_inverse");

  define_method(cfftw3_vector, "real_transform!", fftw3_real_transform2, -1);
  define_alias(cfftw3_vector, "transform!", "real_transform!");
  define_alias(cfftw3_vector, "forward!", "real_transform!");
  define_alias(cfftw3_vector, "fft_forward!", "real_transform!");
  define_alias(cfftw3_vector, "fft!", "real_transform!");
  define_method(cfftw3_vector, "halfcomplex_transform!", 
		   fftw3_halfcomplex_transform2, -1);
  define_method(cfftw3_vector, "halfcomplex_backward!",
		   fftw3_halfcomplex_backward2, -1);
  define_alias(cfftw3_vector, "backward!", "halfcomplex_backward!");
  define_alias(cfftw3_vector, "fft_backward!", "halfcomplex_backward!");
  define_method(cfftw3_vector, "halfcomplex_inverse!", 
		   fftw3_halfcomplex_inverse2, -1); 
  define_alias(cfftw3_vector, "fft_inverse!", "halfcomplex_inverse!");
  define_alias(cfftw3_vector, "ifft!", "halfcomplex_inverse!");
  define_alias(cfftw3_vector, "inverse!", "halfcomplex_inverse!");

  /***/
  define_method(cfftw3_vector, "fft_real_unpack", fftw3_real_unpack, 0);
  define_alias(cfftw3_vector, "real_unpack", "fft_real_unpack");
  define_alias(cfftw3_vector, "real_to_complex", "fft_real_unpack");
  define_alias(cfftw3_vector, "r_to_c", "fft_real_unpack");

  define_method(cfftw3_vector, "fft_halfcomplex_unpack", 
		   fftw3_halfcomplex_unpack, 0);
  define_alias(cfftw3_vector, "halfcomplex_unpack", "fft_halfcomplex_unpack");
  define_alias(cfftw3_vector, "halfcomplex_to_complex", "fft_halfcomplex_unpack");
  define_alias(cfftw3_vector, "hc_to_c", "fft_halfcomplex_unpack");

  /*****/

  define_method(cfftw3_vector, "to_nrc_order",
			     fftw3_halfcomplex_to_nrc, 0);
			     
  define_method(cfftw3_vector, "halfcomplex_amp_phase",
			     fftw3_halfcomplex_amp_phase, 0);
  define_alias(cfftw3_vector, "hc_amp_phase", "halfcomplex_amp_phase");
}
