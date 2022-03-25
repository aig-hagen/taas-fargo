/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_compare.cpp
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#include "taas_compare.h"

#include <iostream>
/* ============================================================================================================== */
namespace taas{
/* ============================================================================================================== */
  /*
   * constructor
   */
  taas::ArgumentCompare::ArgumentCompare(){
  }
/* ============================================================================================================== */
  /*
   * constructor
   */
  taas::ArgumentCompare::ArgumentCompare(taas::Af& af, taas::Labeling& lab, taas::SortOrder order){
    this->af = &af;
    this->lab = &lab;
    this->order = order;
  }
/* ============================================================================================================== */
 /*
  * compare
  */
  bool taas::ArgumentCompare::operator()(int arg1, int arg2){
    if(this->order == taas::ASC_IN_DEGREE)
      return this->lab->get_number_of_non_out_attackers(arg1) < this->lab->get_number_of_non_out_attackers(arg2);
    if(this->order == taas::DESC_IN_DEGREE)
      return this->lab->get_number_of_non_out_attackers(arg1) > this->lab->get_number_of_non_out_attackers(arg2);
    if(this->order == taas::ASC_OUT_DEGREE)
      return this->af->get_attacked(arg1).size() < this->af->get_attacked(arg2).size();
    if(this->order == taas::DESC_IN_DEGREE)
      return this->af->get_attacked(arg1).size() > this->af->get_attacked(arg2).size();
    return arg1 < arg2;
  }
  
/* ============================================================================================================== */
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
