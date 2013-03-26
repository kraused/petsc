
#include <petsc-private/pcimpl.h>          /*I   "petscpc.h"   I*/

PETSC_EXTERN PetscErrorCode PCCreate_Jacobi(PC);
PETSC_EXTERN PetscErrorCode PCCreate_BJacobi(PC);
PETSC_EXTERN PetscErrorCode PCCreate_PBJacobi(PC);
PETSC_EXTERN PetscErrorCode PCCreate_ILU(PC);
PETSC_EXTERN PetscErrorCode PCCreate_None(PC);
PETSC_EXTERN PetscErrorCode PCCreate_LU(PC);
PETSC_EXTERN PetscErrorCode PCCreate_SOR(PC);
PETSC_EXTERN PetscErrorCode PCCreate_Shell(PC);
PETSC_EXTERN PetscErrorCode PCCreate_MG(PC);
PETSC_EXTERN PetscErrorCode PCCreate_Eisenstat(PC);
PETSC_EXTERN PetscErrorCode PCCreate_ICC(PC);
PETSC_EXTERN PetscErrorCode PCCreate_ASM(PC);
PETSC_EXTERN PetscErrorCode PCCreate_GASM(PC);
PETSC_EXTERN PetscErrorCode PCCreate_KSP(PC);
PETSC_EXTERN PetscErrorCode PCCreate_Composite(PC);
PETSC_EXTERN PetscErrorCode PCCreate_Redundant(PC);
PETSC_EXTERN PetscErrorCode PCCreate_NN(PC);
PETSC_EXTERN PetscErrorCode PCCreate_Cholesky(PC);
PETSC_EXTERN PetscErrorCode PCCreate_FieldSplit(PC);
PETSC_EXTERN PetscErrorCode PCCreate_Galerkin(PC);
PETSC_EXTERN PetscErrorCode PCCreate_HMPI(PC);
PETSC_EXTERN PetscErrorCode PCCreate_Exotic(PC);
PETSC_EXTERN PetscErrorCode PCCreate_ASA(PC);
PETSC_EXTERN PetscErrorCode PCCreate_CP(PC);
PETSC_EXTERN PetscErrorCode PCCreate_LSC(PC);
PETSC_EXTERN PetscErrorCode PCCreate_Redistribute(PC);
PETSC_EXTERN PetscErrorCode PCCreate_SVD(PC);
PETSC_EXTERN PetscErrorCode PCCreate_GAMG(PC);

#if defined(PETSC_HAVE_BOOST) && defined(PETSC_CLANGUAGE_CXX)
PETSC_EXTERN PetscErrorCode PCCreate_SupportGraph(PC);
#endif
#if defined(PETSC_HAVE_ML)
PETSC_EXTERN PetscErrorCode PCCreate_ML(PC);
#endif
#if defined(PETSC_HAVE_SPAI)
PETSC_EXTERN PetscErrorCode PCCreate_SPAI(PC);
#endif
PETSC_EXTERN PetscErrorCode PCCreate_Mat(PC);
#if defined(PETSC_HAVE_HYPRE)
PETSC_EXTERN PetscErrorCode PCCreate_HYPRE(PC);
PETSC_EXTERN PetscErrorCode PCCreate_PFMG(PC);
PETSC_EXTERN PetscErrorCode PCCreate_SysPFMG(PC);
#endif
#if !defined(PETSC_USE_COMPLEX)
PETSC_EXTERN PetscErrorCode PCCreate_TFS(PC);
#endif
#if defined(PETSC_HAVE_CUSP_SMOOTHED_AGGREGATION) && defined(PETSC_HAVE_CUSP)
PETSC_EXTERN PetscErrorCode PCCreate_SACUSP(PC);
PETSC_EXTERN PetscErrorCode PCCreate_SACUSPPoly(PC);
PETSC_EXTERN PetscErrorCode PCCreate_BiCGStabCUSP(PC);
PETSC_EXTERN PetscErrorCode PCCreate_AINVCUSP(PC);
#endif
#if defined(PETSC_HAVE_PARMS)
PETSC_EXTERN PetscErrorCode PCCreate_PARMS(PC);
#endif
#if defined(PETSC_HAVE_PCBDDC)
PETSC_EXTERN PetscErrorCode PCCreate_BDDC(PC);
#endif

#undef __FUNCT__
#define __FUNCT__ "PCRegisterAll"
/*@C
   PCRegisterAll - Registers all of the preconditioners in the PC package.

   Not Collective

   Input Parameter:
.  path - the library where the routines are to be found (optional)

   Level: advanced

.keywords: PC, register, all

.seealso: PCRegister(), PCRegisterDestroy()
@*/
PetscErrorCode  PCRegisterAll(void)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  PCRegisterAllCalled = PETSC_TRUE;

  ierr = PCRegister(PCNONE         ,"PCCreate_None",PCCreate_None);CHKERRQ(ierr);
  ierr = PCRegister(PCJACOBI       ,"PCCreate_Jacobi",PCCreate_Jacobi);CHKERRQ(ierr);
  ierr = PCRegister(PCPBJACOBI     ,"PCCreate_PBJacobi",PCCreate_PBJacobi);CHKERRQ(ierr);
  ierr = PCRegister(PCBJACOBI      ,"PCCreate_BJacobi",PCCreate_BJacobi);CHKERRQ(ierr);
  ierr = PCRegister(PCSOR          ,"PCCreate_SOR",PCCreate_SOR);CHKERRQ(ierr);
  ierr = PCRegister(PCLU           ,"PCCreate_LU",PCCreate_LU);CHKERRQ(ierr);
  ierr = PCRegister(PCSHELL        ,"PCCreate_Shell",PCCreate_Shell);CHKERRQ(ierr);
  ierr = PCRegister(PCMG           ,"PCCreate_MG",PCCreate_MG);CHKERRQ(ierr);
  ierr = PCRegister(PCEISENSTAT    ,"PCCreate_Eisenstat",PCCreate_Eisenstat);CHKERRQ(ierr);
  ierr = PCRegister(PCILU          ,"PCCreate_ILU",PCCreate_ILU);CHKERRQ(ierr);
  ierr = PCRegister(PCICC          ,"PCCreate_ICC",PCCreate_ICC);CHKERRQ(ierr);
  ierr = PCRegister(PCCHOLESKY     ,"PCCreate_Cholesky",PCCreate_Cholesky);CHKERRQ(ierr);
  ierr = PCRegister(PCASM          ,"PCCreate_ASM",PCCreate_ASM);CHKERRQ(ierr);
  ierr = PCRegister(PCGASM         ,"PCCreate_GASM",PCCreate_GASM);CHKERRQ(ierr);
  ierr = PCRegister(PCKSP          ,"PCCreate_KSP",PCCreate_KSP);CHKERRQ(ierr);
  ierr = PCRegister(PCCOMPOSITE    ,"PCCreate_Composite",PCCreate_Composite);CHKERRQ(ierr);
  ierr = PCRegister(PCREDUNDANT    ,"PCCreate_Redundant",PCCreate_Redundant);CHKERRQ(ierr);
  ierr = PCRegister(PCNN           ,"PCCreate_NN",PCCreate_NN);CHKERRQ(ierr);
  ierr = PCRegister(PCMAT          ,"PCCreate_Mat",PCCreate_Mat);CHKERRQ(ierr);
  ierr = PCRegister(PCFIELDSPLIT   ,"PCCreate_FieldSplit",PCCreate_FieldSplit);CHKERRQ(ierr);
  ierr = PCRegister(PCGALERKIN     ,"PCCreate_Galerkin",PCCreate_Galerkin);CHKERRQ(ierr);
  ierr = PCRegister(PCEXOTIC       ,"PCCreate_Exotic",PCCreate_Exotic);CHKERRQ(ierr);
  ierr = PCRegister(PCHMPI         ,"PCCreate_HMPI",PCCreate_HMPI);CHKERRQ(ierr);
  ierr = PCRegister(PCASA          ,"PCCreate_ASA",PCCreate_ASA);CHKERRQ(ierr);
  ierr = PCRegister(PCCP           ,"PCCreate_CP",PCCreate_CP);CHKERRQ(ierr);
  ierr = PCRegister(PCLSC          ,"PCCreate_LSC",PCCreate_LSC);CHKERRQ(ierr);
  ierr = PCRegister(PCREDISTRIBUTE ,"PCCreate_Redistribute",PCCreate_Redistribute);CHKERRQ(ierr);
  ierr = PCRegister(PCSVD          ,"PCCreate_SVD",PCCreate_SVD);CHKERRQ(ierr);
  ierr = PCRegister(PCGAMG         ,"PCCreate_GAMG",PCCreate_GAMG);CHKERRQ(ierr);
#if defined(PETSC_HAVE_BOOST) && defined(PETSC_CLANGUAGE_CXX)
  ierr = PCRegister(PCSUPPORTGRAPH ,"PCCreate_SupportGraph",PCCreate_SupportGraph);CHKERRQ(ierr);
#endif
#if defined(PETSC_HAVE_ML)
  ierr = PCRegister(PCML           ,"PCCreate_ML",PCCreate_ML);CHKERRQ(ierr);
#endif
#if defined(PETSC_HAVE_SPAI)
  ierr = PCRegister(PCSPAI         ,"PCCreate_SPAI",PCCreate_SPAI);CHKERRQ(ierr);
#endif
#if defined(PETSC_HAVE_HYPRE)
  ierr = PCRegister(PCHYPRE        ,"PCCreate_HYPRE",PCCreate_HYPRE);CHKERRQ(ierr);
  ierr = PCRegister(PCPFMG         ,"PCCreate_PFMG",PCCreate_PFMG);CHKERRQ(ierr);
  ierr = PCRegister(PCSYSPFMG      ,"PCCreate_SysPFMG",PCCreate_SysPFMG);CHKERRQ(ierr);
#endif
#if !defined(PETSC_USE_COMPLEX)
  ierr = PCRegister(PCTFS          ,"PCCreate_TFS",PCCreate_TFS);CHKERRQ(ierr);
#endif
#if defined(PETSC_HAVE_CUSP_SMOOTHED_AGGREGATION) && defined(PETSC_HAVE_CUSP)
  ierr = PCRegister(PCSACUSP       ,"PCCreate_SACUSP",PCCreate_SACUSP);CHKERRQ(ierr);
  ierr = PCRegister(PCAINVCUSP     ,"PCCreate_AINVCUSP",PCCreate_AINVCUSP);CHKERRQ(ierr);
  ierr = PCRegister(PCBICGSTABCUSP ,"PCCreate_BiCGStabCUSP",PCCreate_BiCGStabCUSP);CHKERRQ(ierr);
  ierr = PCRegister(PCSACUSPPOLY   ,"PCCreate_SACUSPPoly",PCCreate_SACUSPPoly);CHKERRQ(ierr);
#endif
#if defined(PETSC_HAVE_PARMS)
  ierr = PCRegister(PCPARMS        ,"PCCreate_PARMS",PCCreate_PARMS);CHKERRQ(ierr);
#endif
#if defined(PETSC_HAVE_PCBDDC)
  ierr = PCRegister(PCBDDC         ,"PCCreate_BDDC",PCCreate_BDDC);CHKERRQ(ierr);
#endif
  PetscFunctionReturn(0);
}
