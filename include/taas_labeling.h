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

#include <vector>

using namespace std;

namespace taas{

  class Labeling{
    private:
      unsigned int decision_id;
      vector<bool> in;
      vector<bool> out;
      vector<int> number_of_non_out_attackers;
      taas::Af* af;
      vector<int> unattacked_and_not_in_arguments;
      vector<int> not_unattacked_and_in_arguments;
      vector<bool> conflicts;
      int num_conflicts;
      vector<int> decision_level;
    public:
      Labeling(taas::Af& af);
      void set_in(int arg,bool inc_decision_id = true);
      void set_out(int arg);
      void reset_in(int arg);
      void undo_attack(int arg, int decision_id);
      bool is_in(int arg);
      bool is_out(int arg);
      bool is_reset(int arg);
      bool faf();
      bool rev_faf();
      bool has_conflict();
      int get_number_of_non_out_attackers(int arg);
      void print_debug();
      void print();
  };

}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
