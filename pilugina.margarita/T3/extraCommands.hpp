#ifndef EXTRA_COMMANDS_HPP
#define EXTRA_COMMANDS_HPP

#include <vector>
#include <cmath>
#include "polygon.hpp"

namespace pilugina
{
  int echo(std::vector< Polygon > &, const Polygon &);

  struct EchoTransform
  {
    const Polygon &poly;
    std::vector< Polygon > &result;

    int operator()(const Polygon &p) const;
  };

  struct IntersectsWith
  {
    IntersectsWith(const Polygon &polygon);
    bool operator()(const Polygon &other) const;

  private:
    Polygon polygon_;
  };

  struct PermutationPredicate
  {
    PermutationPredicate(const Polygon &ref);
    bool operator()(const Polygon &other) const;

  private:
    const Polygon &poly;
  };
}

#endif
