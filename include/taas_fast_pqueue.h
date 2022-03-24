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

#include <boost/dynamic_bitset.hpp>
#include <queue>
#include <vector>

using namespace std;

namespace taas{

  class FastPriorityQueue {
    private:
      boost::dynamic_bitset<> b;
      priority_queue<int,vector<int>,taas::ArgumentCompare> q;
      vector<int> pred;
      void trim();
    public:
      FastPriorityQueue(const taas::FastPriorityQueue & other);
      FastPriorityQueue(int num_of_arguments,const taas::ArgumentCompare & c);
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
