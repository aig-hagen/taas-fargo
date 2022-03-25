/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_af.h
 Author      : Matthias Thimm
 Version     : 1.0
 Copyright   : GPL3
============================================================================
*/
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

namespace taas{

  class Af{
    private:
      /** Mapping internal argument identifiers (ints) to argument names **/
      vector<string> ids2arguments;
      /** Mapping argument names to internal argument identifiers (ints)**/
      unordered_map<string,int> arguments2ids;
      /** The number of arguments. */
      int number_of_arguments;
      /** The number of attacks. */
      int number_of_attacks;
      /** Maps arguments to their children */
      vector<vector<int>> children;
      /** Maps arguments to their parents */
      vector<vector<int>> parents;
      /** Maps arguments to the number of their attackers */
      vector<int> number_of_attackers;
      /** number of attacked arguments (if < number_of_arguments, there
          is at least one initial argument) */
      int number_of_attacked_arguments;
      /** Self-attacking arguments */
      vector<bool> loops;
    public:
      Af();
      void add_argument(const string& arg_name);
      void add_attack(const string& arg_name1, const string& arg_name2);
      void print();
      int get_argument_index(const string& arg_name);
      const string& get_argument_name(int argument);
      bool is_loop(int arg);
      bool is_initial(int arg);
      bool has_initial_arguments();
      int get_number_of_arguments();
      vector<int>& get_attacked(int arg);
      vector<int>& get_attackers(int arg);
      vector<int>& get_number_of_attackers();
  };

}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
