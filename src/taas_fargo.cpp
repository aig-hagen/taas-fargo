/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas-fargo.cpp
 Author      : Matthias Thimm
 Version     : 1.0.7
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
#include <algorithm>

using namespace std;
/* ============================================================================================================== */
// main solving method: checks whether the given argument is contained in an
// admissible set
int is_acceptable(taas::Af& af, taas::Labeling& lab, int argument,taas::ArgumentCompare defeater_compare,taas::ArgumentCompare must_out_compare,int limit){
  // -- begin variables used in the main loop
  int next_out, tmp;
  int current_arg;
  // arguments that must be turned OUT
  taas::FastPriorityQueue must_out = taas::FastPriorityQueue(af.get_number_of_arguments(),must_out_compare);
  must_out_compare.set_must_out(must_out);
  // arguments that have already been unsuccessfully shown to be IN
  vector<bool> not_in(af.get_number_of_arguments());
  // arguments that are selected by the key argument to be tried next
  // and thus go into not_in once tried and have to be removed when backtracking
  // beyond the key argument
  vector<stack<int>> not_in_pred(af.get_number_of_arguments());
  // -- end variables used in the main loop
  // stacks that controls which arguments to be processed next
  stack<int> s_arg;
  stack<vector<int>> s_must_out_add;
  stack<vector<int>> s_must_out_add_pred;
  stack<vector<int>> s_must_out_del;
  // add argument under consideration
  s_arg.push(argument);
  // main loop
  int iteration = 0;
  while(!s_arg.empty() && (limit == -1 || iteration < limit)){
    iteration++;
    #ifdef DEBUG
      cout << "================================================================" << endl;
      cout << "Main loop begin" << endl;
      cout << "Stack s_arg:              ";
      print_arg_stack(s_arg,af);
      cout << endl;
      cout << "Stack s_must_out_add:     ";
      print_arglist_stack(s_must_out_add,af);
      cout << endl;
      cout << "Stack s_must_out_add_pred:";
      print_arglist_stack(s_must_out_add_pred,af);
      cout << endl;
      cout << "Stack s_must_out_del:     ";
      print_arglist_stack(s_must_out_del,af);
      cout << endl;
      cout << "Labelling lab:            ";
      lab.print_debug();
      cout << "Set NOT IN:               ";
      print_arg_bitset(not_in,af);
      cout << endl;
      cout << "Set MUST OUT:             ";
      must_out.print(af);
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
      #ifdef DEBUG
        cout<< endl << "MODE:                     BACKTRACKING" << endl;
        //cout << "Selected from s_must_out: ";
        //current_must_out.print(af);
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
      for(int i = 0; i < s_must_out_add.top().size(); i++)
        must_out.add(s_must_out_add.top()[i],s_must_out_add_pred.top()[i]);
      for(int a: s_must_out_del.top())
        must_out.remove(a);
      s_must_out_del.pop();
      s_must_out_add.pop();
      s_must_out_add_pred.pop();
      #ifdef DEBUG
        cout << "Update MUST OUT:          ";
        must_out.print(af);
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
        print_arg_bitset(not_in,af);
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
          print_arg_bitset(not_in,af);
          cout << endl;
        #endif
        // continue processing
        continue;
      }
      // backup must_out
      vector<int> must_out_del;
      vector<int> must_out_add;
      vector<int> must_out_add_pred;
      // check on attackers of arg
      bool loop_found = false;
      for(int a: af.get_attackers(current_arg))
        if(!lab.is_out(a) && !must_out.contains(a)){
          // special case: a is loop and not attacked otherwise
          if(af.is_loop(a) && af.get_attackers(a).size() == 1)
            loop_found = true;
          must_out.add(a,current_arg);
          must_out_del.push_back(a);
        }
      #ifdef DEBUG
        cout << "Update MUST OUT:          ";
        must_out.print(af);
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
            if(must_out.contains(a) && must_out.get_pred(a) == current_arg){
              must_out.remove(a);
            }
          #ifdef DEBUG
            cout << "Update MUST OUT:          ";
            must_out.print(af);
            cout << endl;
          #endif
          // continue processing
          continue;
      }
      // pick next argument from must_out
      // (also remove all arguments already shown to be OUT)
      do{
        // check whether we are finished
        if(must_out.empty()){
          return 1;
        }
        next_out = must_out.top_and_pop();
        must_out_add.push_back(next_out);
        must_out_add_pred.push_back(must_out.get_pred(next_out));
      }while(lab.is_out(next_out));
      #ifdef DEBUG
        cout << endl;
        cout << "Next OUT selected:        " << af.get_argument_name(next_out) << endl;
        cout << "Update MUST OUT:          ";
        must_out.print(af);
        cout << endl;
      #endif
      // sort parents of next_out
      sort(af.get_attackers(next_out).begin(),af.get_attackers(next_out).end(),defeater_compare);
      //add this argument back to the stack (to indicate
      // when it should be backtracked) and then the attackers
      // of next_out (but only those that are not already impossible
      // to turn IN)
      s_arg.push(current_arg);
      s_must_out_add.push(must_out_add);
      s_must_out_add_pred.push(must_out_add_pred);
      s_must_out_del.push(must_out_del);
      for(int a: af.get_attackers(next_out)){
        if(lab.is_out(a) || must_out.contains(a) || not_in[a] || af.is_loop(a))
          continue;
        s_arg.push(a);
        not_in_pred[current_arg].push(a);
      }
    }
  }
  // if we end up here, no admissible set could be found
  return 0;
}
/* ============================================================================================================== */
int solve(taas::Problem problem, map<string,string>& params, taas::Af& af, taas::Labeling& lab, int argument = -1){
  // NOTE: lab contains initially the grounded labeling
  // NOTE: we only solve DC-CO and DC-PR here
  #ifdef DEBUG
    cout << "Debugging on!" << endl;
    cout << "Problem:  " << taas::problem_to_string(problem) << endl;
    cout << "AF:       ";
    af.print();
    cout << "Argument: " << af.get_argument_name(argument) << endl << endl;
  #endif
  // parse parameters
  taas::SortOrder order_defeaters;
  taas::SortOrder order_must_outs;
  taas::SortByDecisionLevel dl_order;
  if(params.find("-sd") != params.end()){
    if(params["-sd"] == "asc_in")
      order_defeaters = taas::ASC_IN_DEGREE;
    else if(params["-sd"] == "desc_in")
      order_defeaters = taas::DESC_IN_DEGREE;
    else if(params["-sd"] == "asc_out")
      order_defeaters = taas::ASC_OUT_DEGREE;
    else if(params["-sd"] == "desc_out")
      order_defeaters = taas::DESC_OUT_DEGREE;
    else order_defeaters = taas::NUMERICAL;
  }else order_defeaters = taas::DESC_IN_DEGREE;
  if(params.find("-sm") != params.end()){
    if(params["-sm"] == "asc_in")
      order_must_outs = taas::ASC_IN_DEGREE;
    else if(params["-sm"] == "desc_in")
      order_must_outs = taas::DESC_IN_DEGREE;
    else if(params["-sm"] == "asc_out")
      order_must_outs = taas::ASC_OUT_DEGREE;
    else if(params["-sm"] == "desc_out")
      order_must_outs = taas::DESC_OUT_DEGREE;
    else order_must_outs = taas::NUMERICAL;
  }else order_must_outs = taas::DESC_IN_DEGREE;
  if(params.find("-sdl") != params.end()){
    if(params["-sdl"] == "asc")
      dl_order = taas::ASC;
    else if(params["-sdl"] == "desc")
      dl_order = taas::DESC;
    else dl_order = taas::OFF;
  }else dl_order = taas::OFF;
  taas::ArgumentCompare defeater_compare(af,lab,order_defeaters,taas::OFF);
  taas::ArgumentCompare must_out_compare(af,lab,order_must_outs,dl_order);
  // for approximate reasoning: the maximum number of iterations of the main loop
  int limit = -1;
  if(params.find("-limit") != params.end()){
    limit = std::stoi(params["-limit"]) * af.get_number_of_arguments();
  }
  // support of tasks other than taas::Problem::DC_CO,taas::Problem::DC_PR is
  // only for the approximate version
  if( limit == -1 && (problem != taas::Problem::DC_CO && problem != taas::Problem::DC_PR)){
      cout << "The problem " << taas::problem_to_string(problem) << " is only supported by the approximate version of this solver (flag '-limit' must be set to value other than -1)" << endl;
      exit(0);
  }
  // for the approximate version and decision problems with stable semantics:
  // if query argument is in the grounded extension (or attacked by it)
  // then answer is automatically Yes (No) (no further existence checking is done)
  if(problem == taas::Problem::DC_ST || problem == taas::Problem::DS_ST){
    if(lab.is_in(argument)){
      cout << "YES" << endl;
      return 0;
    }else if(lab.is_out(argument)){
      cout << "NO" << endl;
      return 0;
    }
  }
  if(problem == taas::Problem::DC_CO || problem == taas::Problem::DC_PR || problem == taas::Problem::DC_ST || problem == taas::Problem::DC_SST || problem == taas::Problem::DC_STG){
    // for credulous reasoning other than ID, check whether the argument is contained
    // in an admissible set
    if(is_acceptable(af,lab,argument,defeater_compare,must_out_compare,limit))
        cout << "YES" << endl;
      else
        cout << "NO" << endl;
  }else{
    // for skeptical reasoning and DC-ID, check in addition whether any attacker
    // of the argument is contained in an admissible set
    // copy lab
    taas::Labeling lab_copy = taas::Labeling(lab);
    if(!is_acceptable(af,lab_copy,argument,defeater_compare,must_out_compare,limit))
      cout << "NO" << endl;
    else{
      for(int a: af.get_attackers(argument)){
        lab_copy = taas::Labeling(lab);
        if(is_acceptable(af,lab_copy,a,defeater_compare,must_out_compare,limit)){
          cout << "NO" << endl;
          return 0;
        }
      }
      cout << "YES" << endl;
    }
  }
  return 0;
}
/* ============================================================================================================== */
int main(int argc, char *argv[]){
  // only taas::Problem::DC_CO,taas::Problem::DC_PR are supported by the non-approximate version
  taas::Solver solver(
    "taas-fargo v1.1.2 (2023-06-21)\nMatthias Thimm (matthias.thimm@fernuni-hagen.de)",
    {taas::Problem::DC_CO,taas::Problem::DC_PR,taas::Problem::DC_ST,taas::Problem::DC_SST,taas::Problem::DC_STG,taas::Problem::DC_ID,taas::Problem::DS_CO,taas::Problem::DS_PR,taas::Problem::DS_ST,taas::Problem::DS_SST,taas::Problem::DS_STG,taas::Problem::DS_ID,taas::Problem::DS_GR,taas::Problem::DC_GR},

    solve);
  return solver.execute(argc,argv);
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
