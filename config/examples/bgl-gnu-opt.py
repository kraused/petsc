#!/usr/bin/env python
#
# BGL has broken 'libc' dependencies. The option 'LIBS' is used to
# workarround this problem.
#
# LIBS="-lc -lnss_files -lnss_dns -lresolv"
#
# Another workarround is to modify mpicc/mpif77 scripts and make them
# link with the corresponding compilers, and these additional
# libraries. The following tarball has the modified compiler scripts
#
# ftp://ftp.mcs.anl.gov/pub/petsc/tmp/petsc-bgl-tools.tar.gz 
#
configure_options = [
  '-LIBS=-lc -lc -lnss_files -lnss_dns -lresolv',
  '--with-cc=mpicc',
  '--with-cxx=mpicxx',
  '--with-fc=mpif77',

  '--download-f-blas-lapack=1',
  '--with-shared=0',
  
  '-COPTFLAGS=-O3',
  '-FOPTFLAGS=-O3',
  '--with-debugging=0',
  '--with-fortran-kernels=generic',
  '--with-x=0',
  
  '--with-batch=1',
  '--with-mpi-shared=0',
  '--with-endian=big',
  '--with-memcmp-ok',
  '--sizeof_char=1',
  '--sizeof_void_p=4',
  '--sizeof_short=2',
  '--sizeof_int=4',
  '--sizeof_long=4',
  '--sizeof_size_t=4',
  '--sizeof_long_long=8',
  '--sizeof_float=4',
  '--sizeof_double=8',
  '--bits_per_byte=8',
  '--sizeof_MPI_Comm=4',
  '--sizeof_MPI_Fint=4',
  '--have-mpi-long-double=1',
  ]

if __name__ == '__main__':
  import sys,os
  sys.path.insert(0,os.path.abspath('config'))
  import configure
  configure.petsc_configure(configure_options)

# Extra options used for testing locally
test_options = []
    