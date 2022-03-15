/* ============================================================================================================== */
/* == BEGIN FILE ================================================================================================ */
/* ============================================================================================================== */
/*
 ============================================================================
 Name        : taas_af.h
 Author      : Matthias Thimm
 Version     : 0.1
 Copyright   : GPL3
============================================================================
*/
#include <string>
#include <vector>
#include <unordered_map>
#include <boost/dynamic_bitset.hpp>

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
      /** The initial arguments (unattacked ones) */
      vector<bool> initial;
      /** number of attacked arguments (if < number_of_arguments, there
          is at least one initial argument) */
      int number_of_attacked_arguments;
      /** Self-attacking arguments */
      vector<bool> loops;
    protected:

    public:
      Af();
      void add_argument(string arg_name);
      void add_attack(string arg_name1, string arg_name2);
      int get_argument_index(string arg_name);
      void print();
      bool is_loop(int arg);
      bool is_initial(int arg);
      bool has_initial_arguments();
  };

}
/* ============================================================================================================== */
/* == END FILE ================================================================================================== */
/* ============================================================================================================== */
