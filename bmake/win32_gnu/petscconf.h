#ifdef PETSC_RCS_HEADER
"$Id: petscconf.h,v 1.25 2001/06/20 21:20:06 buschelm Exp $"
"Defines the configuration for this machine"
#endif

#if !defined(INCLUDED_PETSCCONF_H)
#define INCLUDED_PETSCCONF_H

#define PARCH_win32_gnu
#define PETSC_ARCH_NAME "win32_gnu"

#define PETSC_HAVE_GETCWD
#define PETSC_HAVE_POPEN
#define PETSC_HAVE_LIMITS_H
#define PETSC_HAVE_SLOW_NRM2
#define PETSC_HAVE_SEARCH_H
#define PETSC_HAVE_PWD_H
#define PETSC_HAVE_STRING_H
#define PETSC_HAVE_GETDOMAINNAME 
#define PETSC_HAVE_UNISTD_H
#define PETSC_HAVE_SYS_TIME_H 
#define PETSC_HAVE_MALLOC_H
#define PETSC_HAVE_STDLIB_H
#define PETSC_HAVE_UNISTD_H
#define PETSC_HAVE_SYS_TIME_H
#define PETSC_NEEDS_GETTIMEOFDAY_PROTO
#define PETSC_HAVE_SLEEP
#define PETSC_HAVE_SYS_STAT_H

#define PETSC_HAVE_FORTRAN_UNDERSCORE 
#define PETSC_HAVE_FORTRAN_UNDERSCORE_UNDERSCORE

#define PETSC_HAVE_READLINK
#define PETSC_HAVE_MEMMOVE
#define PETSC_HAVE_RAND
#define PETSC_HAVE_DOUBLE_ALIGN_MALLOC

#define PETSC_CANNOT_START_DEBUGGER
#define PETSC_HAVE_SYS_RESOURCE_H

#define PETSC_HAVE_GET_USER_NAME
#define PETSC_SIZEOF_VOID_P 4
#define PETSC_SIZEOF_INT 4
#define PETSC_SIZEOF_DOUBLE 8
#define PETSC_BITS_PER_BYTE 8
#define PETSC_SIZEOF_FLOAT 4
#define PETSC_SIZEOF_LONG 4
#define PETSC_SIZEOF_LONG_LONG 8

#define PETSC_USE_NT_TIME

#define PETSC_MISSING_SIGSYS

#ifdef PETSC_USE_MAT_SINGLE
#  define PETSC_MEMALIGN 16
#  define PETSC_HAVE_SSE "gccsse.h"
#endif

#define PETSC_PRINTF_FORMAT_CHECK(a,b) __attribute__ ((format (printf, a,b)))
#define PETSC_HAVE_CXX_NAMESPACE
 
#endif
