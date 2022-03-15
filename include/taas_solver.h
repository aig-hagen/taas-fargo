/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_solver.h
 Author      : Matthias Thimm
 Version     : 0.1
 Copyright   : GPL3
============================================================================
*/
#include "taas_problem.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;
/* ============================================================================================================== */
namespace taas{

  class Solver{
      private:
        string version_info;
        vector<taas::Problem> supported_problems;
        int(*solve_function)(taas::Problem,taas::Af,int);

        taas::Af parse_tgf(ifstream& file);
        taas::Af parse_apx(ifstream& file);
      protected:

      public:
        Solver(string version_info, vector<taas::Problem> supported_problems, int (*solve_function)(taas::Problem,taas::Af,int));
        int execute(int argc, char *argv[]);
  };

}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
