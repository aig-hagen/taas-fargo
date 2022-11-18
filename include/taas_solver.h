/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_solver.h
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#include "taas_problem.h"
#include "taas_labeling.h"

#include <string>
#include <vector>
#include <iostream>
#include <map>

using namespace std;
/* ============================================================================================================== */
namespace taas{

  class Solver{
      private:
        string version_info;
        vector<taas::Problem> supported_problems;
        int(*solve_function)(taas::Problem,map<string,string>&,taas::Af&,taas::Labeling&,int);
        taas::Af parse_tgf(ifstream& file);
        taas::Af parse_apx(ifstream& file);
        taas::Af parse_i23(ifstream& file);
      public:
        Solver(string version_info, vector<taas::Problem> supported_problems, int (*solve_function)(taas::Problem,map<string,string>&,taas::Af&,taas::Labeling&,int));
        int execute(int argc, char *argv[]);
  };

}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
