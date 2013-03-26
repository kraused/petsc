
#include <petsc-private/snesimpl.h>     /*I  "petscsnes.h"  I*/

PETSC_EXTERN PetscErrorCode SNESCreate_NEWTONLS(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_NEWTONTR(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_Test(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_NRichardson(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_KSPONLY(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_VINEWTONRSLS(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_VINEWTONSSLS(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_NGMRES(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_QN(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_Shell(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_GS(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_NCG(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_FAS(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_MS(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_NASM(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_Anderson(SNES);
PETSC_EXTERN PetscErrorCode SNESCreate_ASPIN(SNES);

const char *SNESConvergedReasons_Shifted[] = {" "," ","DIVERGED_LOCAL_MIN","DIVERGED_INNER","DIVERGED_LINE_SEARCH","DIVERGED_MAX_IT",
                                              "DIVERGED_FNORM_NAN","DIVERGED_LINEAR_SOLVE","DIVERGED_FUNCTION_COUNT","DIVERGED_FUNCTION_DOMAIN",
                                              "CONVERGED_ITERATING"," ","CONVERGED_FNORM_ABS","CONVERGED_FNORM_RELATIVE",
                                              "CONVERGED_SNORM_RELATIVE","CONVERGED_ITS"," ","CONVERGED_TR_DELTA","SNESConvergedReason","",0};
const char *const *SNESConvergedReasons = SNESConvergedReasons_Shifted + 10;

const char *SNESNormTypes_Shifted[]    = {"DEFAULT","NONE","FUNCTION","INITIALONLY","FINALONLY","INITIALFINALONLY","SNESNormType","SNES_NORM_",0};
const char *const *const SNESNormTypes = SNESNormTypes_Shifted + 1;

/*
      This is used by SNESSetType() to make sure that at least one
    SNESRegisterAll() is called. In general, if there is more than one
    DLL then SNESRegisterAll() may be called several times.
*/
extern PetscBool SNESRegisterAllCalled;

#undef __FUNCT__
#define __FUNCT__ "SNESRegisterAll"
/*@C
   SNESRegisterAll - Registers all of the nonlinear solver methods in the SNES package.

   Not Collective

   Level: advanced

.keywords: SNES, register, all

.seealso:  SNESRegisterDestroy()
@*/
PetscErrorCode  SNESRegisterAll(void)
{
  PetscErrorCode ierr;

  PetscFunctionBegin;
  SNESRegisterAllCalled = PETSC_TRUE;

  ierr = SNESRegister(SNESNEWTONLS,     "SNESCreate_NEWTONLS",     SNESCreate_NEWTONLS);CHKERRQ(ierr);
  ierr = SNESRegister(SNESNEWTONTR,     "SNESCreate_NEWTONTR",     SNESCreate_NEWTONTR);CHKERRQ(ierr);
  ierr = SNESRegister(SNESTEST,         "SNESCreate_Test",         SNESCreate_Test);CHKERRQ(ierr);
  ierr = SNESRegister(SNESNRICHARDSON,  "SNESCreate_NRichardson",  SNESCreate_NRichardson);CHKERRQ(ierr);
  ierr = SNESRegister(SNESKSPONLY,      "SNESCreate_KSPONLY",      SNESCreate_KSPONLY);CHKERRQ(ierr);
  ierr = SNESRegister(SNESVINEWTONRSLS, "SNESCreate_VINEWTONRSLS", SNESCreate_VINEWTONRSLS);CHKERRQ(ierr);
  ierr = SNESRegister(SNESVINEWTONSSLS, "SNESCreate_VINEWTONSSLS", SNESCreate_VINEWTONSSLS);CHKERRQ(ierr);
  ierr = SNESRegister(SNESNGMRES,       "SNESCreate_NGMRES",       SNESCreate_NGMRES);CHKERRQ(ierr);
  ierr = SNESRegister(SNESQN,           "SNESCreate_QN",           SNESCreate_QN);CHKERRQ(ierr);
  ierr = SNESRegister(SNESSHELL,        "SNESCreate_Shell",        SNESCreate_Shell);CHKERRQ(ierr);
  ierr = SNESRegister(SNESGS,           "SNESCreate_GS",           SNESCreate_GS);CHKERRQ(ierr);
  ierr = SNESRegister(SNESNCG,          "SNESCreate_NCG",          SNESCreate_NCG);CHKERRQ(ierr);
  ierr = SNESRegister(SNESFAS,          "SNESCreate_FAS",          SNESCreate_FAS);CHKERRQ(ierr);
  ierr = SNESRegister(SNESMS,           "SNESCreate_MS",           SNESCreate_MS);CHKERRQ(ierr);
  ierr = SNESRegister(SNESNASM,         "SNESCreate_NASM",         SNESCreate_NASM);CHKERRQ(ierr);
  ierr = SNESRegister(SNESANDERSON,     "SNESCreate_Anderson",     SNESCreate_Anderson);CHKERRQ(ierr);
  ierr = SNESRegister(SNESASPIN,        "SNESCreate_ASPIN",        SNESCreate_ASPIN);CHKERRQ(ierr);
  PetscFunctionReturn(0);
}
