/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas-fargo.cpp
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
 Description : The taas-fargo solver for abstract argumentation.

============================================================================
*/
#include "taas_af.h"
#include "taas_problem.h"
#include "taas_solver.h"
#include "taas_labeling.h"
#include "taas_compare.h"

#include <boost/dynamic_bitset.hpp>
#include <map>
#include <queue>
#include <stack>

using namespace std;
/* ============================================================================================================== */
int solve(taas::Problem problem, map<string,string>& params, taas::Af& af, taas::Labeling& lab, int argument = -1){
  // NOTE: lab contains initially the grounded labeling
  // NOTE: we only solve DC-CO and DC-PR here
  // stack that controls which arguments to be processed next
  stack<int> s;
  // add argument under consideration
  s.push(argument);
  // -- begin variables used in the main loop
  int arg;
  // arguments that must be turned OUT (bitset and pqueue representation)
  boost::dynamic_bitset<> must_out_bitset(af.get_number_of_arguments());
  priority_queue<int,vector<int>,taas::ArgumentCompare> must_out_queue(taas::ArgumentCompare(af,lab));
  vector<int> must_out_pred(af.get_number_of_arguments());
  // arguments that have already been unsuccessfully shown to be IN
  boost::dynamic_bitset<> not_in(af.get_number_of_arguments());
  // -- end variables used in the main loop
  // main loop
  while(s.empty()){
    // pop top argument
    arg = s.top();
    s.pop();
    // check whether we are searching or backtracking
    if(lab.is_in(arg)){
      // we are backtracking, so remove everything about arg
      // undo changes to lab
      lab.reset_in(arg);
      while(lab.rev_faf())
        ;
      // undo must_out information
      for(int a: af.get_attackers(arg))
        if(must_out_pred[a] == arg){
          must_out_bitset.reset(a);
          // do no explicitly remove from must_out_queue,
          // this is done lazily by checking must_out_bitset when needed
        }
      // TODO continue
    }else{
      // we are searching, so make arg IN in the current state
      lab.set_in(arg);
      // propagate information
      while(lab.faf())
        ;
      // check if we have a conflict
      if(lab.has_conflict()){
        // undo changes to lab
        lab.reset_in(arg);
        while(lab.rev_faf())
          ;
        // continue processing
        continue;
      }
      // check on attackers of arg
      for(int a: af.get_attackers(arg))
        if(!lab.is_out(a) && !must_out_bitset.test(a)){
          must_out_bitset.set(a);
          must_out_queue.push(a);
          must_out_pred[a] = arg;
        }
      //TODO continue
    }
  }
  // if we end up here, no admissible set could be found
  cout << "NO" << endl;
  return 0;
}
/* ============================================================================================================== */
int main(int argc, char *argv[]){
  taas::Solver solver(
    "taas-fargo v1.0 (2022-03-15)\nMatthias Thimm (matthias.thimm@fernuni-hagen.de)",
    {taas::Problem::DC_CO,taas::Problem::DC_PR},
    solve);
  return solver.execute(argc,argv);
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
