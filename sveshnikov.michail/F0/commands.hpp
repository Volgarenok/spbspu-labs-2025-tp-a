#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "population.hpp"

namespace sveshnikov
{
  using populations_t = std::map< std::string, Population >;

  void load(populations_t &populations, std::istream &in);
  bool check_format(std::istream &in);
  void print_help_manual(std::ostream &out);

  void add_population(populations_t &populations, std::istream &in);
  void remove_population(populations_t &populations, std::istream &in);
  void add(populations_t &populations, std::istream &in);
  void remove(populations_t &populations, std::istream &in);
  void mutate(populations_t &populations, std::istream &in);
  void calc_fitness(const populations_t &populations, std::istream &in, std::ostream &out);
  void print_pedigree(const populations_t &populations, std::istream &in, std::ostream &out);
  void crossover(populations_t &populations, std::istream &in);
  void print_list(const populations_t &populations, std::istream &in, std::ostream &out);
  void print_stats(const populations_t &populations, std::istream &in, std::ostream &out);
  void save(const populations_t &populations, std::istream &in, std::ostream &out);
  void select(populations_t &populations, std::istream &in);
  void make_older(populations_t &populations, std::istream &in);
  void unite(populations_t &populations, std::istream &in);
  void list_population(const populations_t &populations, std::ostream &out);
}

#endif
