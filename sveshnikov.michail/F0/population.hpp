#ifndef POPULATION_HPP
#define POPULATION_HPP
#include <utility>
#include <map>
#include "individual.hpp"

namespace sveshnikov
{
  class Population
  {
  public:
    void add(const Individual &individ);
    void remove(const std::string &name);

    void mutate(const std::string &name, const Genotype &genotype);
    int calc_fitness(const std::string &name) const;
    void print_pedigree(const std::string &name, std::ostream &out) const;
    void make_older(size_t years);
    void crossover(const std::string &name, const std::string &parent1, const std::string &parent2);
    void print_list(std::ostream &out, const std::string &life_specifier) const;

    size_t get_size() const noexcept;
    const Individual &get_individual(const std::string &name) const;
    int get_average_fitness() const;
    int get_min_fitness() const;
    int get_max_fitness() const;
    int get_average_age() const;
    int get_min_age() const;
    int get_max_age() const;

    void select(int survival_threshold);
    void unite(const Population &other);

  private:
    std::map< std::string, Individual > population_;
    std::map< std::string, Individual > cemetery_;

    void printPedigreeLine(const std::string &childName, const sveshnikov::Individual &child,
        const std::string &parentName, std::ostream &out) const;
  };

  std::istream &operator>>(std::istream &in, Population &genotype);
  std::ostream &operator<<(std::ostream &out, const Population &genotype);
}

#endif
