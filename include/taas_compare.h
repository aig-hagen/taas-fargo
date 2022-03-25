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

  enum SortOrder {NONE,ASC_IN_DEGREE,DESC_IN_DEGREE,ASC_OUT_DEGREE,DESC_OUT_DEGREE};

  class ArgumentCompare {
    private:
      taas::Af* af;
      taas::Labeling* lab;
      taas::SortOrder order;
    public:
      ArgumentCompare();
      ArgumentCompare(taas::Af& af, taas::Labeling& lab, taas::SortOrder order);
      bool operator()(int arg1, int arg2);
  };

}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
