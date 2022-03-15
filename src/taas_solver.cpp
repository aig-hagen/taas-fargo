/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_solver.cpp
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#include "taas_af.h"
#include "taas_problem.h"
#include "taas_solver.h"
#include "taas_labeling.h"
#include <boost/algorithm/string/trim.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
/* ============================================================================================================== */
namespace taas{
/* ============================================================================================================== */
  /*
   * constructor
   */
  taas::Solver::Solver(string version_info, vector<taas::Problem> supported_problems, int(*solve_function)(taas::Problem,taas::Af&,taas::Labeling&,int)){
    this->version_info = version_info;
    this->supported_problems = supported_problems;
    // add standard problems
    this->supported_problems.push_back(taas::Problem::DS_GR);
    this->supported_problems.push_back(taas::Problem::DC_GR);
    this->supported_problems.push_back(taas::Problem::SE_GR);
    this->supported_problems.push_back(taas::Problem::EE_GR);
    this->supported_problems.push_back(taas::Problem::SE_CO);
    this->supported_problems.push_back(taas::Problem::DS_CO);
    this->solve_function = solve_function;
  }
/* ============================================================================================================== */
  /*
   * execute: handles command line IO
   */
  int taas::Solver::execute(int argc, char *argv[]){
    // print version information
    if (argc == 1) {
		  cout << this->version_info << endl;
		  return 0;
	  }
    // parse parameters
    //char *file_name = NULL, *file_format = NULL, *argument = NULL;
    string file_name = "", file_format = "", argument = "";
    taas::Problem problem;
    bool problem_specified = false;
    for(int i = 1; i < argc; i++){
      if(strcmp(argv[i],"-p") == 0){
          problem = taas::string_to_problem(argv[++i]);
          problem_specified = true;
          continue;
      }
      if(strcmp(argv[i],"-f") == 0){
         file_name = argv[++i];
         continue;
      }
      if(strcmp(argv[i],"-a") == 0){
         argument = argv[++i];
        continue;
      }
      if(strcmp(argv[i],"-fo") == 0){
         file_format = argv[++i];
        continue;
      }
      // for the parameter "--formats" print out the formats and exit
      if(strcmp(argv[i],"--formats") == 0){
        cout << "[tgf,apx]" << endl;
        return 0;
      }
      // for the parameter "--problems" print out the problems and exit
      if(strcmp(argv[i],"--problems") == 0){
        cout << taas::problems_to_string(this->supported_problems) << endl;
        return 0;
      }
    }
    // check for issues with the parameters
    if( !problem_specified ){
      cout << "Problem specification is missing (parameter '-p')" << endl;
      return 1;
    }
    if( file_name == "" ){
      cout << "File specification is missing (parameter '-f')" << endl;
      return 1;
    }
    if( file_format == "" ){
      cout << "File format specification is missing (parameter '-fo')" << endl;
      return 1;
    }
    if(find(this->supported_problems.begin(), this->supported_problems.end(), problem) == this->supported_problems.end()){
      cout << "Problem not supported by this solver" << endl;
      return 1;
    }
    if(file_format != "apx" && file_format != "tgf"){
      cout << "File format not supported by this solver" << endl;
      return 1;
    }
    ifstream file;
	  file.open(file_name);
	  if (!file.good()) {
		    cout << "File could not be opened." << endl;
		    return 1;
	  }
    taas::Af af;
    if( file_format == "tgf" )
      af = parse_tgf(file);
    else
      af = parse_apx(file);
    file.close();
    int arg_idx = -1;
    if(taas::is_decision_problem(problem))
      if(argument == ""){
        cout << "The problem requires the specification of a query argument (using '-a')" << endl;
        return 1;
      }else{
        arg_idx = af.get_argument_index(argument);
      }
    // check some trivial special cases
    // --------------------------------
	  // for every semantics, any argument in a loop is neither credulously nor
	  // skeptically accepted (except for skeptical reasoning with stable semantics
	  // (if there is no stable extension))
    if(taas::is_decision_problem(problem) && af.is_loop(arg_idx) && problem != DS_ST){
      cout << "NO" << endl;
      return 0;
    }
    // "af->initial" already contains all initial arguments
	  // check some simple special cases for that
    if(!af.has_initial_arguments()){
      // if there are no initial arguments, the grounded extension is empty;
		  // furthermore the grounded extension is also a complete extension
		  if(problem == SE_GR || problem == SE_CO){
        cout << "[]" << endl;
        return 0;
			}
		  if(problem == EE_GR){
        cout << "[[]]" << endl;
        return 0;
		  }
		  // no argument is accepted (cred.+skept) under grounded semantics
		  // and skeptically accepted wrt. complete semantics
		  if(problem == DC_GR || problem == DS_GR || problem == DS_CO){
  			cout << "NO" << endl;
        return 0;
		  }
    }else{
      // for every semantics, if an argument is initial
		  // it is both credulously and skeptically accepted
		  // (except for credulous reasoning with stable semantics
		  // (if there is no stable extension))
		  if(taas::is_decision_problem(problem) && problem != DC_ST && af.is_initial(arg_idx)){
			  cout << "YES" << endl;
				return 0;
			}
    }
    // compute grounded
    taas::Labeling grounded(af);
    while(grounded.faf())
      ;
    // solve problems regarding grounded semantics
    if( problem == DS_GR || problem == DC_GR){
      if(grounded.is_in(arg_idx))
        cout << "YES" << endl;
      else cout << "NO" << endl;
      return 0;
    }
    if( problem == SE_GR || problem == SE_CO ){
      grounded.print();
      cout << endl;
      return 0;
    }
    if( problem == EE_GR ){
      cout << "[";
      grounded.print();
      cout << "]" << endl;
      return 0;
    }
    // if we have a decision problem and the query argument
    // is IN or OUT in the grounded labeling, we can also answer
    // (except for stable semantics)
    if(taas::is_decision_problem(problem) && problem != DC_ST && problem != DS_ST)
      if(grounded.is_in(arg_idx)){
        cout << "YES" << endl;
        return 0;
      }else if(grounded.is_out(arg_idx)){
        cout << "NO" << endl;
        return 0;
      }
    // now solve the remaining problems
    this->solve_function(problem,af,grounded,arg_idx);
    return 0;
  }
/* ============================================================================================================== */
  /*
   * parse_tgf: parse TGF file into an AF
   */
  taas::Af taas::Solver::parse_tgf(ifstream& file){
    taas::Af af;
    string argument, attacker, attacked;
    while (file >> argument) {
			if (argument == "#") break;
			af.add_argument(argument);
		}
		while (file >> attacker >> attacked) {
			af.add_attack(attacker, attacked);
		}
    return af;
  }
/* ============================================================================================================== */
  /*
   * parse_apx: parse APX file into an AF
   */
  taas::Af taas::Solver::parse_apx(ifstream& file){
    taas::Af af;
    string line;
    string argument, attacker, attacked;
    while (getline(file, line)) {
      boost::algorithm::trim(line);
      if(line.rfind("arg", 0) == 0) {
        argument = line.substr(4,line.length()-6);
        boost::algorithm::trim(argument);
        af.add_argument(argument);
      }else if(line.rfind("att", 0) == 0){
        attacker = line.substr(4,line.find(",",0)-4);
        attacked = line.substr(line.find(",",0)+1,line.length()-3-line.find(",",0));
        boost::algorithm::trim(attacker);
        boost::algorithm::trim(attacked);
        af.add_attack(attacker,attacked);
      }else {
        cerr << "Cannot parse line '" << line << "' in file, ignoring";
      }
    }
    return af;
  }
/* ============================================================================================================== */
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
