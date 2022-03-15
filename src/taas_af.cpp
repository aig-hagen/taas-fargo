/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_af.cpp
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#include "taas_af.h"

#include <iostream>

using namespace std;
/* ============================================================================================================== */
namespace taas{
/* ============================================================================================================== */
  /*
   * constructor
   */
  taas::Af::Af(){
    this->number_of_arguments = 0;
    this->number_of_attacks = 0;
    this->number_of_attacked_arguments = 0;
  }
/* ============================================================================================================== */
  /*
   * add some argument
   */
   void taas::Af::add_argument(const string& arg_name){
     this->number_of_arguments++;
     this->ids2arguments.push_back(arg_name);
     this->arguments2ids[arg_name] = this->number_of_arguments-1;
     this->children.push_back(vector<int>());
     this->parents.push_back(vector<int>());
     this->number_of_attackers.push_back(0);
     this->loops.push_back(false);
   }
/* ============================================================================================================== */
/*
 * add some attack
 */
 void taas::Af::add_attack(const string& arg_name1, const string& arg_name2){
   this->number_of_attacks++;
   int arg1 = this->arguments2ids[arg_name1];
   int arg2 = this->arguments2ids[arg_name2];
   this->children[arg1].push_back(arg2);
   this->parents[arg2].push_back(arg1);
   if(this->number_of_attackers[arg2] == 0)
    this->number_of_attacked_arguments++;
   this->number_of_attackers[arg2]++;
   if(arg1 == arg2)
    this->loops[arg1] = true;
 }
/* ============================================================================================================== */
 /*
  * print the AF
  */
  void taas::Af::print(){
    cout << "<{";
    bool is_first = true;
    for(int arg = 0; arg < this->number_of_arguments; arg++ ){
      if(is_first)
        is_first = false;
      else cout << ",";
      cout << this->ids2arguments[arg];
    }
    cout << "},{";
    is_first = true;
    for(int arg1 = 0; arg1 < this->number_of_arguments; arg1++ ){
      for(int arg2 = 0; arg2 < this->children[arg1].size(); arg2++){
        if(is_first)
          is_first = false;
        else cout << ",";
        cout << "(" << this->ids2arguments[arg1] << "," << this->ids2arguments[this->children[arg1][arg2]] << ")";
      }
    }
    cout << "}>\n";
  }
/* ============================================================================================================== */
 /*
  * get argument index
  */
  int taas::Af::get_argument_index(const string& arg_name){
    return this->arguments2ids[arg_name];
  }
/* ============================================================================================================== */
 /*
  * get argument name
  */
  const string& taas::Af::get_argument_name(int argument){
    return this->ids2arguments[argument];
  }
/* ============================================================================================================== */
 /*
  * is the argument in a loop?
  */
  bool taas::Af::is_loop(int arg){
    return this->loops[arg];
  }
/* ============================================================================================================== */
 /*
  * is the argument initial?
  */
  bool taas::Af::is_initial(int arg){
    return this->number_of_attackers[arg] == 0;
  }
/* ============================================================================================================== */
 /*
  * is there an unattacked argument?
  */
  bool taas::Af::has_initial_arguments(){
    return (this->number_of_arguments - this->number_of_attacked_arguments) > 0;
  }
/* ============================================================================================================== */
 /*
  * get number of arguments
  */
  int taas::Af::get_number_of_arguments(){
    return this->number_of_arguments;
  }
/* ============================================================================================================== */
 /*
  * get the attacked of arg
  */
  vector<int>& taas::Af::get_attacked(int arg){
    return this->children[arg];
  }
/* ============================================================================================================== */
 /*
  * get vector (by reference) of number of attackers
  */
  vector<int>& taas::Af::get_number_of_attackers(){
    return this->number_of_attackers;
  }
/* ============================================================================================================== */
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
