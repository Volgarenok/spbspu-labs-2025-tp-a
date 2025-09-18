#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "population.hpp"

namespace sveshnikov
{
  using populations_t = std::map< std::string, Population >;

  void addPopulation(populations_t &populations, std::istream &in);
  void removePopulation(populations_t &populations, std::istream &in);

  void add(populations_t &populations, std::istream &in);
  void removeInds(populations_t &populations, std::istream &in);
  void mutate(populations_t &populations, std::istream &in);
  void calcFitness(const populations_t &populations, std::istream &in, std::ostream &out);
  void printPedigree(const populations_t &populations, std::istream &in, std::ostream &out);
  void crossover(populations_t &populations, std::istream &in);

  void printList(const populations_t &populations, std::istream &in, std::ostream &out);
  void printStats(const populations_t &populations, std::istream &in, std::ostream &out);

  void selectInds(populations_t &populations, std::istream &in);

  void makeOlder(populations_t &populations, std::istream &in);
  void unite(populations_t &populations, std::istream &in);
  void listPopulation(const populations_t &populations, std::ostream &out);

  void save(const populations_t &populations, std::istream &in, std::ostream &out);
  void load(populations_t &populations, std::istream &in);
}

#endif
