/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas-fargo.cpp
 Author      : Matthias Thimm
 Version     : 0.1
 Copyright   : GPL3
 Description : The taas-fargo solver for abstract argumentation.

============================================================================
*/
#define COMPUTATION_FINISHED 0
#define COMPUTATION_ABORTED__ANSWER_YES 1
#define COMPUTATION_ABORTED__ANSWER_NO  2
#define COMPUTATION_ABORTED__ANSWER_EMPTYSET  3
#define COMPUTATION_ABORTED__ANSWER_EMPTYEMPTYSET  4
#define COMPUTATION_FINISHED__EMPTY_GROUNDED  5
#define COMPUTATION_FINISHED__NONEMPTY_GROUNDED  6
#define COMPUTATION_FINISHED__ANSWER_NO  7

#define TRUE 1
#define FALSE 0
/* ============================================================================================================== */
/* ============================================================================================================== */
#include <stdio.h>
#include <ctype.h>
#include <glib.h>
#include <assert.h>
#include <stdbool.h>

#include "util/bitset.c"
#include "util/miscutil.c"

#include "taas/taas_aaf.c"
#include "taas/taas_inout.c"
#include "taas/taas_labeling.c"
#include "taas/taas_basics.c"

/* ============================================================================================================== */
/* ============================================================================================================== */
void solve(struct TaskSpecification *task, struct AAF* aaf, struct Labeling* grounded) {

}

/* ============================================================================================================== */
int main(int argc, char *argv[]){
  // General solver information
	struct SolverInformation *info = taas__solverinformation(
			"taas-fargo v0.1 (2022-03-12)\nMatthias Thimm (matthias.thimm@fernuni-hagen.de)",
			"[tgf]",
			"[SE-GR,EE-GR,DC-GR,DS-GR,DC-CO,DC-PR]"
		);
  return taas__solve(argc,argv,info,solve);
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
