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

//#define DEBUG true

#include "taas_af.h"
#include "taas_problem.h"
#include "taas_solver.h"
#include "taas_labeling.h"
#include "taas_compare.h"
#include "taas_util.h"
#include "taas_fast_pqueue.h"

#include <map>
#include <queue>
#include <stack>
#include <utility>

using namespace std;
/* ============================================================================================================== */
void sort_defeaters(const vector<int> & defeaters){
  //TODO
}
/* ============================================================================================================== */
int solve(taas::Problem problem, map<string,string>& params, taas::Af& af, taas::Labeling& lab, int argument = -1){
  #ifdef DEBUG
    cout << "Debugging on!" << endl;
    cout << "Problem:  " << taas::problem_to_string(problem) << endl;
    cout << "AF:       ";
    af.print();
    cout << "Argument: " << af.get_argument_name(argument) << endl << endl;
  #endif
  // NOTE: lab contains initially the grounded labeling
  // NOTE: we only solve DC-CO and DC-PR here
  // -- begin variables used in the main loop
  int next_out, tmp;
  int current_arg;
  taas::FastPriorityQueue * current_must_out;
  // arguments that must be turned OUT
  taas::FastPriorityQueue * must_out = new taas::FastPriorityQueue(af.get_number_of_arguments(),taas::ArgumentCompare(af,lab));
  taas::FastPriorityQueue * bck_must_out;
  // arguments that have already been unsuccessfully shown to be IN
  //boost::dynamic_bitset<> not_in(af.get_number_of_arguments());
  vector<bool> not_in(af.get_number_of_arguments());
  // arguments that are selected by the key argument to be tried next
  // and thus go into not_in once tried and have to be removed when backtracking
  // beyond the key argument
  vector<stack<int>> not_in_pred(af.get_number_of_arguments());
  // -- end variables used in the main loop
  // stacks that controls which arguments to be processed next
  stack<int> s_arg;
  stack<taas::FastPriorityQueue*> s_must_out;
  // add argument under consideration
  s_arg.push(argument);
  // main loop
  while(!s_arg.empty()){
    #ifdef DEBUG
      cout << "================================================================" << endl;
      cout << "Main loop begin" << endl;
      cout << "Stack s_arg:              ";
      print_arg_stack(s_arg,af);
      cout << endl;
      cout << "Stack s_must_out:         ";
      print_pqueue_stack(s_must_out,af);
      cout << endl;
      cout << "Labelling lab:            ";
      lab.print_debug();
      cout << "Set NOT IN:               ";
      //print_arg_bitset(not_in,af);
      cout << endl;
      cout << "Set MUST OUT:             ";
      must_out->print(af);
      cout << endl;
    #endif
    // pop top argument
    current_arg = s_arg.top();
    s_arg.pop();
    #ifdef DEBUG
      cout << "Selected from s_arg:      " << af.get_argument_name(current_arg) << endl;
    #endif
    // check whether we are searching or backtracking
    if(lab.is_in(current_arg)){
      current_must_out = s_must_out.top();
      s_must_out.pop();
      #ifdef DEBUG
        cout<< endl << "MODE:                     BACKTRACKING" << endl;
        cout << "Selected from s_must_out: ";
        current_must_out->print(af);
        cout << endl ;
      #endif
      // we are backtracking, so remove everything about arg
      // undo changes to lab
      lab.reset_in(current_arg);
      #ifdef DEBUG
        cout << "lab after reset:          ";
        lab.print_debug();
      #endif
      while(lab.rev_faf())
        ;
      #ifdef DEBUG
        cout << "lab after propa:          ";
        lab.print_debug();
      #endif
      // undo must_out information
      must_out = current_must_out;
      #ifdef DEBUG
        cout << "Update MUST OUT:          ";
        must_out->print(af);
        cout << endl;
      #endif
      //for(int a: af.get_attackers(arg_pair->first))
      //  if(must_out.get_pred(a) == arg_pair->first)
      //    must_out.remove(a);
      // update not_in
      while(!not_in_pred[current_arg].empty()){
        tmp = not_in_pred[current_arg].top();
        not_in_pred[current_arg].pop();
        not_in[tmp] = false; //.reset(tmp);
      }
      not_in[current_arg] = true;//.set(current_arg);
      #ifdef DEBUG
        cout << "Update NOT IN:            ";
        //print_arg_bitset(not_in,af);
        cout << endl;
      #endif
    }else{
      #ifdef DEBUG
        cout << endl << "MODE:                     SEARCHING" << endl;
      #endif
      // we are searching, so make arg IN in the current state
      lab.set_in(current_arg);
      #ifdef DEBUG
        cout << "lab after set:            ";
        lab.print_debug();
      #endif
      // propagate information
      while(lab.faf())
        ;
      #ifdef DEBUG
        cout << "lab after propa:          ";
        lab.print_debug();
      #endif
      // check if we have a conflict
      if(lab.has_conflict()){
        // undo changes to lab
        lab.reset_in(current_arg);
        #ifdef DEBUG
          cout << endl;
          cout << "Conflict detected" << endl;
          cout << "lab after reset:          ";
          lab.print_debug();
        #endif
        while(lab.rev_faf())
          ;
        #ifdef DEBUG
          cout << "lab after propa:          ";
          lab.print_debug();
        #endif
        // store information that the current argument does not work
        not_in[current_arg] = true; //.set(current_arg);
        #ifdef DEBUG
          cout << "Update NOT IN:            ";
          //print_arg_bitset(not_in,af);
          cout << endl;
        #endif
        // continue processing
        continue;
      }
      // backup must_out
      bck_must_out = new taas::FastPriorityQueue(*must_out);
      // check on attackers of arg
      bool loop_found = false;
      for(int a: af.get_attackers(current_arg))
        if(!lab.is_out(a) && !must_out->contains(a)){
          // special case: a is loop and not attacked otherwise
          if(af.is_loop(a) && af.get_attackers(a).size() == 1)
            loop_found = true;
          must_out->add(a,current_arg);
        }
      #ifdef DEBUG
        cout << "Update MUST OUT:          ";
        must_out->print(af);
        cout << endl;
      #endif
      if(loop_found){
          // undo changes to lab
          lab.reset_in(current_arg);
          #ifdef DEBUG
            cout << endl;
            cout << "Loop detected" << endl;
            cout << "lab after reset:          ";
            lab.print_debug();
          #endif
          while(lab.rev_faf())
            ;
          #ifdef DEBUG
            cout << "lab after propa:          ";
            lab.print_debug();
          #endif
          // store information that the current argument does not work
          not_in[current_arg] = true; //.set(current_arg);
          #ifdef DEBUG
            cout << "Update NOT IN:            ";
            //print_arg_bitset(not_in,af);
            cout << endl;
          #endif
          // redact must_out updates
          for(int a: af.get_attackers(current_arg))
            if(must_out->contains(a) && must_out->get_pred(a) == current_arg){
              must_out->remove(a);
            }
          #ifdef DEBUG
            cout << "Update MUST OUT:          ";
            must_out->print(af);
            cout << endl;
          #endif
          // continue processing
          continue;
      }
      // pick next argument from must_out
      // (also remove all arguments already shown to be OUT)
      do{
        // check whether we are finished
        if(must_out->empty()){
          cout << "YES" << endl;
          return 0;
        }
        next_out = must_out->top_and_pop();
      }while(lab.is_out(next_out));
      #ifdef DEBUG
        cout << endl;
        cout << "Next OUT selected:        " << af.get_argument_name(next_out) << endl;
        cout << "Update MUST OUT:          ";
        must_out->print(af);
        cout << endl;
      #endif
      // sort parents of next_out
      sort_defeaters(af.get_attackers(next_out));
      //add this argument back to the stack (to indicate
      // when it should be backtracked) and then the attackers
      // of next_out (but only those that are not already impossible
      // to turn IN)
      s_arg.push(current_arg);
      s_must_out.push(bck_must_out);
      for(int a: af.get_attackers(next_out)){
        if(lab.is_out(a) || must_out->contains(a) || not_in[a] || af.is_loop(a))
          continue;
        s_arg.push(a);
        not_in_pred[current_arg].push(a);
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
    "taas-fargo v1.0.4 (2022-03-25)\nMatthias Thimm (matthias.thimm@fernuni-hagen.de)",
    {taas::Problem::DC_CO,taas::Problem::DC_PR},
    solve);
  return solver.execute(argc,argv);
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
