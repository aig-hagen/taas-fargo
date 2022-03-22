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
    this->in = boost::dynamic_bitset<>(af.get_number_of_arguments());
    this->out = boost::dynamic_bitset<>(af.get_number_of_arguments());
    this->number_of_non_out_attackers = vector<int>(af.get_number_of_attackers());
    this->af = &af;
    this->conflicts = boost::dynamic_bitset<>(af.get_number_of_arguments());
    this->num_conflicts = 0;
    this->visited_attacks = vector<boost::dynamic_bitset<>>(af.get_number_of_arguments());
    this->unattacked_and_not_in_arguments = vector<int>();
    this->not_unattacked_and_in_arguments = vector<int>();
    this->pred = vector<int>(af.get_number_of_arguments());
    for( int i = 0; i < this->af->get_number_of_arguments(); i++ ){
      this->visited_attacks[i].resize(this->af->get_attacked(i).size());
      if(this->number_of_non_out_attackers[i] == 0)
        this->unattacked_and_not_in_arguments.push_back(i);
    }
  }
/* ============================================================================================================== */
/* ============================================================================================================== */
  /*
   * set in
   */
   void taas::Labeling::set_in(int arg){
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
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++){
      this->visited_attacks[arg].set(i);
      this->set_out(this->af->get_attacked(arg)[i], arg);
     }
   }
/* ============================================================================================================== */
  /*
   * set out
   */
   void taas::Labeling::set_out(int arg, int from){
     if(this->out.test(arg))
      return;
     this->out.set(arg);
     this->pred[arg] = from;
     // check for conflict
     if(this->in.test(arg)){
       if(!this->conflicts.test(arg)){
         this->conflicts.set(arg);
         this->num_conflicts++;
       }
       return;
     }
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++){
      this->visited_attacks[arg].set(i);
      this->number_of_non_out_attackers[this->af->get_attacked(arg)[i]]--;
      if(this->number_of_non_out_attackers[this->af->get_attacked(arg)[i]] == 0 && !this->in.test(this->af->get_attacked(arg)[i])){
        this->pred[this->af->get_attacked(arg)[i]] = arg;
        this->unattacked_and_not_in_arguments.push_back(this->af->get_attacked(arg)[i]);
      }
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
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++){
      if(this->visited_attacks[arg].test(i)){
        this->visited_attacks[arg].reset(i);
        this->undo_attack(this->af->get_attacked(arg)[i], arg);
      }
     }
   }
/* ============================================================================================================== */
  /*
   * An attacker of arg has been unset from IN,
   * check whether this has implications and propagate further
   */
   void taas::Labeling::undo_attack(int arg, int from){
     if(!this->out.test(arg))
      return;
     if(this->pred[arg] != from)
      return;
     this->out.reset(arg);
     // check for conflict
     if(this->in.test(arg)){
       this->conflicts.reset(arg);
       this->num_conflicts--;
     }
     for(int i = 0; i < this->af->get_attacked(arg).size(); i++){
      if(this->visited_attacks[arg].test(i)){
        this->visited_attacks[arg].reset(i);
        this->number_of_non_out_attackers[this->af->get_attacked(arg)[i]]++;
        if(this->in.test(this->af->get_attacked(arg)[i]) && this->pred[this->af->get_attacked(arg)[i]] == arg)
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
     this->set_in(arg);
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
     cout << "}" << endl;
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
