/*
   A star forest (SF) describes a communication pattern
*/
#if !defined(__PETSCSF_H)
#define __PETSCSF_H
#include <petscsys.h>

PETSC_EXTERN PetscClassId PETSCSF_CLASSID;

/*S
   PetscSF - PETSc object for setting up and managing the communication of certain entries of arrays and Vecs between MPI processes.

   Level: intermediate

  Concepts: star forest

       PetscSF uses the concept of star forests to indicate and determine the communication patterns concisely and efficiently.
  A star  http://en.wikipedia.org/wiki/Star_(graph_theory) forest is simply a collection of trees of height 1. The leave nodes represent
  "ghost locations" for the root nodes.

.seealso: PetscSFCreate(), VecScatter, VecScatterCreate()
S*/
typedef struct _p_PetscSF* PetscSF;


/*J
    PetscSFType - String with the name of a PetscSF method or the creation function
       with an optional dynamic library name, for example
       http://www.mcs.anl.gov/petsc/lib.so:mysfcreate()

   Level: beginner

   Notes: The two approaches provided are
$     PETSCSFBASIC which uses MPI 1 message passing to perform the communication and
$     PETSCSFWINDOW which uses MPI 2 one-sided operations to perform the communication, this may be more efficient,
$                   but may not be available for all MPI distributions. In particular OpenMPI has bugs in its one-sided
$                   operations that prevent its use.

.seealso: PetscSFSetType(), PetscSF
J*/
typedef const char *PetscSFType;
#define PETSCSFBASIC  "basic"
#define PETSCSFWINDOW "window"

/*S
   PetscSFNode - specifier of owner and index

   Level: beginner

  Concepts: indexing, stride, distribution

.seealso: PetscSFSetGraph()
S*/
typedef struct {
  PetscInt rank;                /* Rank of owner */
  PetscInt index;               /* Index of node on rank */
} PetscSFNode;

/*E
    PetscSFWindowSyncType - Type of synchronization for PETSCSFWINDOW

$  PETSCSF_WINDOW_SYNC_FENCE - simplest model, synchronizing across communicator
$  PETSCSF_WINDOW_SYNC_LOCK - passive model, less synchronous, requires less setup than PETSCSF_WINDOW_SYNC_ACTIVE, but may require more handshakes
$  PETSCSF_WINDOW_SYNC_ACTIVE - active model, provides most information to MPI implementation, needs to construct 2-way process groups (more setup than PETSCSF_WINDOW_SYNC_LOCK)

   Level: advanced

.seealso: PetscSFWindowSetSyncType(), PetscSFWindowGetSyncType()
E*/
typedef enum {PETSCSF_WINDOW_SYNC_FENCE,PETSCSF_WINDOW_SYNC_LOCK,PETSCSF_WINDOW_SYNC_ACTIVE} PetscSFWindowSyncType;
PETSC_EXTERN const char *const PetscSFWindowSyncTypes[];

/*E
    PetscSFDuplicateOption - Aspects to preserve when duplicating a PetscSF

$  PETSCSF_DUPLICATE_CONFONLY - configuration only, user must call PetscSFSetGraph()
$  PETSCSF_DUPLICATE_RANKS - communication ranks preserved, but different graph (allows simpler setup after calling PetscSFSetGraph())
$  PETSCSF_DUPLICATE_GRAPH - entire graph duplicated

   Level: beginner

.seealso: PetscSFDuplicate()
E*/
typedef enum {PETSCSF_DUPLICATE_CONFONLY,PETSCSF_DUPLICATE_RANKS,PETSCSF_DUPLICATE_GRAPH} PetscSFDuplicateOption;
PETSC_EXTERN const char *const PetscSFDuplicateOptions[];

PETSC_EXTERN PetscFunctionList PetscSFunctionList;
PETSC_EXTERN PetscErrorCode PetscSFRegisterDestroy(void);
PETSC_EXTERN PetscErrorCode PetscSFRegisterAll(void);
PETSC_EXTERN PetscErrorCode PetscSFRegister(const char[],const char[],PetscErrorCode (*)(PetscSF));

PETSC_EXTERN PetscErrorCode PetscSFInitializePackage(void);
PETSC_EXTERN PetscErrorCode PetscSFFinalizePackage(void);
PETSC_EXTERN PetscErrorCode PetscSFCreate(MPI_Comm comm,PetscSF*);
PETSC_EXTERN PetscErrorCode PetscSFDestroy(PetscSF*);
PETSC_EXTERN PetscErrorCode PetscSFSetType(PetscSF,PetscSFType);
PETSC_EXTERN PetscErrorCode PetscSFView(PetscSF,PetscViewer);
PETSC_EXTERN PetscErrorCode PetscSFSetUp(PetscSF);
PETSC_EXTERN PetscErrorCode PetscSFSetFromOptions(PetscSF);
PETSC_EXTERN PetscErrorCode PetscSFDuplicate(PetscSF,PetscSFDuplicateOption,PetscSF*);
PETSC_EXTERN PetscErrorCode PetscSFWindowSetSyncType(PetscSF,PetscSFWindowSyncType);
PETSC_EXTERN PetscErrorCode PetscSFWindowGetSyncType(PetscSF,PetscSFWindowSyncType*);
PETSC_EXTERN PetscErrorCode PetscSFSetRankOrder(PetscSF,PetscBool);
PETSC_EXTERN PetscErrorCode PetscSFSetGraph(PetscSF,PetscInt,PetscInt,const PetscInt*,PetscCopyMode,const PetscSFNode*,PetscCopyMode);
PETSC_EXTERN PetscErrorCode PetscSFGetGraph(PetscSF,PetscInt *nroots,PetscInt *nleaves,const PetscInt **ilocal,const PetscSFNode **iremote);
PETSC_EXTERN PetscErrorCode PetscSFGetLeafRange(PetscSF,PetscInt*,PetscInt*);
PETSC_EXTERN PetscErrorCode PetscSFCreateEmbeddedSF(PetscSF,PetscInt nroots,const PetscInt *selected,PetscSF *newsf);
PETSC_EXTERN PetscErrorCode PetscSFReset(PetscSF);
PETSC_EXTERN PetscErrorCode PetscSFGetRanks(PetscSF,PetscInt*,const PetscMPIInt**,const PetscInt**,const PetscInt**,const PetscInt**);
PETSC_EXTERN PetscErrorCode PetscSFGetGroups(PetscSF,MPI_Group*,MPI_Group*);
PETSC_EXTERN PetscErrorCode PetscSFGetMultiSF(PetscSF,PetscSF*);
PETSC_EXTERN PetscErrorCode PetscSFCreateInverseSF(PetscSF,PetscSF*);

/* broadcasts rootdata to leafdata */
PETSC_EXTERN PetscErrorCode PetscSFBcastBegin(PetscSF,MPI_Datatype,const void *rootdata,void *leafdata)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2);
PETSC_EXTERN PetscErrorCode PetscSFBcastEnd(PetscSF,MPI_Datatype,const void *rootdata,void *leafdata)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2);
/* Reduce leafdata into rootdata using provided operation */
PETSC_EXTERN PetscErrorCode PetscSFReduceBegin(PetscSF,MPI_Datatype,const void *leafdata,void *rootdata,MPI_Op)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2);
PETSC_EXTERN PetscErrorCode PetscSFReduceEnd(PetscSF,MPI_Datatype,const void *leafdata,void *rootdata,MPI_Op)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2);
/* Atomically modifies (using provided operation) rootdata using leafdata from each leaf, value at root at time of modification is returned in leafupdate. */
PETSC_EXTERN PetscErrorCode PetscSFFetchAndOpBegin(PetscSF,MPI_Datatype,void *rootdata,const void *leafdata,void *leafupdate,MPI_Op)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2) PetscAttrMPIPointerWithType(5,2);
PETSC_EXTERN PetscErrorCode PetscSFFetchAndOpEnd(PetscSF,MPI_Datatype,void *rootdata,const void *leafdata,void *leafupdate,MPI_Op)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2) PetscAttrMPIPointerWithType(5,2);
/* Compute the degree of every root vertex (number of leaves in its star) */
PETSC_EXTERN PetscErrorCode PetscSFComputeDegreeBegin(PetscSF,const PetscInt **degree);
PETSC_EXTERN PetscErrorCode PetscSFComputeDegreeEnd(PetscSF,const PetscInt **degree);
/* Concatenate data from all leaves into roots */
PETSC_EXTERN PetscErrorCode PetscSFGatherBegin(PetscSF,MPI_Datatype,const void *leafdata,void *multirootdata)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2);
PETSC_EXTERN PetscErrorCode PetscSFGatherEnd(PetscSF,MPI_Datatype,const void *leafdata,void *multirootdata)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2);
/* Distribute distinct values to each leaf from roots */
PETSC_EXTERN PetscErrorCode PetscSFScatterBegin(PetscSF,MPI_Datatype,const void *multirootdata,void *leafdata)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2);
PETSC_EXTERN PetscErrorCode PetscSFScatterEnd(PetscSF,MPI_Datatype,const void *multirootdata,void *leafdata)
  PetscAttrMPIPointerWithType(3,2) PetscAttrMPIPointerWithType(4,2);

#endif
