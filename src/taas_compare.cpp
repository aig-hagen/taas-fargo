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
#include "taas_fast_pqueue.h"

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
  taas::ArgumentCompare::ArgumentCompare(taas::Af& af, taas::Labeling& lab, taas::SortOrder order, taas::SortByDecisionLevel dl_order){
    this->af = &af;
    this->lab = &lab;
    this->order = order;
    this->dl_order = dl_order;
  }
/* ============================================================================================================== */
  /*
   * set must_out
   */
   void taas::ArgumentCompare::set_must_out(taas::FastPriorityQueue & must_out){
     this->must_out = &must_out;
   }
/* ============================================================================================================== */
 /*
  * compare
  */
  bool taas::ArgumentCompare::operator()(int arg1, int arg2){
    if(this->dl_order != taas::OFF){
      // we only do pre-sorting by decision level for must_out arguments
      // and compare the decision levels of their corresponding predecessors
      if(this->lab->get_decision_level(this->must_out->get_pred(arg1)) < this->lab->get_decision_level(this->must_out->get_pred(arg2)))
        return this->dl_order == taas::ASC;
      else if(this->lab->get_decision_level(this->must_out->get_pred(arg1)) > this->lab->get_decision_level(this->must_out->get_pred(arg2)))
        return this->dl_order != taas::ASC;
    }
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
