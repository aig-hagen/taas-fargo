/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_fast_pqueue.h
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#pragma once

#include "taas_compare.h"

#include <queue>
#include <vector>

using namespace std;

namespace taas{

  class FastPriorityQueue {
    private:
      vector<bool> b;
      priority_queue<int,vector<int>,taas::ArgumentCompare> q;
      vector<int> pred;
      void trim();
    public:
      FastPriorityQueue(int number_of_arguments, const taas::ArgumentCompare & c);
      void add(int arg, int pred);
      void remove(int arg);
      int top_and_pop();
      bool contains(int arg);
      bool empty();
      int get_pred(int arg);
      void print(taas::Af & af);
  };
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
