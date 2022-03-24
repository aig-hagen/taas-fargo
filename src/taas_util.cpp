/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_util.cpp
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#include "taas_util.h"

#include <boost/dynamic_bitset.hpp>
#include <iostream>
#include <vector>

using namespace std;

/* ============================================================================================================== */
namespace taas{
/* ============================================================================================================== */
  /*
   * print some vector with argument identifiers
   */
   void print_arg_vector(vector<int> & v, taas::Af& af){
    bool is_first = true;
    cout << "[";
    for( int a: v ){
      if(is_first)
         is_first = false;
      else
        cout << ",";
      cout << af.get_argument_name(a);
    }
    cout << "]";
   }
/* ============================================================================================================== */
  /*
   * print some bitset with argument identifiers
   */
   void print_arg_bitset(boost::dynamic_bitset<> & b, taas::Af& af){
     bool is_first = true;
     cout << "[";
     for( int a = 0; a < b.size(); a++ ){       
       if(!b.test(a))
        continue;
       if(is_first)
          is_first = false;
       else
         cout << ",";
       cout << af.get_argument_name(a);
     }
     cout << "]";
   }
/* ============================================================================================================== */
  /*
   * print some stack with argument identifiers
   */
   void print_arg_stack(stack<int> & s, taas::Af& af){
     bool is_first= true;
     stack<int> s_tmp(s);
     cout << "<";
     while(!s_tmp.empty()){
       int arg = s_tmp.top();
       s_tmp.pop();
       if(is_first) is_first = false;
       else cout << ",";
       cout << af.get_argument_name(arg);
     }
     cout << ">";
   }
/* ============================================================================================================== */
   /*
    * print some stack with argument identifiers
    */
    void print_pqueue_stack(stack<taas::FastPriorityQueue*> & s, taas::Af& af){
      bool is_first= true;
      stack<taas::FastPriorityQueue*> s_tmp(s);
      cout << "<";
      while(!s_tmp.empty()){
        taas::FastPriorityQueue* q = s_tmp.top();
        s_tmp.pop();
        if(is_first) is_first = false;
        else cout << ",";
        q->print(af);
      }
      cout << ">";
    }
/* ============================================================================================================== */
}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
