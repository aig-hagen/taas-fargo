/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_compare.h
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#pragma once

#include "taas_af.h"
#include "taas_labeling.h"

namespace taas{
  class FastPriorityQueue;

  enum SortOrder {NUMERICAL,ASC_IN_DEGREE,DESC_IN_DEGREE,ASC_OUT_DEGREE,DESC_OUT_DEGREE};
  enum SortByDecisionLevel {OFF,ASC,DESC};

  class ArgumentCompare {
    private:
      taas::Af* af;
      taas::Labeling* lab;
      taas::SortOrder order;
      taas::SortByDecisionLevel dl_order;
      taas::FastPriorityQueue* must_out;
    public:
      ArgumentCompare();
      ArgumentCompare(taas::Af& af, taas::Labeling& lab, taas::SortOrder order, taas::SortByDecisionLevel dl_order);
      void set_must_out(taas::FastPriorityQueue & must_out);
      bool operator()(int arg1, int arg2);
  };

}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
