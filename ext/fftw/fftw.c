#include <fftw3.h>
#include "ruby.h"


#ifdef HAVE_NMATRIX_H
EXTERN VALUE cNMatrix;
#endif

VALUE cFFTW;
VALUE rb_fftw(int argc, VALUE* argv, VALUE self);

// re-define so it doesn't use reinterpret_cast<>
//#define NM_STRUCT(val)          ((NMATRIX*)(DATA_PTR(val)))
#define NM_STORAGE(val)         (NM_STRUCT(val)->storage)
#define NM_LIST_STORAGE(val)    ((LIST_STORAGE*)(NM_STORAGE(val)))
#define NM_YALE_STORAGE(val)    ((YALE_STORAGE*)(NM_STORAGE(val)))
#define NM_DENSE_STORAGE(val)   ((DENSE_STORAGE*)(NM_STORAGE(val)))

//#define NM_DENSE_SRC(val)       (NM_DENSE_STORAGE(val)->src)
#define NM_RANK(val)            (NM_STORAGE(val)->rank)
#define NM_DTYPE(val)           (NM_STORAGE(val)->dtype)
//#define NM_ITYPE(val)           (NM_YALE_STORAGE(val)->itype)
#define NM_STYPE(val)           (NM_STRUCT(val)->stype)
#define NM_SHAPE(val,i)         (NM_STORAGE(val)->shape[(i)])
#define NM_SHAPE0(val)          (NM_STORAGE(val)->shape[0])
#define NM_SHAPE1(val)          (NM_STORAGE(val)->shape[1])

void Init_fftw()
{
  cFFTW = rb_define_class("FFTW", rb_cObject);
  rb_define_method(cFFTW, "fftw", rb_fftw, -1);
}
