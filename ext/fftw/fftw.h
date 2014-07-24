/* fftw/fftw.h.  Generated automatically by configure.  */
/* -*- C -*- */
/*
 * Copyright (c) 1997-1999 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "ruby.h"
#include <fftw3.h>
#include <stdlib.h>

/* fftw.h -- system-wide definitions */
#ifndef FFTW_H
    #define FFTW_H
 // Defining a space for information and references about the module to be stored internally

// Prototype for the initialization method - Ruby calls this, not you
          /* __cplusplus */

  /* Define for using single precision */
  /*
   * If you can, use configure --enable-float instead of changing this
   * flag directly 
   */
  #define FFTW_ENABLE_FLOAT 1

  /* our real numbers */
  #ifdef FFTW_ENABLE_FLOAT
    typedef float fftw_real;
  #else
    typedef double fftw_real;
  #endif

  /*********************************************
   * Complex numbers and operations 
   *********************************************/
  typedef struct {
    fftw_real re, im;
  }ftw_complex;

  #define c_re(c)  ((c).re)
  #define c_im(c)  ((c).im)

  /* backward compatibility with FFTW-1.3 */
  typedef fftw_complex FFTW_COMPLEX;
  typedef fftw_real FFTW_REAL;

  #ifndef FFTW_1_0_COMPATIBILITY
  #define FFTW_1_0_COMPATIBILITY 0
  #endif

  #if FFTW_1_0_COMPATIBILITY
  /* backward compatibility with FFTW-1.0 */
  #define REAL fftw_real
  #define COMPLEX fftw_complex
  #endif

  /*********************************************
   * Success or failure status
   *********************************************/

  typedef enum {
       FFTW_SUCCESS = 0, FFTW_FAILURE = -1
  } fftw_status;

  /*********************************************
   *              Codelets
   *********************************************/
  typedef void (fftw_notw_codelet) 
       (const fftw_complex *, fftw_complex *, int, int);
  typedef void (fftw_twiddle_codelet)
       (fftw_complex *, const fftw_complex *, int,
        int, int);
  typedef void (fftw_generic_codelet) 
       (fftw_complex *, const fftw_complex *, int,
        int, int, int);
  typedef void (fftw_real2hc_codelet)
       (const fftw_real *, fftw_real *, fftw_real *,
        int, int, int);
  typedef void (fftw_hc2real_codelet)
       (const fftw_real *, const fftw_real *,
        fftw_real *, int, int, int);
  typedef void (fftw_hc2hc_codelet)
       (fftw_real *, const fftw_complex *,
        int, int, int);
  typedef void (fftw_rgeneric_codelet)
       (fftw_real *, const fftw_complex *, int,
        int, int, int);

  /*********************************************
   *     Configurations
   *********************************************/
  /*
   * A configuration is a database of all known codelets
   */

  enum fftw_node_type {
       FFTW_NOTW, FFTW_TWIDDLE, FFTW_GENERIC, FFTW_RADER,
       FFTW_REAL2HC, FFTW_HC2REAL, FFTW_HC2HC, FFTW_RGENERIC
  };

#ifdef __cplusplus
extern "C" {
#endif

  #define FFTW_OUT_OF_PLACE (0)
  #define FFTW_IN_PLACE (8)
  #define FFTW_USE_WISDOM (16)

  #define FFTW_THREADSAFE (128)  /* guarantee plan is read-only so that the
            same plan can be used in parallel by
            multiple threads */

  #define FFTWND_FORCE_BUFFERED (256)     /* internal flag, forces buffering
                                             in fftwnd transforms */

  #define FFTW_NO_VECTOR_RECURSE (512)    /* internal flag, prevents use *?
  /*=========== RWCox: omit the malloc.c file ==========*/

  #if 0
     extern void fftw_die(const char *s);
     extern void *fftw_malloc(size_t n);
     extern void fftw_free(void *p);
     extern void fftw_check_memory_leaks(void);
     extern void fftw_print_max_memory_usage(void);
  #else
  # ifndef RWCOX
  #  define fftw_malloc malloc
  #  define fftw_free   free
  #  define fftw_die(s) (fflush(stdout),fprintf(stderr,"fftw: %s\n",s),exit(1))
  #  define fftw_check_memory_leaks     /* nada */
  #  define fftw_print_max_memory_usage /* nada */
  # endif
  #endif
      /* FFTW_H */

void Init_fftw();


#ifdef __cplusplus
}
#endif
#endif

