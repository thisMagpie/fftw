

![[Travis CLI][(https://travis-ci.org)]](https://travis-ci.org/thisMagpie/fftw.png) ![[Gemnasium][(https://gemnasium.com/thisMagpie/fftw)]](https://gemnasium.com/gemnasium/gemnasium-gem.png)

## Install FFTW

    git clone http://github.com/thismagpie/fftw

### Install FFTW3 on a Mac or linux

    cd fftw/fftw3
    ./configure  --enable-float  --enable-threads
    make
    sudo  -s  # Type in your root password and hit the enter button)
    make  install

Now to do the double precision version, we need to clean up the tree.

    make  distclean
    ./configure  --enable-threads
    make
    make  install

Now to do the double precision version, we need to clean up the tree.

    make  distclean
    ./configure  --enable-threads
    make
    make  install

## Common names

     R       : real type, aka fftw_real
     E       : real type for local variables (possibly extra precision)
     C       : complex type
     sz      : size
     vecsz   : vector size
     is, os  : input/output stride
     ri, ii  : real/imag input (complex data)
     ro, io  : real/imag output (complex data)
     I, O    : real input/output (real data)
     A       : assert
     CK      : check
     S       : solver, defined internally to each solver file
     P       : plan, defined internally to each solver file
     k       : codelet
     X(...)  : used for mangling of external names (see below)
     K(...)  : floating-point constant, in E precision

##  Conventions in FFTW3

If a name is used often and must have the form `fftw_foo` to avoid namespace pollution, `#define FOO fftw_foo` and use the short name: Use `foo` instead.

foo is lowercase so that it does not look like a `DOS` program.

Exception: `typedef struct foo_s {...} foo;`  instead of:

    typedef struct foo {...} foo;

for `C++` compatibility.

![[Donate with Pledgie][(https://pledgie.com/campaigns/25892)](https://pledgie.com/campaigns/25892.png)