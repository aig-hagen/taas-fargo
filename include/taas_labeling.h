/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_labeling.h
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#pragma once

#include "taas_af.h"

#include <boost/dynamic_bitset.hpp>
#include <vector>

using namespace std;

namespace taas{

  class Labeling{
    private:
      boost::dynamic_bitset<> in;
      boost::dynamic_bitset<> out;
      vector<int> number_of_non_out_attackers;
      taas::Af* af;
      vector<int> unattacked_and_not_in_arguments;
    public:
      Labeling(taas::Af& af);
      void set_in(int arg);
      void set_out(int arg);
      void reset(int arg);
      bool is_in(int arg);
      bool is_out(int arg);
      bool is_reset(int arg);
      bool faf();
      void print_debug();
      void print();
  };

}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
