#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <vector>

namespace {

  struct AreaTransformFunctor {
    double operator()(const gavrilova::Polygon& p) const
    {
      return gavrilova::getArea(p);
    }
  };

  struct OddAreaTransformFunctor {
    double operator()(const gavrilova::Polygon& p) const
    {
      return gavrilova::hasOddVertices(p) ? gavrilova::getArea(p) : 0.0;
    }
  };

  struct EvenAreaTransformFunctor {
    double operator()(const gavrilova::Polygon& p) const
    {
      return gavrilova::hasEvenVertices(p) ? gavrilova::getArea(p) : 0.0;
    }
  };

  struct SpecificVertexAreaTransformFunctor {
    size_t num_vertices;

    explicit SpecificVertexAreaTransformFunctor(size_t n):
      num_vertices(n)
    {}

    double operator()(const gavrilova::Polygon& p) const
    {
      return p.points.size() == num_vertices ? gavrilova::getArea(p) : 0.0;
    }
  };

  struct IsOddPredicate {
    bool operator()(const gavrilova::Polygon& p) const
    {
      return gavrilova::hasOddVertices(p);
    }
  };

  struct IsEvenPredicate {
    bool operator()(const gavrilova::Polygon& p) const
    {
      return gavrilova::hasEvenVertices(p);
    }
  };

  struct HasNVertexesPredicate {
    size_t num_vertices;

    explicit HasNVertexesPredicate(size_t n):
      num_vertices(n)
    {}

    bool operator()(const gavrilova::Polygon& p) const
    {
      return p.points.size() == num_vertices;
    }
  };

  struct AreaComparator {
    bool operator()(const gavrilova::Polygon& a, const gavrilova::Polygon& b) const
    {
      return gavrilova::getArea(a) < gavrilova::getArea(b);
    }
  };

  struct VertexesComparator {
    bool operator()(const gavrilova::Polygon& a, const gavrilova::Polygon& b) const
    {
      return a.points.size() < b.points.size();
    }
  };

  struct IsPermutationPredicate {
    gavrilova::Polygon ref_polygon;

    explicit IsPermutationPredicate(const gavrilova::Polygon& p):
      ref_polygon(p)
    {
      std::sort(ref_polygon.points.begin(), ref_polygon.points.end());
    }

    bool operator()(const gavrilova::Polygon& p) const
    {
      if (p.points.size() != ref_polygon.points.size()) {
        return false;
      }
      auto temp_points = p.points;
      std::sort(temp_points.begin(), temp_points.end());
      return temp_points == ref_polygon.points;
    }
  };

  struct IsLessAreaPredicate {
    double ref_area;

    explicit IsLessAreaPredicate(const gavrilova::Polygon& p):
      ref_area(gavrilova::getArea(p))
    {}

    bool operator()(const gavrilova::Polygon& p) const
    {
      return gavrilova::getArea(p) < ref_area;
    }
  };

}

void gavrilova::execArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string arg;
  in >> arg;
  std::vector< double > areas;
  areas.reserve(polygons.size());
  double total_area = 0;

  if (arg == "ODD") {
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), OddAreaTransformFunctor());
  } else if (arg == "EVEN") {
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), EvenAreaTransformFunctor());
  } else if (arg == "MEAN") {
    if (polygons.empty()) {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), AreaTransformFunctor());
    total_area = std::accumulate(areas.begin(), areas.end(), 0.0) / polygons.size();
    out << std::fixed << std::setprecision(1) << total_area;
    return;
  } else {
    try {
      size_t num_vertices = std::stoul(arg);
      if (num_vertices < 3) {
        throw std::invalid_argument("");
      }
      std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), SpecificVertexAreaTransformFunctor(num_vertices));
    } catch (...) {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }
  total_area = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << std::fixed << std::setprecision(1) << total_area;
}

void gavrilova::execMax(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  std::string arg;
  in >> arg;
  if (arg == "AREA") {
    const auto& p = *std::max_element(polygons.begin(), polygons.end(), AreaComparator());
    out << std::fixed << std::setprecision(1) << getArea(p);
  } else if (arg == "VERTEXES") {
    const auto& p = *std::max_element(polygons.begin(), polygons.end(), VertexesComparator());
    out << p.points.size();
  } else {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void gavrilova::execMin(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  std::string arg;
  in >> arg;
  if (arg == "AREA") {
    const auto& p = *std::min_element(polygons.begin(), polygons.end(), AreaComparator());
    out << std::fixed << std::setprecision(1) << getArea(p);
  } else if (arg == "VERTEXES") {
    const auto& p = *std::min_element(polygons.begin(), polygons.end(), VertexesComparator());
    out << p.points.size();
  } else {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void gavrilova::execCount(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string arg;
  in >> arg;
  size_t count = 0;
  if (arg == "ODD") {
    count = std::count_if(polygons.begin(), polygons.end(), IsOddPredicate());
  } else if (arg == "EVEN") {
    count = std::count_if(polygons.begin(), polygons.end(), IsEvenPredicate());
  } else {
    try {
      size_t num_vertices = std::stoul(arg);
      if (num_vertices < 3) {
        throw std::invalid_argument("");
      }
      count = std::count_if(polygons.begin(), polygons.end(), HasNVertexesPredicate(num_vertices));
    } catch (...) {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }
  out << count;
}

void gavrilova::execPerms(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon ref_polygon;
  if (!(in >> ref_polygon)) {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  out << std::count_if(polygons.begin(), polygons.end(), IsPermutationPredicate(ref_polygon));
}

void gavrilova::execLessArea(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  Polygon ref_polygon;
  if (!(in >> ref_polygon)) {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  out << std::count_if(polygons.begin(), polygons.end(), IsLessAreaPredicate(ref_polygon));
}
