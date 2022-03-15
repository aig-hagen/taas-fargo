/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_af.cpp
 Author      : Matthias Thimm
 Version     : 0.1
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
   void taas::Af::add_argument(string arg_name){
     this->number_of_arguments++;
     this->ids2arguments.push_back(arg_name);
     this->arguments2ids[arg_name] = this->number_of_arguments-1;
     this->children.push_back(vector<int>());
     this->parents.push_back(vector<int>());
     this->initial.push_back(true);
     this->loops.push_back(false);
   }
/* ============================================================================================================== */
/*
 * add some attack
 */
 void taas::Af::add_attack(string arg_name1, string arg_name2){
   this->number_of_attacks++;
   int arg1 = this->arguments2ids[arg_name1];
   int arg2 = this->arguments2ids[arg_name2];
   this->children[arg1].push_back(arg2);
   this->parents[arg2].push_back(arg1);
   if(this->initial[arg2]){
     this->initial[arg2] = false;
     this->number_of_attacked_arguments++;
   }
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
  int taas::Af::get_argument_index(string arg_name){
    return this->arguments2ids[arg_name];
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
    return this->initial[arg];
  }
/* ============================================================================================================== */
 /*
  * is there an unattacked argument?
  */
  bool taas::Af::has_initial_arguments(){
    return (this->number_of_arguments - this->number_of_attacked_arguments) > 0;
  }
/* ============================================================================================================== */
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
