#include "permutations.hpp"

pilugina::PermutationPredicate::PermutationPredicate(const Polygon &ref):
  poly(ref)
{}

bool pilugina::PermutationPredicate::operator()(const Polygon &other) const
{
  return (poly.points.size() == other.points.size()) && (std::is_permutation(other.points.begin(), other.points.end(), poly.points.begin()));
}
