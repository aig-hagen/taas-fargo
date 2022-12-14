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
   void print_arg_bitset(vector<bool> & b, taas::Af& af){
     bool is_first = true;
     cout << "[";
     for( int a = 0; a < b.size(); a++ ){
       if(!b[a])
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
   * print some stack with lists argument identifiers
   */
   void print_arglist_stack(stack<vector<int>> & s, taas::Af& af){
     bool is_first= true;
     stack<vector<int>> s_tmp(s);
     cout << "<";
     while(!s_tmp.empty()){
       vector<int> args = s_tmp.top();
       s_tmp.pop();
       if(is_first) is_first = false;
       else cout << ",";
       print_arg_vector(args,af);
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
