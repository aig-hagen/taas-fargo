/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_fast_pqueue.cpp
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#include "taas_fast_pqueue.h"
#include "taas_util.h"

#include <iostream>
/* ============================================================================================================== */
namespace taas{
/* ============================================================================================================== */
  /*
   * copy constructor
   */
  taas::FastPriorityQueue::FastPriorityQueue(const taas::FastPriorityQueue & other){
    this->b = boost::dynamic_bitset<>(other.b);
    this->q = priority_queue<int,vector<int>,taas::ArgumentCompare>(other.q);
    this->pred = vector<int>(other.pred);
  }
/* ============================================================================================================== */
  /*
   * constructor
   */
  taas::FastPriorityQueue::FastPriorityQueue(int num_of_arguments,const taas::ArgumentCompare & c){
      this->b = boost::dynamic_bitset<>(num_of_arguments);
      this->q = priority_queue<int,vector<int>,taas::ArgumentCompare>(c);
      this->pred = vector<int>(num_of_arguments);
  }
/* ============================================================================================================== */
  /*
   * trim
   */
  void taas::FastPriorityQueue::trim(){
    while(!this->q.empty()){
      if(!this->b.test(this->q.top()))
        this->q.pop();
      else return;
    }
  }
/* ============================================================================================================== */
  /*
   * add
   */
  void taas::FastPriorityQueue::add(int arg, int pred){
    this->b.set(arg);
    this->q.push(arg);
    this->pred[arg] = pred;
  }
/* ============================================================================================================== */
  /*
   * remove
   */
   void taas::FastPriorityQueue::remove(int arg){
     this->b.reset(arg);
     // do no explicitly remove from q,
     // this is done lazily by checking b when needed
   }
/* ============================================================================================================== */
  /*
   * top and pop
   */
   int taas::FastPriorityQueue::top_and_pop(){
     if(this->empty())
      return -1;
     int next_out = this->q.top();
     this->q.pop();
     this->b.reset(next_out);
     return next_out;
   }
/* ============================================================================================================== */
  /*
   * contains
   */
   bool taas::FastPriorityQueue::contains(int arg){
     return this->b.test(arg);
   }
/* ============================================================================================================== */
  /*
   * empty
   */
   bool taas::FastPriorityQueue::empty(){
     this->trim();
     return this->q.empty();
   }
/* ============================================================================================================== */
  /*
   * get pred
   */
   int taas::FastPriorityQueue::get_pred(int arg){
     return this->pred[arg];
   }
/* ============================================================================================================== */
  /*
   * print
   */
   void taas::FastPriorityQueue::print(taas::Af & af){
     print_arg_bitset(this->b,af);
   }
/* ============================================================================================================== */
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
