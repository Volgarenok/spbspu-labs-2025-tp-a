#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP
#include <utility>
#include "genotype.hpp"

namespace sveshnikov
{
  class Individual
  {
  public:
    Individual() = default;
    Individual(const std::string &name, const Genotype &genotype, size_t age = 0,
        const std::string &parent1 = "", const std::string &parent2 = "");

    bool operator==(const Individual &other) const noexcept;
    bool operator!=(const Individual &other) const noexcept;

    const std::string &get_name() const noexcept;
    const Genotype &get_genotype() const noexcept;
    size_t get_age() const noexcept;
    const std::pair< std::string, std::string > &get_parents() const;

    void make_older(size_t years) noexcept;
    void mutate(const Genotype &genotype);
    int calc_fitness() const;
    Individual crossover(const std::string &name, const Individual &other) const;

  private:
    std::string name_;
    Genotype genotype_;
    size_t age_;
    std::pair< std::string, std::string > parents_;
  };

  std::istream &operator>>(std::istream &in, Individual &genotype);
  std::ostream &operator<<(std::ostream &out, const Individual &genotype);
}

#endif
