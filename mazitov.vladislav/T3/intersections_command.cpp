#include "intersections_command.hpp"
#include <algorithm>

mazitov::IntersectsWith::IntersectsWith(const Polygon &polygon):
  polygon_(polygon)
{}

bool mazitov::IntersectsWith::operator()(const Polygon &other) const
{
  auto left = std::minmax_element(polygon_.points.begin(), polygon_.points.end());
  auto right = std::minmax_element(other.points.begin(), other.points.end());
  return !((*left.second < *right.first) || (*right.second < *left.first));
}
