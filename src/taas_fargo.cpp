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
void sort_defeaters(const vector<int> & defeaters){
  //TODO
}
/* ============================================================================================================== */
int solve(taas::Problem problem, map<string,string>& params, taas::Af& af, taas::Labeling& lab, int argument = -1){
  // NOTE: lab contains initially the grounded labeling
  // NOTE: we only solve DC-CO and DC-PR here
  // stack that controls which arguments to be processed next
  stack<int> s;
  // add argument under consideration
  s.push(argument);
  // -- begin variables used in the main loop
  int arg, next_out, tmp;
  // arguments that must be turned OUT (bitset and pqueue representation
  // and link to predecessor)
  boost::dynamic_bitset<> must_out_bitset(af.get_number_of_arguments());
  priority_queue<int,vector<int>,taas::ArgumentCompare> must_out_queue(taas::ArgumentCompare(af,lab));
  vector<int> must_out_pred(af.get_number_of_arguments());
  // arguments that have already been unsuccessfully shown to be IN
  boost::dynamic_bitset<> not_in(af.get_number_of_arguments());
  // arguments that are selected by the key argument to be tried next
  // and thus go into not_in once tried and have to be removed when backtracking
  // beyond the key argument
  vector<stack<int>> not_in_pred(af.get_number_of_arguments());
  // -- end variables used in the main loop
  // main loop
  while(!s.empty()){
    // pop top argument
    arg = s.top();
    s.pop();
    /* ------
    lab.print_debug();
    bool is_first= true;
    stack<int> s_tmp(s);
    while(!s_tmp.empty()){
      int e = s_tmp.top();
      s_tmp.pop();
      if(is_first) is_first = false;
      else cout << ",";
      cout << af.get_argument_name(e) ;
    }
    cout << endl;
    cout << "popped: " << af.get_argument_name(arg) <<endl ;
    cout << endl;
    // ------ */

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
      // update not_in
      while(!not_in_pred[arg].empty()){
        tmp = not_in_pred[arg].top();
        not_in_pred[arg].pop();
        not_in.reset(tmp);
      }
      not_in.set(arg);
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
        // store information that the current argument does not work
        not_in.set(arg);
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
      // pick next argument from must_out
      // since must_out_queue is only lazily updated
      // we have to check the bitset; also remove all
      // arguments already shown to be OUT
      do{
        do{
          if(must_out_queue.empty()){
            // nothing left to turn out, so we are finished
            cout << "YES" << endl;
            return 0;
          }
          next_out = must_out_queue.top();
          must_out_queue.pop();
        }while(!must_out_bitset.test(next_out));
        must_out_bitset.reset(next_out);
      }while(lab.is_out(next_out));
      // sort parents of next_out
      sort_defeaters(af.get_attackers(next_out));
      //add this argument back to the stack (to indicate
      // when it should be backtracked) and then the attackers
      // of next_out (but only those that are not already impossible
      // to turn IN)
      s.push(arg);
      for(int a: af.get_attackers(next_out)){
        if(lab.is_out(a) || must_out_bitset.test(a) || not_in.test(a) || af.is_loop(a))
          continue;
        s.push(a);
        not_in_pred[arg].push(a);
      }
    }
  }
  // if we end up here, no admissible set could be found
  cout << "NO" << endl;
  return 0;
}
/* ============================================================================================================== */
int main(int argc, char *argv[]){
  taas::Solver solver(
    "taas-fargo v1.0.2 (2022-03-22)\nMatthias Thimm (matthias.thimm@fernuni-hagen.de)",
    {taas::Problem::DC_CO,taas::Problem::DC_PR},
    solve);
  return solver.execute(argc,argv);
}

/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
