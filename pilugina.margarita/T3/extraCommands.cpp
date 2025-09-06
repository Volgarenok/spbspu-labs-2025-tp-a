#include "extraCommands.hpp"
#include <algorithm>
#include <functional>

int pilugina::EchoTransform::operator()(const Polygon &p) const
{
  if (p == poly)
  {
    result.push_back(p);
    result.push_back(p);
  }
  else
  {
    result.push_back(p);
  }
  return 0;
}

int pilugina::echo(std::vector< Polygon > &polygons, const Polygon &target)
{
  using namespace std::placeholders;
  int count = std::count(polygons.begin(), polygons.end(), target);
  std::vector< Polygon > result;
  result.reserve(polygons.size() + count);
  std::copy(polygons.begin(), polygons.end(), std::back_inserter(result));
  auto polygonEqT = std::bind(std::equal_to< Polygon >(), std::placeholders::_1, target);
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(result), polygonEqT);

  polygons.swap(result);
  return count;
}

pilugina::IntersectsWith::IntersectsWith(const Polygon &polygon):
  polygon_(polygon)
{}

bool pilugina::IntersectsWith::operator()(const Polygon &other) const
{
  auto left = std::minmax_element(polygon_.points.begin(), polygon_.points.end());
  auto right = std::minmax_element(other.points.begin(), other.points.end());
  return !((*left.second < *right.first) || (*right.second < *left.first));
}

pilugina::PermutationPredicate::PermutationPredicate(const Polygon &ref):
  poly(ref)
{}

bool pilugina::PermutationPredicate::operator()(const Polygon &other) const
{
  if (poly.points.size() != other.points.size())
  {
    return false;
  }
  bool is_permutation = std::is_permutation(other.points.begin(), other.points.end(), poly.points.begin());
  return is_permutation;
}
