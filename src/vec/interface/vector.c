#ifndef lint
static char vcid[] = "$Id: vector.c,v 1.30 1995/05/02 23:37:16 bsmith Exp bsmith $";
#endif

/* 
   This is where the abstract vector operations are defined
 */
#include "vecimpl.h"    /*I "vec.h" I*/

/*@
   VecValidVector - Returns 1 if this is a valid vector; otherwise returns 0.

   Input Parameter:
.  v - the object to check

.keywords: vector, valid
@*/
int VecValidVector(Vec v)
{
  if (!v) return 0;
  if (v->cookie != VEC_COOKIE) return 0;
  return 1;
}
/*@
   VecDot - Computes the vector dot product.

   Input Parameters:
.  x, y - the vectors

   Output Parameter:
.  val - the dot product

.keywords: vector, dot product, inner product

.seealso: VecMDot(), VecDot()
@*/
int VecDot(Vec x, Vec y, Scalar *val)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(y,VEC_COOKIE);
  CHKSAME(x,y);
  PLogEventBegin(VEC_Dot,x,y,0,0);
  ierr = (*x->ops->dot)(x,y,val); CHKERR(ierr);
  PLogEventEnd(VEC_Dot,x,y,0,0);
  return 0;
}

/*@
   VecNorm  - Computes the vector two norm.

   Input Parameters:
.  x - the vector

   Output Parameter:
.  val - the norm 

.keywords: vector, norm

.seealso: VecASum()
@*/
int VecNorm(Vec x,double *val)  
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE);
  PLogEventBegin(VEC_Norm,x,0,0,0);
  ierr = (*x->ops->norm)(x,val); CHKERR(ierr);
  PLogEventEnd(VEC_Norm,x,0,0,0);
  return 0;
}
/*@
   VecASum - Computes the vector one norm (sum of components' absolute values).

   Input Parameter:
.  x - the vector

   Output Parameter:
.  val - the sum 

.keywords: vector, sum, absolute value, norm

.seealso: VecNorm(), VecSum()
@*/
int VecASum(Vec x,double *val)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE);
  PLogEventBegin(VEC_ASum,x,0,0,0);
  ierr = (*x->ops->asum)(x,val); CHKERR(ierr);
  PLogEventEnd(VEC_ASum,x,0,0,0);
  return 0;
}

/*@
   VecAMax - Determines the vector infinity norm (component with maximum
   absolute value) and its location.

   Input Parameter:
.  x - the vector

   Output Parameters:
.  val - the component with maximum absolute value
.  p - the location of val

.keywords: vector, maximum, norm, absolute value

.seealso: VecMax(), VecNorm(), VecASum()
@*/
int VecAMax(Vec x,int *p,double *val)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE);
  PLogEventBegin(VEC_AMax,x,0,0,0);
  ierr = (*x->ops->amax)(x,p,val); CHKERR(ierr);
  PLogEventEnd(VEC_AMax,x,0,0,0);
  return 0;
}

/*@
   VecMax - Determines the maximum vector component and its location.

   Input Parameter:
.  x - the vector

   Output Parameters:
.  val - the maximum component
.  p - the location of val

.keywords: vector, maximum

.seealso: VecAMax(), VecMin()
@*/
int VecMax(Vec x,int *p,double *val)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE);
  PLogEventBegin(VEC_Max,x,0,0,0);
  ierr = (*x->ops->max)(x,p,val); CHKERR(ierr);
  PLogEventEnd(VEC_Max,x,0,0,0);
  return 0;
}

/*@
   VecMin - Determines the minimum vector component and its location.

   Input Parameters:
.  x - the vector

   Output Parameter:
.  val - the minimum component
.  p - the location of val

.keywords: vector, minimum

.seealso: VecMax()
@*/
int VecMin(Vec x,int *p,double *val)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE);
  PLogEventBegin(VEC_Min,x,0,0,0);
  ierr = (*x->ops->min)(x,p,val); CHKERR(ierr);
  PLogEventEnd(VEC_Min,x,0,0,0);
  return 0;
}

/*@
   VecTDot - Computes non-Hermitian vector dot product. That is, this
   routine does NOT use the complex conjugate.

   Input Parameters:
.  x, y - the vectors

   Output Parameter:
.  val - the dot product

.keywords: vector, dot product, inner product, non-Hermitian

.seealso: VecDot(), VecMTDot()
@*/
int VecTDot(Vec x,Vec y,Scalar *val) 
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(y,VEC_COOKIE);
  CHKSAME(x,y);
  PLogEventBegin(VEC_TDot,x,y,0,0);
  ierr = (*x->ops->tdot)(x,y,val); CHKERR(ierr);
  PLogEventEnd(VEC_TDot,x,y,0,0);
  return 0;
}

/*@
   VecScale - Scales a vector. 

   Input Parameters:
.  x - the vector
.  alpha - the scalar

.keywords: vector, scale
@*/
int VecScale(Scalar *alpha,Vec x)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE);
  PLogEventBegin(VEC_Scale,x,0,0,0);
  ierr = (*x->ops->scale)(alpha,x); CHKERR(ierr);
  PLogEventEnd(VEC_Scale,x,0,0,0);
  return 0;
}

/*@
   VecCopy - Copies a vector. 

   Input Parameter:
.  x  - the vector

   Output Parameter:
.  y  - the copy

.keywords: vector, copy
@*/
int VecCopy(Vec x,Vec y)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(y,VEC_COOKIE);
  PLogEventBegin(VEC_Copy,x,y,0,0);
  ierr = (*x->ops->copy)(x,y); CHKERR(ierr);
  PLogEventEnd(VEC_Copy,x,y,0,0);
  return 0;
}
 
/*@
   VecSet - Sets all components of a vector to a scalar. 

   Input Parameters:
.  alpha - the scalar
.  x  - the vector

   Output Parameter:
.  x  - the scaled vector

.keywords: vector, set
@*/
int VecSet(Scalar *alpha,Vec x) 
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE);
  PLogEventBegin(VEC_Set,x,0,0,0);
  ierr = (*x->ops->set)(alpha,x); CHKERR(ierr);
  PLogEventEnd(VEC_Set,x,0,0,0);
  return 0;
} 

/*@
   VecAXPY - Computes y <- alpha x + y. 

   Input Parameters:
.  alpha - the scalar
.  x, y  - the vectors

   Output Parameter:
.  y - output vector

.keywords: vector, saxpy

.seealso: VecAYPX(), VecMAXPY(), VecWAXPY()
@*/
int VecAXPY(Scalar *alpha,Vec x,Vec y)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); 
  VALIDHEADER(y,VEC_COOKIE);
  PLogEventBegin(VEC_AXPY,x,y,0,0);
  ierr = (*x->ops->axpy)(alpha,x,y); CHKERR(ierr);
  PLogEventEnd(VEC_AXPY,x,y,0,0);
  return 0;
} 
/*@
   VecAYPX - Computes y <- x + alpha y.

   Input Parameters:
.  alpha - the scalar
.  x, y  - the vectors

   Output Parameter:
.  y - output vector

.keywords: vector, saypx

.seealso: VecAXPY(), VecWAXPY()
@*/
int VecAYPX(Scalar *alpha,Vec x,Vec y)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(y,VEC_COOKIE);
  CHKSAME(x,y);
  PLogEventBegin(VEC_AYPX,x,y,0,0);
  ierr =  (*x->ops->aypx)(alpha,x,y); CHKERR(ierr);
  PLogEventEnd(VEC_AYPX,x,y,0,0);
  return 0;
} 
/*@
   VecSwap - Swaps the vectors x and y.

   Input Parameters:
.  x, y  - the vectors

.keywords: vector, swap
@*/
int VecSwap(Vec x,Vec y)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE);  VALIDHEADER(y,VEC_COOKIE);
  CHKSAME(x,y);
  PLogEventBegin(VEC_Swap,x,y,0,0);
  ierr = (*x->ops->swap)(x,y); CHKERR(ierr);
  PLogEventEnd(VEC_Swap,x,y,0,0);
  return 0;
}
/*@
   VecWAXPY - Computes w <- alpha x + y.

   Input Parameters:
.  alpha - the scalar
.  x, y  - the vectors

   Output Parameter:
.  w - the result

.keywords: vector, waxpy

.seealso: VecAXPY(), VecAYPX()
@*/
int VecWAXPY(Scalar *alpha,Vec x,Vec y,Vec w)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(y,VEC_COOKIE);
  VALIDHEADER(w,VEC_COOKIE);
  CHKSAME(x,y); CHKSAME(y,w);
  PLogEventBegin(VEC_WAXPY,x,y,w,0);
  ierr =  (*x->ops->waxpy)(alpha,x,y,w); CHKERR(ierr);
  PLogEventEnd(VEC_WAXPY,x,y,w,0);
  return 0;
}
/*@
   VecPMult - Computes the componentwise multiplication w = x*y.

   Input Parameters:
.  x, y  - the vectors

   Output Parameter:
.  w - the result

.keywords: vector, multiply, componentwise

.seealso: VecPDiv()
@*/
int VecPMult(Vec x,Vec y,Vec w)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(y,VEC_COOKIE);
  VALIDHEADER(w,VEC_COOKIE);
  PLogEventBegin(VEC_PMult,x,y,w,0);
  ierr = (*x->ops->pmult)(x,y,w); CHKERR(ierr);
  PLogEventEnd(VEC_PMult,x,y,w,0);
  return 0;
} 
/*@
   VecPDiv - Computes the componentwise division w = x/y.

   Input Parameters:
.  x, y  - the vectors

   Output Parameter:
.  w - the result

.keywords: vector, divide, componentwise

.seealso: VecPMult()
@*/
int VecPDiv(Vec x,Vec y,Vec w)
{
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(y,VEC_COOKIE);
  VALIDHEADER(w,VEC_COOKIE);
  CHKSAME(x,y); CHKSAME(y,w);
  return (*x->ops->pdiv)(x,y,w);
}
/*@
   VecCreate - Creates a vector from another vector. 

   Input Parameters:
.  v - a vector to mimic

   Output Parameter:
.  newv - location to put new vector

   Notes:
   Use VecDestroy() to free the space. Use VecGetVecs() to get several
   vectors. 

.keywords: vector, create

.seealso: VecDestroy(), VecGetVecs(), VecCreateInitialVector()
@*/
int VecCreate(Vec v,Vec *newv) 
{
  VALIDHEADER(v,VEC_COOKIE);
  return   (*v->ops->create)(v,newv);
}
/*@
   VecDestroy - Destroys  a vector created with VecCreate().

   Input Parameters:
.  v  - the vector

.keywords: vector, destroy

.seealso: VecCreate()
@*/
int VecDestroy(Vec v)
{
  VALIDHEADER(v,VEC_COOKIE);
  return (*v->destroy)((PetscObject )v);
}

/*@
   VecGetVecs - Creates several vectors from another vector. 

   Input Parameters:
.  m - the number of vectors to obtain
.  v - a vector to mimic

   Output Parameter:
.  V - location to put pointer to array of vectors

   Notes:
   Use VecFreeVecs() to free the space. Use VecCreate() to get a single
   vector.

.keywords: vector, get 

.seealso:  VecFreeVecs(), VecCreate(), VecCreateInitialVector()
@*/
int VecGetVecs(Vec v,int m,Vec **V)  
{
  VALIDHEADER(v,VEC_COOKIE);
  return (*v->ops->getvecs)( v, m,V );
}

/*@
   VecFreeVecs - Frees a block of vectors obtained with VecGetVecs().

   Input Parameters:
.  vv - pointer to array of vector pointers
.  m - the number of vectors previously obtained

.keywords: vector, free

.seealso: VecGetVecs()
@*/
int VecFreeVecs(Vec *vv,int m)
{
  if (!vv) SETERR(1,"Null vectors");
  VALIDHEADER(*vv,VEC_COOKIE);
  return (*(*vv)->ops->freevecs)( vv, m );
}

/*@
   VecSetValues - Inserts or adds values into certain locations of a vector. 
   These values may be cached, so VecAssemblyBegin() and VecAssemblyEnd() 
   MUST be called after all calls to VecSetValues() have been completed.

   Input Parameters:
.  x - vector to insert in
.  ni - number of elements to add
.  ix - indices where to add
.  y - array of values
.  iora - either INSERTVALUES or ADDVALUES

   Notes: 
   x[ix[i]] = y[i], for i=0,...,ni-1.

   Calls to VecSetValues() with the INSERTVALUES and ADDVALUES 
   options cannot be mixed without intervening calls to the assembly
   routines.

.keywords: vector, set, values

.seealso:  VecAssemblyBegin(), VecAssemblyEnd()
@*/
int VecSetValues(Vec x,int ni,int *ix,Scalar *y,InsertMode iora) 
{
  VALIDHEADER(x,VEC_COOKIE);
  return (*x->ops->setvalues)( x, ni,ix, y,iora );
}

/*@
   VecAssemblyBegin - Begins assembling the vector.  This routine should
   be called after completing all calls to VecSetValues().

   Input Parameter:
.  vec - the vector

.keywords: vector, begin, assembly, assemble

.seealso: VecAssemblyEnd(), VecSetValues()
@*/
int VecAssemblyBegin(Vec vec)
{
  int ierr;
  VALIDHEADER(vec,VEC_COOKIE);
  PLogEventBegin(VEC_BeginAssembly,vec,0,0,0);
  if (vec->ops->assemblybegin) {
    ierr = (*vec->ops->assemblybegin)(vec); CHKERR(ierr);
  }
  PLogEventEnd(VEC_BeginAssembly,vec,0,0,0);
  return 0;
}

/*@
   VecAssemblyEnd - Completes assembling the matrix.  This routine should
   be called after all calls to VecSetValues() and after VecAssemblyBegin().

   Input Parameter:
.  vec - the vector

.keywords: vector, end, assembly, assemble

.seealso: VecAssemblyBegin(), VecSetValues()
@*/
int VecAssemblyEnd(Vec vec)
{
  int ierr;
  VALIDHEADER(vec,VEC_COOKIE);
  PLogEventBegin(VEC_EndAssembly,vec,0,0,0);
  if (vec->ops->assemblyend) {
    ierr = (*vec->ops->assemblyend)(vec); CHKERR(ierr);
  }
  PLogEventEnd(VEC_EndAssembly,vec,0,0,0);
  return 0;
}

/*@
   VecMTDot - Computes non-Hermitian vector multiple dot product. 
   That is, it does NOT use the complex conjugate.

   Input Parameters:
.  nv - number of vectors
.  x - one vector
.  y - array of vectors.  Note that vectors are pointers

   Output Parameter:
.  val - array of the dot products

.keywords: vector, dot product, inner product, non-Hermitian, multiple

.seealso: VecMDot(), VecTDot()
@*/
int VecMTDot(int nv,Vec x,Vec *y,Scalar *val)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(*y,VEC_COOKIE);
  CHKSAME(x,*y);
  PLogEventBegin(VEC_MTDot,x,*y,0,0);
  ierr = (*x->ops->mtdot)(nv,x,y,val); CHKERR(ierr);
  PLogEventEnd(VEC_MTDot,x,*y,0,0);
  return 0;
}
/*@
   VecMDot - Computes vector multiple dot product. 

   Input Parameters:
.  nv - number of vectors
.  x - one vector
.  y - array of vectors. 

   Output Parameter:
.  val - array of the dot products

.keywords: vector, dot product, inner product, multiple

.seealso: VecMTDot(), VecDot()
@*/
int VecMDot(int nv,Vec x,Vec *y,Scalar *val)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(*y,VEC_COOKIE);
  CHKSAME(x,*y);
  PLogEventBegin(VEC_MDot,x,*y,0,0);
  ierr = (*x->ops->mdot)(nv,x,y,val); CHKERR(ierr);
  PLogEventEnd(VEC_MDot,x,*y,0,0);
  return 0;
}

/*@
   VecMAXPY - Computes y <- alpha[j] x[j] + y. 

   Input Parameters:
.  nv - number of scalars and x-vectors
.  alpha - array of scalars
.  x  - one vector
.  y  - array of vectors

   Output Parameter:
.  y  - array of vectors

.keywords: vector, saxpy, maxpy, multiple

.seealso: VecAXPY(), VecWAXPY(), VecAYPX()
@*/
int  VecMAXPY(int nv,Scalar *alpha,Vec x,Vec *y)
{
  int ierr;
  VALIDHEADER(x,VEC_COOKIE); VALIDHEADER(*y,VEC_COOKIE);
  CHKSAME(x,*y);
  PLogEventBegin(VEC_MAXPY,x,*y,0,0);
  ierr = (*x->ops->maxpy)(nv,alpha,x,y); CHKERR(ierr);
  PLogEventEnd(VEC_MAXPY,x,*y,0,0);
  return 0;
} 

/*@
   VecGetArray - Returns a pointer to vector data. For default seqential 
   vectors, VecGetArray() returns a pointer to the data array. Otherwise,
   this routine is implementation dependent. You MUST call VecRestoreArray() 
   when you no longer need access to the array.

   Input Parameter:
.  x - the vector

   Output Parameter:
.  a - location to put pointer to the array

.keywords: vector, get, array

.seealso: VecRestoreArray()
@*/
int VecGetArray(Vec x,Scalar **a)
{
  VALIDHEADER(x,VEC_COOKIE);
  return (*x->ops->getarray)(x,a);
}

/*@
   VecRestoreArray - Restores a vector after VecGetArray() has been called.

   Input Parameters:
.  x - the vector
.  a - location of pointer to array obtained from VecGetArray()

.keywords: vector, restore, array

.seealso: VecGetArray()
@*/
int VecRestoreArray(Vec x,Scalar **a)
{
  VALIDHEADER(x,VEC_COOKIE);
  if (x->ops->restorearray) return (*x->ops->getarray)(x,a);
  else return 0;
}

/*@
   VecView - Views a vector object. This routine is intended
   to be replaceable with fancy graphical based viewing.

   Input Parameters:
.  v - the vector
.  ptr - an optional visualization context

   Notes:
   The available visualization contexts include
$     STDOUT_VIEWER - standard output (the default)
$     SYNC_STDOUT_VIEWER - synchronized standard
$        output, where only the first processor opens
$        the file.  All other processors send their 
$        data to the first processor to print. 

   The user can open alternative vistualization contexts with
$    ViewerFileOpen() - output to a specified file
$    ViewerFileOpenSync() - synchronized output to a 
$         specified file
$    DrawOpenX() - output vector to an X window display
$    DrawLGCreate() - output vector as a line graph to an X window display
$    ViewerMatlabOpen() - output vector to Matlab viewer

.keywords: Vec, view, visualize

.seealso: ViewerFileOpen(), ViewerFileOpenSync(), DrawOpenX(), 
          DrawLGCreate(), ViewerMatlabOpen()
@*/
int VecView(Vec v,Viewer ptr)
{
  VALIDHEADER(v,VEC_COOKIE);
  return (*v->view)((PetscObject)v,ptr);
}

/*@
   VecGetSize - Returns the global number of elements of the vector.

   Input Parameter:
.  x - the vector

   Output Parameters:
.  size - the global length of the vector

.keywords: vector, get, size, global, dimension

.seealso: VecGetLocalSize()
@*/
int VecGetSize(Vec x,int *size)
{
  VALIDHEADER(x,VEC_COOKIE);
  return (*x->ops->getsize)(x,size);
}

/*@
   VecGetLocalSize - Returns the number of elements of the vector stored 
   in local memory. This routine may be implementation dependent, so use 
   with care.

   Input Parameter:
.  x - the vector

   Output Parameter:
.  size - the length of the local piece of the vector

.keywords: vector, get, dimension, size, local

.seealso: VecGetSize()
@*/
int VecGetLocalSize(Vec x,int *size)
{
  VALIDHEADER(x,VEC_COOKIE);
  return (*x->ops->getlocalsize)(x,size);
}

/*@
   VecGetOwnershipRange - Returns the range of indices owned by 
   this processor, assuming that the vectors are laid out with the
   first n1 elements on the first processor, next n2 elements on the
   second, etc.  For certain parallel layouts this range may not be 
   well-defined. 

   Input Parameter:
.  x - the vector

   Output Parameters:
.  low - the first local element
.  high - one more than the last local element

.keywords: vector, get, range, ownership
@*/
int VecGetOwnershipRange(Vec x,int *low,int *high)
{
  VALIDHEADER(x,VEC_COOKIE);
  return (*x->ops->getownershiprange)(x,low,high);
}

/* Default routines for obtaining and releasing; */
/* may be used by any implementation */
int Veiobtain_vectors(Vec w,int m,Vec **V )
{
  Vec *v;
  int  i;
  *V = v = (Vec *) MALLOC( m * sizeof(Vec *) );
  for (i=0; i<m; i++) VecCreate(w,v+i);
  return 0;
}

int Veirelease_vectors( Vec *v, int m )
{
  int i;
  for (i=0; i<m; i++) VecDestroy(v[i]);
  FREE( v );
  return 0;
}


