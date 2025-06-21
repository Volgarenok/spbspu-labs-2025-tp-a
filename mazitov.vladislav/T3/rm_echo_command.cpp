#include "rm_echo_command.hpp"
#include <algorithm>

namespace
{
  using mazitov::Polygon;
  struct RmEchoPredicate
  {
    RmEchoPredicate(const Polygon &poly):
      ref(poly)
    {}

    bool operator()(const Polygon &p1, const Polygon &p2)
    {
      return (p1 == p2) && (ref == p1);
    }

    const Polygon &ref;
  };
}

std::size_t mazitov::getRmEcho(std::vector< Polygon > &polygons, const Polygon &ref)
{
  auto last = std::unique(polygons.begin(), polygons.end(), RmEchoPredicate{ref});
  polygons.erase(last, polygons.end());
  return std::distance(last, polygons.end());
}
