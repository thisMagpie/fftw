#include "fftw3_config.h"
#ifdef HAVE_NMATRIX_H
#include "fftw3_array.h"
#include "fftw3_with_nmatrix.h"

// re-define so it doesn't use reinterpret_cast<>
#define NM_STRUCT(val)          ((NMATRIX*)(DATA_PTR(val)))
#define NM_STORAGE(val)         (NM_STRUCT(val)->storage)
#define NM_LIST_STORAGE(val)    ((LIST_STORAGE*)(NM_STORAGE(val)))
#define NM_YALE_STORAGE(val)    ((YALE_STORAGE*)(NM_STORAGE(val)))
#define NM_DENSE_STORAGE(val)   ((DENSE_STORAGE*)(NM_STORAGE(val)))

#define NM_DENSE_SRC(val)       (NM_DENSE_STORAGE(val)->src)
#define NM_RANK(val)            (NM_STORAGE(val)->rank)
#define NM_DTYPE(val)           (NM_STORAGE(val)->dtype)
#define NM_ITYPE(val)           (NM_YALE_STORAGE(val)->itype)
#define NM_STYPE(val)           (NM_STRUCT(val)->stype)
#define NM_SHAPE(val,i)         (NM_STORAGE(val)->shape[(i)])
#define NM_SHAPE0(val)          (NM_STORAGE(val)->shape[0])
#define NM_SHAPE1(val)          (NM_STORAGE(val)->shape[1])


// External API
extern VALUE rb_nmatrix_dense_create(nm_dtype_t dtype, size_t* shape, size_t rank, void* elements, size_t length);
extern VALUE rb_nvector_dense_create(nm_dtype_t dtype, void* elements, size_t length);

extern VALUE nm_eDataTypeError, nm_eStorageTypeError;

/* fftw3::Vector -> NMatrix */
static VALUE rb_fftw3_vector_to_nvector(VALUE obj, VALUE klass) {
  fftw3_vector *v = NULL;
  Data_Get_Struct(obj, fftw3_vector, v);

  return rb_nvector_dense_create(NM_FLOAT64, v->data, v->size);
}


static VALUE rb_fftw3_vector_complex_to_nvector(VALUE obj, VALUE klass) {
  fftw3_vector *v = NULL;
  Data_Get_Struct(obj, fftw3_vector, v);

  return rb_nvector_dense_create(NM_COMPLEX128, v->data, v->size);
}


static VALUE rb_fftw3_vector_int_to_nvector(VALUE obj, VALUE klass) {
  fftw3_vector *v = NULL;
  Data_Get_Struct(obj, fftw3_vector, v);

  return rb_nvector_dense_create(NM_INT64, v->data, v->size);
}


static VALUE rb_fftw3_matrix_to_nmatrix(VALUE obj, VALUE klass) {
  fftw3_matrix *m = NULL;
  Data_Get_Struct(obj, fftw3_matrix, m);

  return rb_nmatrix_dense_create(NM_FLOAT64, &(m->size1), 2, m->data, m->size1 * m->size2);
}


static VALUE rb_fftw3_matrix_complex_to_nmatrix(VALUE obj, VALUE klass) {
  fftw3_matrix *m = NULL;
  Data_Get_Struct(obj, fftw3_matrix, m);

  return rb_nmatrix_dense_create(NM_COMPLEX128, &(m->size1), 2, m->data, m->size1 * m->size2);
}


static VALUE rb_fftw3_matrix_int_to_nmatrix(VALUE obj, VALUE klass) {
  fftw3_matrix *m = NULL;
  Data_Get_Struct(obj, fftw3_matrix, m);

  return rb_nmatrix_dense_create(NM_INT64, &(m->size1), 2, m->data, m->size1 * m->size2);
}


fftw3_vector* nv_to_gv(VALUE nm) {
  DENSE_STORAGE* s = NM_DENSE_STORAGE(nm);
  fftw3_vector* v = fftw3_vector_alloc( s->count );

  if (s->dtype != NM_FLOAT64) {
    rb_raise(nm_eDataTypeError, "requires dtype of :float64 to convert to a fftw3 vector");
  }

  memcpy(v->data, s->elements, v->size*sizeof(double));

  return v;
}


fftw3_vector_complex* nv_to_gv_complex(VALUE nm) {
  DENSE_STORAGE* s = NM_DENSE_STORAGE(nm);
  fftw3_vector_complex* v = fftw3_vector_complex_alloc( s->count );

  if (s->dtype != NM_COMPLEX128) {
    rb_raise(nm_eDataTypeError, "requires dtype of :complex128 to convert to a fftw3 complex vector");
  }

  memcpy(v->data, s->elements, v->size*sizeof(double)*2);

  return v;
}


fftw3_vector_int* nv_to_gv_int(VALUE nm) {
  DENSE_STORAGE* s = NM_DENSE_STORAGE(nm);
  fftw3_vector_int* v = fftw3_vector_int_alloc( s->count );

  if (s->dtype != NM_INT64) {
    rb_raise(nm_eDataTypeError, "requires dtype of :int64 to convert to a fftw3 int vector");
  }

  memcpy(v->data, s->elements, v->size*sizeof(int));

  return v;
}


fftw3_matrix* nm_to_gm(VALUE nm) {
  DENSE_STORAGE* s = NM_DENSE_STORAGE(nm);
  fftw3_matrix* m = fftw3_matrix_alloc( s->shape[0], s->shape[1] );

  if (s->dtype != NM_FLOAT64) {
    rb_raise(nm_eDataTypeError, "requires dtype of :float64 to convert from a fftw3 double vector");
  }

  memcpy(m->data, s->elements, s->count);
  return m;
}

fftw3_matrix_complex* nm_to_gm_complex(VALUE nm) {
  DENSE_STORAGE* s = NM_DENSE_STORAGE(nm);
  fftw3_matrix_complex* m = fftw3_matrix_complex_alloc( s->shape[0], s->shape[1] );

  if (s->dtype != NM_COMPLEX128) {
    rb_raise(nm_eDataTypeError, "requires dtype of :complex128 to convert from a fftw3 complex vector");
  }

  memcpy(m->data, s->elements, s->count);
  return m;
}


fftw3_matrix_int* nm_to_gm_int(VALUE nm) {
  DENSE_STORAGE* s = NM_DENSE_STORAGE(nm);
  fftw3_matrix_int* m = fftw3_matrix_int_alloc( s->shape[0], s->shape[1] );

  if (s->dtype != NM_INT64) {
    rb_raise(nm_eDataTypeError, "requires dtype of :int64 to convert from a fftw3 int vector");
  }

  memcpy(m->data, s->elements, s->count);
  return m;
}


VALUE rb_fftw3_nv_to_fftw3_vector(VALUE obj, VALUE n) {
  return Data_Wrap_Struct(cfftw3_vector, 0, fftw3_vector_free, nv_to_gv(n));
}

VALUE rb_fftw3_nv_to_fftw3_vector_complex(VALUE obj, VALUE n) {
  return Data_Wrap_Struct(cfftw3_vector_complex, 0, fftw3_vector_complex_free, nv_to_gv_complex(n));
}

VALUE rb_fftw3_nv_to_fftw3_vector_int(VALUE obj, VALUE n) {
  return Data_Wrap_Struct(cfftw3_vector_int, 0, fftw3_vector_int_free, nv_to_gv_int(n));
}

VALUE rb_fftw3_nm_to_fftw3_matrix(VALUE obj, VALUE n) {
  return Data_Wrap_Struct(cfftw3_matrix, 0, fftw3_matrix_free, nm_to_gm(n));
}

VALUE rb_fftw3_nm_to_fftw3_matrix_complex(VALUE obj, VALUE n) {
  return Data_Wrap_Struct(cfftw3_matrix_complex, 0, fftw3_matrix_complex_free, nm_to_gm_complex(n));
}

VALUE rb_fftw3_nm_to_fftw3_matrix_int(VALUE obj, VALUE n) {
  return Data_Wrap_Struct(cfftw3_matrix_int, 0, fftw3_matrix_int_free, nm_to_gm_int(n));
}

VALUE rb_fftw3_nv_to_fftw3_vector_method(VALUE nv) {
  VALUE v;

  if (NM_DTYPE(nv) == NM_COMPLEX64 || NM_DTYPE(nv) == NM_COMPLEX128) {
    return Data_Wrap_Struct(cfftw3_vector_complex, 0, fftw3_vector_complex_free, nv_to_gv_complex(nv));
  } else {
    return Data_Wrap_Struct(cfftw3_vector, 0, fftw3_vector_free, nv_to_gv(nv));
  }

  return v;
}



void Init_fftw3_nmatrix(VALUE module) {
  rb_define_method(cfftw3_vector, "to_nv", rb_fftw3_vector_to_nvector, 0);
  rb_define_alias(cfftw3_vector, "to_nm", "to_nv");

  rb_define_singleton_method(cfftw3_vector, "to_fftw3v",    rb_fftw3_nv_to_fftw3_vector, 1);
  rb_define_singleton_method(cfftw3_vector, "to_gv",      rb_fftw3_nv_to_fftw3_vector, 1);
  rb_define_singleton_method(cfftw3_vector, "nv_to_fftw3v", rb_fftw3_nv_to_fftw3_vector, 1);
  rb_define_singleton_method(cfftw3_vector, "nv_to_gv",   rb_fftw3_nv_to_fftw3_vector, 1);

//  rb_define_method(cfftw3_vector_complex, "to_nv", rb_fftw3_vector_to_nvector_complex, 0);
//  rb_define_alias(cfftw3_vector_complex, "to_nm", "to_nv");

  rb_define_singleton_method(cfftw3_vector_complex, "to_fftw3v",    rb_fftw3_nv_to_fftw3_vector_complex, 1);
  rb_define_singleton_method(cfftw3_vector_complex, "to_gv",      rb_fftw3_nv_to_fftw3_vector_complex, 1);
  rb_define_singleton_method(cfftw3_vector_complex, "nv_to_fftw3v", rb_fftw3_nv_to_fftw3_vector_complex, 1);
  rb_define_singleton_method(cfftw3_vector_complex, "nv_to_gv",   rb_fftw3_nv_to_fftw3_vector_complex, 1);

//  rb_define_method(cfftw3_vector_int, "to_nv", rb_fftw3_vector_to_nvector_int, 0);
//  rb_define_alias(cfftw3_vector_int, "to_nm", "to_nv");

  rb_define_singleton_method(cfftw3_vector_int, "to_fftw3v",    rb_fftw3_nv_to_fftw3_vector_int, 1);
  rb_define_singleton_method(cfftw3_vector_int, "to_gv",      rb_fftw3_nv_to_fftw3_vector_int, 1);
  rb_define_singleton_method(cfftw3_vector_int, "nv_to_fftw3v", rb_fftw3_nv_to_fftw3_vector_int, 1);
  rb_define_singleton_method(cfftw3_vector_int, "nv_to_gv",   rb_fftw3_nv_to_fftw3_vector_int, 1);

  rb_define_method(cfftw3_matrix, "to_nm", rb_fftw3_matrix_to_nmatrix, 0);
  rb_define_singleton_method(cfftw3_matrix, "nm_to_fftw3m",  rb_fftw3_nm_to_fftw3_matrix, 1);

  rb_define_method(cfftw3_matrix_complex, "to_nm", rb_fftw3_matrix_complex_to_nmatrix, 0);
  rb_define_singleton_method(cfftw3_matrix_complex, "nm_to_fftw3m",  rb_fftw3_nm_to_fftw3_matrix_complex, 1);

  rb_define_method(cfftw3_matrix_int, "to_nm", rb_fftw3_matrix_int_to_nmatrix, 0);
  rb_define_singleton_method(cfftw3_matrix_int, "nm_to_fftw3m",  rb_fftw3_nm_to_fftw3_matrix_int, 1);

  rb_define_method(cNMatrix, "to_fftw3v",    rb_fftw3_nv_to_fftw3_vector_method, 0);
  rb_define_alias(cNMatrix, "to_gv", "to_fftw3v");
}

//#endif // HAVE_NMATRIX_H
