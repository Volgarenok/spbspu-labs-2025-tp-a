#ifndef KEYS_HPP
#define KEYS_HPP
#include "population.hpp"

namespace sveshnikov
{
  using populations_t = std::map< std::string, Population >;

  void print_help_manual(std::ostream &out);
  bool check_format(std::istream &in);
  void load(populations_t &populations, std::istream &in);
}

#endif
