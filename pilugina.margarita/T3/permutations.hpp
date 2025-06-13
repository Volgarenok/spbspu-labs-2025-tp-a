#ifndef PERMUTATIONS_HPP
#define PERMUTATIONS_HPP

#include "polygon.hpp"

namespace pilugina
{
  struct PermutationPredicate
  {
    PermutationPredicate(const Polygon &ref);
    bool operator()(const Polygon &other) const;

  private:
    const Polygon &poly;
  };
}

#endif
