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
    const Individual &get_individual(const std::string &individ) const noexcept;

    void add(Individual &individ);
    void remove(const std::string &individ);

    void mutate(const std::string &individ);
    int calc_fitness(const std::string &individ) const;
    void pedigree(const std::string &individ, std::ostream &out) const noexcept;
    void make_older(size_t years) noexcept;
    Population crossover(const std::string &name, const std::string &parent1,
        const std::string &parent2) const;
    void print_list(std::ostream &out) const noexcept;

    size_t get_size() const noexcept;
    int get_average_fitness() const noexcept;
    int get_min_fitness() const noexcept;
    int get_max_fitness() const noexcept;
    int get_average_age() const noexcept;
    int get_min_age() const noexcept;
    int get_max_age() const noexcept;

    void select(int survival_threshold);
    Population unite(const std::string &name, const Population &population) const;

  private:
    std::map< std::string, Individual > population_;
    std::map< std::string, Individual > cemetery;
  };

  std::istream &operator>>(std::istream &in, Population &genotype);
  std::ostream &operator<<(std::ostream &out, const Population &genotype);
}

#endif
