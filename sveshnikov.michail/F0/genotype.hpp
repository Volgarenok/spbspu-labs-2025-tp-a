#ifndef GENOTYPE_HPP
#define GENOTYPE_HPP
#include <vector>
#include "gen.hpp"

namespace sveshnikov
{
  class Genotype
  {
  public:
    Genotype() = default;
    explicit Genotype(const std::vector< Gen > &genes);

    bool operator==(const Genotype &other) const noexcept;
    bool operator!=(const Genotype &other) const noexcept;

    const std::vector< Gen > &get_genes() const noexcept;
    int calc_fitness() const;
    Genotype crossover(const Genotype &other) const;

  private:
    std::vector< Gen > genes_;
  };

  std::istream &operator>>(std::istream &in, Genotype &genotype);
  std::ostream &operator<<(std::ostream &out, const Genotype &genotype);
}

#endif
