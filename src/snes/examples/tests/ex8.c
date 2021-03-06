#include <petscsnes.h>
#include <petscdmda.h>

static char help[] = "Parallel version of the minimum surface area problem using DMs.\n\
See ex10.c for the serial version. It solves a system of nonlinear equations in mixed\n\
complementarity form using semismooth newton algorithm.This example is based on a\n\
problem from the MINPACK-2 test suite.  Given a rectangular 2-D domain and\n\
boundary values along the edges of the domain, the objective is to find the\n\
surface with the minimal area that satisfies the boundary conditions.\n\
This application solves this problem using complimentarity -- We are actually\n\
solving the system  (grad f)_i >= 0, if x_i == l_i \n\
                    (grad f)_i = 0, if l_i < x_i < u_i \n\
                    (grad f)_i <= 0, if x_i == u_i  \n\
where f is the function to be minimized. \n\
\n\
The command line options are:\n\
  -da_grid_x <nx>, where <nx> = number of grid points in the 1st coordinate direction\n\
  -da_grid_y <ny>, where <ny> = number of grid points in the 2nd coordinate direction\n\
  -start <st>, where <st> =0 for zero vector, and an average of the boundary conditions otherwise\n\
  -lb <value>, lower bound on the variables\n\
  -ub <value>, upper bound on the variables\n\n";

/*
   User-defined application context - contains data needed by the
   application-provided call-back routines, FormJacobian() and
   FormFunction().
*/

typedef struct {
  DM          da;
  PetscScalar *bottom, *top, *left, *right;
  PetscInt    mx,my;
} AppCtx;


/* -------- User-defined Routines --------- */

extern PetscErrorCode MSA_BoundaryConditions(AppCtx*);
extern PetscErrorCode MSA_InitialPoint(AppCtx*, Vec);
extern PetscErrorCode FormGradient(SNES, Vec, Vec, void*);
extern PetscErrorCode FormJacobian(SNES, Vec, Mat*, Mat*, MatStructure*,void*);

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc, char **argv)
{
  PetscErrorCode info;              /* used to check for functions returning nonzeros */
  Vec            x,r;               /* solution and residual vectors */
  Vec            xl,xu;             /* Bounds on the variables */
  PetscBool      flg_l,flg_u;       /* flags to check if the bounds are set */
  SNES           snes;              /* nonlinear solver context */
  Mat            J;                 /* Jacobian matrix */
  PetscInt       N;                 /* Number of elements in vector */
  PetscScalar    lb = .05;
  PetscScalar    ub = SNES_VI_INF;
  AppCtx         user;              /* user-defined work context */
  PetscBool      flg;

  /* Initialize PETSc */
  PetscInitialize(&argc, &argv, (char*)0, help);

#if defined(PETSC_USE_COMPLEX)
  SETERRQ(PETSC_COMM_WORLD,PETSC_ERR_SUP,"This example does not work for scalar type complex\n");
#endif

  /* Check if lower and upper bounds are set */
  info = PetscOptionsGetScalar(NULL, "-lb", &lb, &flg_l);CHKERRQ(info);
  info = PetscOptionsGetScalar(NULL, "-ub", &ub, &flg_u);CHKERRQ(info);

  /* Create distributed array to manage the 2d grid */
  info = DMDACreate2d(PETSC_COMM_WORLD, DMDA_BOUNDARY_NONE, DMDA_BOUNDARY_NONE,DMDA_STENCIL_BOX,-4,-4,PETSC_DECIDE,PETSC_DECIDE,1,1,NULL,NULL,&user.da);CHKERRQ(info);
  info = DMDAGetInfo(user.da,PETSC_IGNORE,&user.mx,&user.my,PETSC_IGNORE,PETSC_IGNORE,PETSC_IGNORE,PETSC_IGNORE,PETSC_IGNORE,PETSC_IGNORE,PETSC_IGNORE,PETSC_IGNORE,PETSC_IGNORE,PETSC_IGNORE);CHKERRQ(info);
  /* Extract global vectors from DMDA; */
  info = DMCreateGlobalVector(user.da,&x);CHKERRQ(info);
  info = VecDuplicate(x, &r);CHKERRQ(info);

  N    = user.mx*user.my;
  info = DMCreateMatrix(user.da,MATAIJ,&J);CHKERRQ(info);

  /* Create nonlinear solver context */
  info = SNESCreate(PETSC_COMM_WORLD,&snes);CHKERRQ(info);

  /*  Set function evaluation and Jacobian evaluation  routines */
  info = SNESSetFunction(snes,r,FormGradient,&user);CHKERRQ(info);
  info = SNESSetJacobian(snes,J,J,FormJacobian,&user);CHKERRQ(info);

  /* Set the boundary conditions */
  info = MSA_BoundaryConditions(&user);CHKERRQ(info);

  /* Set initial solution guess */
  info = MSA_InitialPoint(&user, x);CHKERRQ(info);


  /* Set Bounds on variables */
  info = VecDuplicate(x, &xl);CHKERRQ(info);
  info = VecDuplicate(x, &xu);CHKERRQ(info);
  info = VecSet(xl, lb);CHKERRQ(info);
  info = VecSet(xu, ub);CHKERRQ(info);

  info = SNESVISetVariableBounds(snes,xl,xu);CHKERRQ(info);

  info = SNESSetFromOptions(snes);CHKERRQ(info);

  /* Solve the application */
  info = SNESSolve(snes,NULL,x);CHKERRQ(info);

  info = PetscOptionsHasName(NULL,"-view_sol",&flg);CHKERRQ(info);
  if (flg) { info = VecView(x,PETSC_VIEWER_STDOUT_WORLD);CHKERRQ(info); }

  /* Free memory */
  info = VecDestroy(&x);CHKERRQ(info);
  info = VecDestroy(&xl);CHKERRQ(info);
  info = VecDestroy(&xu);CHKERRQ(info);
  info = VecDestroy(&r);CHKERRQ(info);
  info = MatDestroy(&J);CHKERRQ(info);
  info = SNESDestroy(&snes);CHKERRQ(info);

  /* Free user-created data structures */
  info = DMDestroy(&user.da);CHKERRQ(info);
  info = PetscFree(user.bottom);CHKERRQ(info);
  info = PetscFree(user.top);CHKERRQ(info);
  info = PetscFree(user.left);CHKERRQ(info);
  info = PetscFree(user.right);CHKERRQ(info);

  info = PetscFinalize();

  return 0;
}

/* -------------------------------------------------------------------- */
#undef __FUNCT__
#define __FUNCT__ "FormGradient"

/*  FormGradient - Evaluates gradient of f.

    Input Parameters:
.   snes  - the SNES context
.   X     - input vector
.   ptr   - optional user-defined context, as set by SNESSetFunction()

    Output Parameters:
.   G - vector containing the newly evaluated gradient
*/
PetscErrorCode FormGradient(SNES snes, Vec X, Vec G, void *ptr)
{
  AppCtx      *user = (AppCtx*) ptr;
  int         info;
  PetscInt    i,j;
  PetscInt    mx=user->mx, my=user->my;
  PetscScalar hx=1.0/(mx+1),hy=1.0/(my+1), hydhx=hy/hx, hxdhy=hx/hy;
  PetscScalar f1,f2,f3,f4,f5,f6,d1,d2,d3,d4,d5,d6,d7,d8,xc,xl,xr,xt,xb,xlt,xrb;
  PetscScalar df1dxc,df2dxc,df3dxc,df4dxc,df5dxc,df6dxc;
  PetscScalar **g, **x;
  PetscInt    xs,xm,ys,ym;
  Vec         localX;

  PetscFunctionBeginUser;
  /* Initialize vector to zero */
  info = VecSet(G,0.0);CHKERRQ(info);

  /* Get local vector */
  info = DMGetLocalVector(user->da,&localX);CHKERRQ(info);
  /* Get ghost points */
  info = DMGlobalToLocalBegin(user->da,X,INSERT_VALUES,localX);CHKERRQ(info);
  info = DMGlobalToLocalEnd(user->da,X,INSERT_VALUES,localX);CHKERRQ(info);
  /* Get pointer to local vector data */
  info = DMDAVecGetArray(user->da,localX, &x);CHKERRQ(info);
  info = DMDAVecGetArray(user->da,G, &g);CHKERRQ(info);

  info = DMDAGetCorners(user->da,&xs,&ys,NULL,&xm,&ym,NULL);CHKERRQ(info);
  /* Compute function over the locally owned part of the mesh */
  for (j=ys; j < ys+ym; j++) {
    for (i=xs; i< xs+xm; i++) {

      xc = x[j][i];
      xlt=xrb=xl=xr=xb=xt=xc;

      if (i==0) { /* left side */
        xl  = user->left[j+1];
        xlt = user->left[j+2];
      } else xl = x[j][i-1];

      if (j==0) { /* bottom side */
        xb  = user->bottom[i+1];
        xrb = user->bottom[i+2];
      } else xb = x[j-1][i];

      if (i+1 == mx) { /* right side */
        xr  = user->right[j+1];
        xrb = user->right[j];
      } else xr = x[j][i+1];

      if (j+1==0+my) { /* top side */
        xt  = user->top[i+1];
        xlt = user->top[i];
      } else xt = x[j+1][i];

      if (i>0 && j+1<my) xlt = x[j+1][i-1]; /* left top side */
      if (j>0 && i+1<mx) xrb = x[j-1][i+1]; /* right bottom */

      d1 = (xc-xl);
      d2 = (xc-xr);
      d3 = (xc-xt);
      d4 = (xc-xb);
      d5 = (xr-xrb);
      d6 = (xrb-xb);
      d7 = (xlt-xl);
      d8 = (xt-xlt);

      df1dxc = d1*hydhx;
      df2dxc = (d1*hydhx + d4*hxdhy);
      df3dxc = d3*hxdhy;
      df4dxc = (d2*hydhx + d3*hxdhy);
      df5dxc = d2*hydhx;
      df6dxc = d4*hxdhy;

      d1 /= hx;
      d2 /= hx;
      d3 /= hy;
      d4 /= hy;
      d5 /= hy;
      d6 /= hx;
      d7 /= hy;
      d8 /= hx;

      f1 = PetscSqrtReal(1.0 + d1*d1 + d7*d7);
      f2 = PetscSqrtReal(1.0 + d1*d1 + d4*d4);
      f3 = PetscSqrtReal(1.0 + d3*d3 + d8*d8);
      f4 = PetscSqrtReal(1.0 + d3*d3 + d2*d2);
      f5 = PetscSqrtReal(1.0 + d2*d2 + d5*d5);
      f6 = PetscSqrtReal(1.0 + d4*d4 + d6*d6);

      df1dxc /= f1;
      df2dxc /= f2;
      df3dxc /= f3;
      df4dxc /= f4;
      df5dxc /= f5;
      df6dxc /= f6;

      g[j][i] = (df1dxc+df2dxc+df3dxc+df4dxc+df5dxc+df6dxc)/2.0;

    }
  }

  /* Restore vectors */
  info = DMDAVecRestoreArray(user->da,localX, &x);CHKERRQ(info);
  info = DMDAVecRestoreArray(user->da,G, &g);CHKERRQ(info);
  info = DMRestoreLocalVector(user->da,&localX);CHKERRQ(info);
  info = PetscLogFlops(67*mx*my);CHKERRQ(info);
  PetscFunctionReturn(0);
}

/* ------------------------------------------------------------------- */
#undef __FUNCT__
#define __FUNCT__ "FormJacobian"
/*
   FormJacobian - Evaluates Jacobian matrix.

   Input Parameters:
.  snes - SNES context
.  X    - input vector
.  ptr  - optional user-defined context, as set by SNESSetJacobian()

   Output Parameters:
.  tH    - Jacobian matrix

*/
PetscErrorCode FormJacobian(SNES snes, Vec X, Mat *tH, Mat *tHPre, MatStructure *flag, void *ptr)
{
  AppCtx         *user = (AppCtx*) ptr;
  Mat            H     = *tH;
  PetscErrorCode info;
  PetscInt       i,j,k;
  PetscInt       mx=user->mx, my=user->my;
  MatStencil     row,col[7];
  PetscScalar    hx=1.0/(mx+1), hy=1.0/(my+1), hydhx=hy/hx, hxdhy=hx/hy;
  PetscScalar    f1,f2,f3,f4,f5,f6,d1,d2,d3,d4,d5,d6,d7,d8,xc,xl,xr,xt,xb,xlt,xrb;
  PetscScalar    hl,hr,ht,hb,hc,htl,hbr;
  PetscScalar    **x, v[7];
  PetscBool      assembled;
  PetscInt       xs,xm,ys,ym;
  Vec            localX;

  PetscFunctionBeginUser;
  /* Set various matrix options */
  info = MatAssembled(H,&assembled);CHKERRQ(info);
  if (assembled) {info = MatZeroEntries(H);CHKERRQ(info);}
  *flag=SAME_NONZERO_PATTERN;

  /* Get local vector */
  info = DMGetLocalVector(user->da,&localX);CHKERRQ(info);
  /* Get ghost points */
  info = DMGlobalToLocalBegin(user->da,X,INSERT_VALUES,localX);CHKERRQ(info);
  info = DMGlobalToLocalEnd(user->da,X,INSERT_VALUES,localX);CHKERRQ(info);

  /* Get pointers to vector data */
  info = DMDAVecGetArray(user->da,localX, &x);CHKERRQ(info);

  info = DMDAGetCorners(user->da,&xs,&ys,NULL,&xm,&ym,NULL);CHKERRQ(info);
  /* Compute Jacobian over the locally owned part of the mesh */
  for (j=ys; j< ys+ym; j++) {
    for (i=xs; i< xs+xm; i++) {
      xc = x[j][i];
      xlt=xrb=xl=xr=xb=xt=xc;

      /* Left */
      if (i==0) {
        xl  = user->left[j+1];
        xlt = user->left[j+2];
      } else xl = x[j][i-1];

      /* Bottom */
      if (j==0) {
        xb  = user->bottom[i+1];
        xrb = user->bottom[i+2];
      } else xb = x[j-1][i];

      /* Right */
      if (i+1 == mx) {
        xr  = user->right[j+1];
        xrb = user->right[j];
      } else xr = x[j][i+1];

      /* Top */
      if (j+1==my) {
        xt  = user->top[i+1];
        xlt = user->top[i];
      } else xt = x[j+1][i];

      /* Top left */
      if (i>0 && j+1<my) xlt = x[j+1][i-1];

      /* Bottom right */
      if (j>0 && i+1<mx) xrb = x[j-1][i+1];

      d1 = (xc-xl)/hx;
      d2 = (xc-xr)/hx;
      d3 = (xc-xt)/hy;
      d4 = (xc-xb)/hy;
      d5 = (xrb-xr)/hy;
      d6 = (xrb-xb)/hx;
      d7 = (xlt-xl)/hy;
      d8 = (xlt-xt)/hx;

      f1 = PetscSqrtReal(1.0 + d1*d1 + d7*d7);
      f2 = PetscSqrtReal(1.0 + d1*d1 + d4*d4);
      f3 = PetscSqrtReal(1.0 + d3*d3 + d8*d8);
      f4 = PetscSqrtReal(1.0 + d3*d3 + d2*d2);
      f5 = PetscSqrtReal(1.0 + d2*d2 + d5*d5);
      f6 = PetscSqrtReal(1.0 + d4*d4 + d6*d6);


      hl = (-hydhx*(1.0+d7*d7)+d1*d7)/(f1*f1*f1)+
           (-hydhx*(1.0+d4*d4)+d1*d4)/(f2*f2*f2);
      hr = (-hydhx*(1.0+d5*d5)+d2*d5)/(f5*f5*f5)+
           (-hydhx*(1.0+d3*d3)+d2*d3)/(f4*f4*f4);
      ht = (-hxdhy*(1.0+d8*d8)+d3*d8)/(f3*f3*f3)+
           (-hxdhy*(1.0+d2*d2)+d2*d3)/(f4*f4*f4);
      hb = (-hxdhy*(1.0+d6*d6)+d4*d6)/(f6*f6*f6)+
           (-hxdhy*(1.0+d1*d1)+d1*d4)/(f2*f2*f2);

      hbr = -d2*d5/(f5*f5*f5) - d4*d6/(f6*f6*f6);
      htl = -d1*d7/(f1*f1*f1) - d3*d8/(f3*f3*f3);

      hc = hydhx*(1.0+d7*d7)/(f1*f1*f1) + hxdhy*(1.0+d8*d8)/(f3*f3*f3) +
           hydhx*(1.0+d5*d5)/(f5*f5*f5) + hxdhy*(1.0+d6*d6)/(f6*f6*f6) +
           (hxdhy*(1.0+d1*d1)+hydhx*(1.0+d4*d4)-2*d1*d4)/(f2*f2*f2) +
           (hxdhy*(1.0+d2*d2)+hydhx*(1.0+d3*d3)-2*d2*d3)/(f4*f4*f4);

      hl/=2.0; hr/=2.0; ht/=2.0; hb/=2.0; hbr/=2.0; htl/=2.0;  hc/=2.0;

      k     =0;
      row.i = i;row.j= j;
      /* Bottom */
      if (j>0) {
        v[k]     =hb;
        col[k].i = i; col[k].j=j-1; k++;
      }

      /* Bottom right */
      if (j>0 && i < mx -1) {
        v[k]     =hbr;
        col[k].i = i+1; col[k].j = j-1; k++;
      }

      /* left */
      if (i>0) {
        v[k]     = hl;
        col[k].i = i-1; col[k].j = j; k++;
      }

      /* Centre */
      v[k]= hc; col[k].i= row.i; col[k].j = row.j; k++;

      /* Right */
      if (i < mx-1) {
        v[k]    = hr;
        col[k].i= i+1; col[k].j = j;k++;
      }

      /* Top left */
      if (i>0 && j < my-1) {
        v[k]     = htl;
        col[k].i = i-1;col[k].j = j+1; k++;
      }

      /* Top */
      if (j < my-1) {
        v[k]     = ht;
        col[k].i = i; col[k].j = j+1; k++;
      }

      info = MatSetValuesStencil(H,1,&row,k,col,v,INSERT_VALUES);CHKERRQ(info);
    }
  }

  /* Assemble the matrix */
  info = MatAssemblyBegin(H,MAT_FINAL_ASSEMBLY);CHKERRQ(info);
  info = DMDAVecRestoreArray(user->da,localX,&x);CHKERRQ(info);
  info = MatAssemblyEnd(H,MAT_FINAL_ASSEMBLY);CHKERRQ(info);
  info = DMRestoreLocalVector(user->da,&localX);CHKERRQ(info);

  info = PetscLogFlops(199*mx*my);CHKERRQ(info);
  PetscFunctionReturn(0);
}

/* ------------------------------------------------------------------- */
#undef __FUNCT__
#define __FUNCT__ "MSA_BoundaryConditions"
/*
   MSA_BoundaryConditions -  Calculates the boundary conditions for
   the region.

   Input Parameter:
.  user - user-defined application context

   Output Parameter:
.  user - user-defined application context
*/
PetscErrorCode MSA_BoundaryConditions(AppCtx * user)
{
  PetscErrorCode info;
  PetscInt       i,j,k,limit=0,maxits=5;
  PetscInt       mx   =user->mx,my=user->my;
  PetscInt       bsize=0, lsize=0, tsize=0, rsize=0;
  PetscScalar    one  =1.0, two=2.0, three=3.0, tol=1e-10;
  PetscScalar    fnorm,det,hx,hy,xt=0,yt=0;
  PetscScalar    u1,u2,nf1,nf2,njac11,njac12,njac21,njac22;
  PetscScalar    b=-0.5, t=0.5, l=-0.5, r=0.5;
  PetscScalar    *boundary;

  PetscFunctionBeginUser;
  bsize=mx+2; lsize=my+2; rsize=my+2; tsize=mx+2;

  info = PetscMalloc(bsize*sizeof(PetscScalar), &user->bottom);CHKERRQ(info);
  info = PetscMalloc(tsize*sizeof(PetscScalar), &user->top);CHKERRQ(info);
  info = PetscMalloc(lsize*sizeof(PetscScalar), &user->left);CHKERRQ(info);
  info = PetscMalloc(rsize*sizeof(PetscScalar), &user->right);CHKERRQ(info);

  hx= (r-l)/(mx+1); hy=(t-b)/(my+1);

  for (j=0; j<4; j++) {
    if (j==0) {
      yt       = b;
      xt       = l;
      limit    = bsize;
      boundary = user->bottom;
    } else if (j==1) {
      yt       = t;
      xt       = l;
      limit    = tsize;
      boundary = user->top;
    } else if (j==2) {
      yt       = b;
      xt       = l;
      limit    = lsize;
      boundary = user->left;
    } else { /* if  (j==3) */
      yt       = b;
      xt       = r;
      limit    = rsize;
      boundary = user->right;
    }

    for (i=0; i<limit; i++) {
      u1=xt;
      u2=-yt;
      for (k=0; k<maxits; k++) {
        nf1   = u1 + u1*u2*u2 - u1*u1*u1/three-xt;
        nf2   = -u2 - u1*u1*u2 + u2*u2*u2/three-yt;
        fnorm = PetscSqrtReal(nf1*nf1+nf2*nf2);
        if (fnorm <= tol) break;
        njac11 = one+u2*u2-u1*u1;
        njac12 = two*u1*u2;
        njac21 = -two*u1*u2;
        njac22 = -one - u1*u1 + u2*u2;
        det    = njac11*njac22-njac21*njac12;
        u1     = u1-(njac22*nf1-njac12*nf2)/det;
        u2     = u2-(njac11*nf2-njac21*nf1)/det;
      }

      boundary[i]=u1*u1-u2*u2;
      if (j==0 || j==1) xt=xt+hx;
      else yt=yt+hy; /* if (j==2 || j==3) */
    }
  }
  PetscFunctionReturn(0);
}

/* ------------------------------------------------------------------- */
#undef __FUNCT__
#define __FUNCT__ "MSA_InitialPoint"
/*
   MSA_InitialPoint - Calculates the initial guess in one of three ways.

   Input Parameters:
.  user - user-defined application context
.  X - vector for initial guess

   Output Parameters:
.  X - newly computed initial guess
*/
PetscErrorCode MSA_InitialPoint(AppCtx * user, Vec X)
{
  PetscErrorCode info;
  PetscInt       start=-1,i,j;
  PetscScalar    zero =0.0;
  PetscBool      flg;

  PetscFunctionBeginUser;
  info = PetscOptionsGetInt(NULL,"-start",&start,&flg);CHKERRQ(info);

  if (flg && start==0) { /* The zero vector is reasonable */

    info = VecSet(X, zero);CHKERRQ(info);
    /* PLogInfo(user,"Min. Surface Area Problem: Start with 0 vector \n"); */


  } else { /* Take an average of the boundary conditions */
    PetscInt    mx=user->mx,my=user->my;
    PetscScalar **x;
    PetscInt    xs,xm,ys,ym;

    /* Get pointers to vector data */
    info = DMDAVecGetArray(user->da,X,&x);CHKERRQ(info);
    info = DMDAGetCorners(user->da,&xs,&ys,NULL,&xm,&ym,NULL);CHKERRQ(info);

    /* Perform local computations */
    for (j=ys; j<ys+ym; j++) {
      for (i=xs; i< xs+xm; i++) {
        x[j][i] = (((j+1)*user->bottom[i+1]+(my-j+1)*user->top[i+1])/(my+2)+
                   ((i+1)*user->left[j+1]+(mx-i+1)*user->right[j+1])/(mx+2))/2.0;
      }
    }

    /* Restore vectors */
    info = DMDAVecRestoreArray(user->da,X,&x);CHKERRQ(info);

  }
  PetscFunctionReturn(0);
}
