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
#include <boost/dynamic_bitset.hpp>

using namespace std;

/* ============================================================================================================== */
namespace taas{
/* ============================================================================================================== */
  /*
   * constructor
   */
  taas::Labeling::Labeling(taas::Af& af){
    this->decision_id = 0;
    this->in = boost::dynamic_bitset<>(af.get_number_of_arguments());
    this->out = boost::dynamic_bitset<>(af.get_number_of_arguments());
    this->number_of_non_out_attackers = vector<int>(af.get_number_of_attackers());
    this->af = &af;
    this->conflicts = boost::dynamic_bitset<>(af.get_number_of_arguments());
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
     if(this->in.test(arg))
      return;
     this->in.set(arg);
     // check for conflict
     if(this->out.test(arg)){
       if(!this->conflicts.test(arg)){
         this->conflicts.set(arg);
         this->num_conflicts++;
       }
       return;
     }
     if(inc_decision_id)
      this->decision_id++;
     this->decision_level[arg] = this->decision_id;
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++)
      this->set_out(this->af->get_attacked(arg)[i]);
   }
/* ============================================================================================================== */
  /*
   * set out
   */
   void taas::Labeling::set_out(int arg){
     if(this->out.test(arg))
      return;
     this->out.set(arg);
     // check for conflict
     if(this->in.test(arg)){
       if(!this->conflicts.test(arg)){
         this->conflicts.set(arg);
         this->num_conflicts++;
       }
       return;
     }
     this->decision_level[arg] = this->decision_id;
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++){
      this->number_of_non_out_attackers[this->af->get_attacked(arg)[i]]--;
      if(this->number_of_non_out_attackers[this->af->get_attacked(arg)[i]] == 0 && !this->in.test(this->af->get_attacked(arg)[i]))
        this->unattacked_and_not_in_arguments.push_back(this->af->get_attacked(arg)[i]);
     }
   }
/* ============================================================================================================== */
  /*
   * reset in
   */
   void taas::Labeling::reset_in(int arg){
     if(!this->in.test(arg))
      return;
     this->in.reset(arg);
     // check for conflict
     if(this->out.test(arg)){
       this->conflicts.reset(arg);
       this->num_conflicts--;
     }
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++)
      this->undo_attack(this->af->get_attacked(arg)[i], this->decision_level[arg]);
   }
/* ============================================================================================================== */
  /*
   * An attacker of arg has been unset from IN,
   * check whether this has implications and propagate further
   */
   void taas::Labeling::undo_attack(int arg, int decision_id){
     if(!this->out.test(arg))
      return;
     if(this->decision_level[arg] != decision_id && !this->in.test(arg))
      return;
     this->out.reset(arg);
     // check for conflict
     if(this->in.test(arg)){
       this->conflicts.reset(arg);
       this->num_conflicts--;
     }
     if(this->decision_level[arg] == decision_id){
       for(int i = 0; i < this->af->get_attacked(arg).size(); i++){
        this->number_of_non_out_attackers[this->af->get_attacked(arg)[i]]++;
        if(this->in.test(this->af->get_attacked(arg)[i]) && this->decision_level[this->af->get_attacked(arg)[i]] == decision_id)
         this->not_unattacked_and_in_arguments.push_back(this->af->get_attacked(arg)[i]);
       }
     }
   }
/* ============================================================================================================== */
  /*
   * is in?
   */
   bool taas::Labeling::is_in(int arg){
     return this->in.test(arg);
   }
/* ============================================================================================================== */
  /*
   * is out?
   */
   bool taas::Labeling::is_out(int arg){
     return this->out.test(arg);
   }
/* ============================================================================================================== */
  /*
   * is unset?
   */
   bool taas::Labeling::is_reset(int arg){
     return this->in.test(arg) || this->out.test(arg);
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
     this->reset_in(arg);
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
   * print for debugging
   */
   void taas::Labeling::print_debug(){
     cout << "IN={";
     bool is_first = true;
     for( int i  = 0; i < this->af->get_number_of_arguments() ; i++ )
      if(this->in.test(i)){
        if(is_first)
          is_first = false;
        else
          cout << ",";
        cout << this->af->get_argument_name(i);
      }
     cout << "}, OUT={";
     is_first = true;
     for( int i  = 0; i < this->af->get_number_of_arguments() ; i++ )
      if(this->out.test(i)){
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
       if(this->conflicts.test(i)){
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
      if(this->in.test(i)){
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
