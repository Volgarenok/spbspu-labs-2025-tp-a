#ifndef GENOTYPE_HPP
#define GENOTYPE_HPP
#include "gen.hpp"

namespace sveshnikov
{
  class Genotype
  {
  public:
    Genotype() = default;
    explicit Genotype(const std::array< Gen, 4 > &genes);

    bool operator==(const Genotype &other) const;
    bool operator!=(const Genotype &other) const;

    const std::array< Gen, 4 > &get_genes() const noexcept;
    int calc_fitness() const;
    Genotype &crossover(const Genotype &other);

  private:
    std::array< Gen, 4 > genes_;
  };

  std::istream &operator>>(std::istream &in, Genotype &genotype);
  std::ostream &operator<<(std::ostream &out, const Genotype &genotype);
}

#endif
