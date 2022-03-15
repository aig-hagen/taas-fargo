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
    this->unattacked_and_not_in_arguments = vector<int>();
    for( int i = 0; i < af.get_number_of_arguments(); i++ )
      if(this->number_of_non_out_attackers[i] == 0)
        this->unattacked_and_not_in_arguments.push_back(i);
  }
/* ============================================================================================================== */
/* ============================================================================================================== */
  /*
   * set in
   */
   void taas::Labeling::set_in(int arg){
     this->in.set(arg);
     this->out.reset(arg);
     for(int a: this->af->get_attacked(arg))
      this->set_out(a);
   }
/* ============================================================================================================== */
  /*
   * set out
   */
   void taas::Labeling::set_out(int arg){
     if(this->out.test(arg))
      return;
     this->in.reset(arg);
     this->out.set(arg);
     for(int a: this->af->get_attacked(arg)){
      this->number_of_non_out_attackers[a]--;
      if(this->number_of_non_out_attackers[a] == 0)
        this->unattacked_and_not_in_arguments.push_back(a);
     }
   }
/* ============================================================================================================== */
  /*
   * reset
   */
   void taas::Labeling::reset(int arg){
     this->in.reset(arg);
     this->out.reset(arg);
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
