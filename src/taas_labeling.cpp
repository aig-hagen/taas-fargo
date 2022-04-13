/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_labeling.cpp
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#include "taas_labeling.h"
#include "taas_af.h"

#include <iostream>
#include <queue>

using namespace std;

/* ============================================================================================================== */
namespace taas{
/* ============================================================================================================== */
  /*
   * constructor
   */
  taas::Labeling::Labeling(taas::Af& af){
    this->decision_id = 0;
    this->in = vector<bool>(af.get_number_of_arguments());
    this->out = vector<bool>(af.get_number_of_arguments());
    this->number_of_non_out_attackers = vector<int>(af.get_number_of_attackers());
    this->af = &af;
    this->conflicts = vector<bool>(af.get_number_of_arguments());
    this->num_conflicts = 0;
    this->decision_level = vector<int>(af.get_number_of_arguments());
    this->unattacked_and_not_in_arguments = vector<int>();
    this->not_unattacked_and_in_arguments = vector<int>();
    for( int i = 0; i < this->af->get_number_of_arguments(); i++ ){
      if(this->number_of_non_out_attackers[i] == 0)
        this->unattacked_and_not_in_arguments.push_back(i);
    }
  }
/* ============================================================================================================== */
/* ============================================================================================================== */
  /*
   * set in
   */
   void taas::Labeling::set_in(int arg, bool inc_decision_id){
     if(this->in[arg])
      return;
     this->in[arg] = true;
     if(inc_decision_id)
      this->decision_id++;
     this->decision_level[arg] = this->decision_id;
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++)
      this->set_out(this->af->get_attacked(arg)[i]);
   }
/* ============================================================================================================== */
  /*
   * conflict analysis
   */
  int taas::Labeling::conflict_analysis(int arg){
    queue<int> q;
    vector<bool> visited(this->af->get_number_of_arguments());
    q.push(arg);
    int largest_dl = this->decision_level[arg];
    int second_largest_dl = this->decision_level[arg];
    while(!q.empty()){
      if(this->decision_level[q.front()] > largest_dl){
        second_largest_dl = largest_dl;
        largest_dl = this->decision_level[q.front()];
      }else if(this->decision_level[q.front()] < largest_dl && this->decision_level[q.front()] > second_largest_dl){
        second_largest_dl = this->decision_level[q.front()];
      }
      for(int a: this->af->get_attackers(q.front()))
        if(!visited[a] && this->decision_level[a] != 0){
          q.push(a);
          visited[a] = true;
        }
      q.pop();
    }
    return second_largest_dl;
  }
/* ============================================================================================================== */
  /*
   * set out
   */
   void taas::Labeling::set_out(int arg){
     if(this->out[arg])
      return;
     this->out[arg] = true;
     // check for conflict
     if(this->in[arg]){
       if(!this->conflicts[arg]){
         this->conflicts[arg];
         this->num_conflicts++;
         // TODO:
        // if(this->conflict_analysis(arg) < this->decision_id)
        //  cout << this->decision_id-this->conflict_analysis(arg) << "  "<<this->conflict_analysis(arg) << " " << this->decision_id << endl;
         // do conflict analysis, simply by BFS in ancestor direction
         // and determine the second to largest decision level, use
         // that value to learn the conflict
       }
       return;
     }
     this->decision_level[arg] = this->decision_id;
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++){
      this->number_of_non_out_attackers[this->af->get_attacked(arg)[i]]--;
      if(this->number_of_non_out_attackers[this->af->get_attacked(arg)[i]] == 0 && !this->in[this->af->get_attacked(arg)[i]])
        this->unattacked_and_not_in_arguments.push_back(this->af->get_attacked(arg)[i]);
     }
   }
/* ============================================================================================================== */
  /*
   * reset in
   */
   void taas::Labeling::reset_in(int arg, bool dec_decision_id){
     if(!this->in[arg])
      return;
     this->in[arg] = false;
      if(dec_decision_id){
       this->decision_id--;
       //cout << ++num_c << endl;
     }
     // check for conflict
     if(this->out[arg]){
       this->conflicts[arg] = false;
       this->num_conflicts--;
     }
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++)
      this->undo_attack(this->af->get_attacked(arg)[i], this->decision_level[arg]);
     //this->decision_level[arg] = 0;
   }
/* ============================================================================================================== */
  /*
   * An attacker of arg has been unset from IN,
   * check whether this has implications and propagate further
   */
   void taas::Labeling::undo_attack(int arg, int decision_id){
     if(!this->out[arg])
      return;
     if(this->decision_level[arg] != decision_id && !this->in[arg])
      return;
     this->out[arg] = false;
     // check for conflict
     if(this->in[arg]){
       this->conflicts[arg] = false;
       this->num_conflicts--;
     }
     if(this->decision_level[arg] == decision_id){
       for(int i = 0; i < this->af->get_attacked(arg).size(); i++){
        this->number_of_non_out_attackers[this->af->get_attacked(arg)[i]]++;
        if(this->in[this->af->get_attacked(arg)[i]] && this->decision_level[this->af->get_attacked(arg)[i]] == decision_id)
         this->not_unattacked_and_in_arguments.push_back(this->af->get_attacked(arg)[i]);
       }
       //this->decision_level[arg] = 0;
     }
   }
/* ============================================================================================================== */
  /*
   * is in?
   */
   bool taas::Labeling::is_in(int arg){
     return this->in[arg];
   }
/* ============================================================================================================== */
  /*
   * is out?
   */
   bool taas::Labeling::is_out(int arg){
     return this->out[arg];
   }
/* ============================================================================================================== */
  /*
   * is unset?
   */
   bool taas::Labeling::is_reset(int arg){
     return this->in[arg] || this->out[arg];
   }
/* ============================================================================================================== */
  /*
   * adds one unattacked arguments to IN and updates
   * OUT, number_of_non_out_attackers, and unattacked_and_not_in_arguments
   * return "true" if something changed
   */
   bool taas::Labeling::faf(){
     if(this->unattacked_and_not_in_arguments.empty())
      return false;
     int arg = this->unattacked_and_not_in_arguments.back();
     this->unattacked_and_not_in_arguments.pop_back();
     this->set_in(arg,false);
     return true;
   }
/* ============================================================================================================== */
  /*
   * the reverse of faf, used to undo changes made by faf
   * return "true" if something changed
   */
   bool taas::Labeling::rev_faf(){
     if(this->not_unattacked_and_in_arguments.empty())
      return false;
     int arg = this->not_unattacked_and_in_arguments.back();
     this->not_unattacked_and_in_arguments.pop_back();
     this->reset_in(arg,false);
     return true;
   }
/* ============================================================================================================== */
  /*
   * whether we have detected some conflict during
   * propagation
   */
   bool taas::Labeling::has_conflict(){
     return this->num_conflicts > 0;
   }
/* ============================================================================================================== */
  /*
   * get the decision level of the given argument
   */
   int taas::Labeling::get_decision_level(int arg){
     if(arg < 0 || arg >= this->decision_level.size())
      return 0;
     return this->decision_level[arg];
   }
/* ============================================================================================================== */
  /*
   * the number of non-out parents of arg
   */
  int taas::Labeling::get_number_of_non_out_attackers(int arg){
    return this->number_of_non_out_attackers[arg];
  }
/* ============================================================================================================== */
  /*
   * print for debugging
   */
   void taas::Labeling::print_debug(){
     cout << "IN={";
     bool is_first = true;
     for( int i  = 0; i < this->af->get_number_of_arguments() ; i++ )
      if(this->in[i]){
        if(is_first)
          is_first = false;
        else
          cout << ",";
        cout << this->af->get_argument_name(i);
      }
     cout << "}, OUT={";
     is_first = true;
     for( int i  = 0; i < this->af->get_number_of_arguments() ; i++ )
      if(this->out[i]){
       if(is_first)
           is_first = false;
         else
           cout << ",";
         cout << this->af->get_argument_name(i);
      }
     cout << "}, UNATT={";
     is_first = true;
     for( int i  = 0; i < this->unattacked_and_not_in_arguments.size() ; i++ ){
      if(is_first)
       is_first = false;
      else
       cout << ",";
      cout << this->af->get_argument_name(this->unattacked_and_not_in_arguments[i]);
     }
     cout << "}, NUNATT={";
     is_first = true;
     for( int i  = 0; i < this->not_unattacked_and_in_arguments.size() ; i++ ){
      if(is_first)
       is_first = false;
      else
       cout << ",";
      cout << this->af->get_argument_name(this->not_unattacked_and_in_arguments[i]);
     }
     cout << "}, #ATT={";
     is_first = true;
     for( int i  = 0; i < this->number_of_non_out_attackers.size() ; i++ ){
      if(is_first)
       is_first = false;
      else
       cout << ",";
      cout << this->af->get_argument_name(i) << "=" << this->number_of_non_out_attackers[i];
     }
     cout << "}, DLEV={";
     is_first = true;
     for( int i  = 0; i < this->af->get_number_of_arguments() ; i++ ){
      if(this->decision_level[i] == 0 )
        continue;
      if(is_first)
       is_first = false;
      else
       cout << ",";
      cout << this->af->get_argument_name(i) << "=" << this->decision_level[i];
     }
     /*cout << "}, VATT={";
     is_first = true;
     for(int arg1 = 0; arg1 < this->af->get_number_of_arguments() ; arg1++ ){
       for(int arg2 = 0; arg2 < this->visited_attacks[arg1].size(); arg2++){
         if(this->visited_attacks[arg1][arg2] > 0){
           if(is_first)
             is_first = false;
           else cout << ",";
           cout << "(" << this->af->get_argument_name(arg1) << "," << this->af->get_argument_name(this->af->get_attacked(arg1)[arg2]) << ")=" << this->visited_attacks[arg1][arg2];
         }
       }
     }*/
     cout << "}, CONFLICTS={";
     is_first = true;
     for( int i  = 0; i < this->af->get_number_of_arguments() ; i++ ){
       if(this->conflicts[i]){
        if(is_first)
         is_first = false;
        else
         cout << ",";
        cout << this->af->get_argument_name(i);
      }
     }
     cout << "}, NUM_CONFLICTS=" << this->num_conflicts << endl;
   }
/* ============================================================================================================== */
  /*
   * print
   */
   void taas::Labeling::print(){
     cout << "[";
     bool is_first = true;
     for( int i  = 0; i < this->af->get_number_of_arguments() ; i++ )
      if(this->in[i]){
        if(is_first)
          is_first = false;
        else
          cout << ",";
        cout << this->af->get_argument_name(i);
      }
     cout << "]";
   }
/* ============================================================================================================== */
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
