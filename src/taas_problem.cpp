/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_problems.h
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#include "taas_problem.h"

#include <string>
#include <vector>

using namespace std;
/* ============================================================================================================== */
namespace taas{
/* ============================================================================================================== */
  taas::Problem string_to_problem(string problem){
    if(problem == "DS-GR")
      return DS_GR;
    if(problem == "DC-GR")
      return DC_GR;
    if(problem == "SE-GR")
      return SE_GR;
    if(problem == "EE-GR")
      return EE_GR;
    if(problem == "DS-CO")
      return DS_CO;
    if(problem == "DC-CO")
      return DC_CO;
    if(problem == "SE-CO")
      return SE_CO;
    if(problem == "EE-CO")
      return EE_CO;
    if(problem == "DS-PR")
      return DS_PR;
    if(problem == "DC-PR")
      return DC_PR;
    if(problem == "SE-PR")
      return SE_PR;
    if(problem == "EE-PR")
      return EE_PR;
    if(problem == "DS-ST")
      return DS_ST;
    if(problem == "DC-ST")
      return DC_ST;
    if(problem == "SE-ST")
      return SE_ST;
    //if(problem == "EE-ST")
    return EE_ST;
  }
/* ============================================================================================================== */
  string problem_to_string(taas::Problem problem){
    if(problem == DS_GR)
      return "DS-GR";
    if(problem == DC_GR)
      return "DC-GR";
    if(problem == SE_GR)
      return "SE-GR";
    if(problem == EE_GR)
      return "EE-GR";
    if(problem == DS_CO)
      return "DS-CO";
    if(problem == DC_CO)
      return "DC-CO";
    if(problem == SE_CO)
      return "SE-CO";
    if(problem == EE_CO)
      return "EE-CO";
    if(problem == DS_PR)
      return "DS-PR";
    if(problem == DC_PR)
      return "DC-PR";
    if(problem == SE_PR)
      return "SE-PR";
    if(problem == EE_PR)
      return "EE-PR";
    if(problem == DS_ST)
      return "DS-ST";
    if(problem == DC_ST)
      return "DC-ST";
    if(problem == SE_ST)
      return "SE-ST";
    //if(problem == EE_ST)
    return "EE-ST";
  }
/* ============================================================================================================== */
  string problems_to_string(vector<taas::Problem> problems){
    string s = "[";
    bool first = true;
    for( taas::Problem problem: problems){
      if(first)
          first = false;
      else
          s += ",";
      s += problem_to_string(problem);
    }
    s += "]";
    return s;
  }
/* ============================================================================================================== */
  bool is_decision_problem(Problem problem){
    switch(problem){
      case DS_GR:
      case DC_GR:
      case DS_CO:
      case DC_CO:
      case DS_PR:
      case DC_PR:
      case DS_ST:
      case DC_ST: return true;
      default   : return false;
    }
  }
/* ============================================================================================================== */
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
