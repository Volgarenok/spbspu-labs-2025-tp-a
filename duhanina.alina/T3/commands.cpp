#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <map>
#include <stdexcept>

namespace
{
  using duhanina::Polygon;

  struct AreaAdder
  {
    double operator()(double sum, const Polygon& poly) const
    {
      return sum + calculateArea(poly);
    }
  };

  struct EvenVertexCounter
  {
    bool operator()(const Polygon& poly) const
    {
      return poly.points.size() % 2 == 0;
    }
  };

  struct OddVertexCounter
  {
    bool operator()(const Polygon& poly) const
    {
      return poly.points.size() % 2 != 0;
    }
  };

  struct EvenAreaAdder
  {
    double operator()(double sum, const Polygon& poly) const
    {
      EvenVertexCounter isEven;
      if (isEven(poly))
      {
        return AreaAdder()(sum, poly);
      }
      return sum;
    }
  };

  struct OddAreaAdder
  {
    double operator()(double sum, const Polygon& poly) const
    {
      OddVertexCounter isOdd;
      if (isOdd(poly))
      {
        return AreaAdder()(sum, poly);
      }
      return sum;
    }
  };

 struct ExactVertexCounter
  {
    size_t num;
    ExactVertexCounter(size_t n):
      num(n)
    {}

    bool operator()(const Polygon& poly) const
    {
      return poly.points.size() == num;
    }
  };

  struct VertexCountAreaAdder
  {
    size_t num;
    VertexCountAreaAdder(size_t n):
      num(n)
    {}

    double operator()(double sum, const Polygon& poly) const
    {
      if (ExactVertexCounter(num)(poly))
      {
        return AreaAdder()(sum, poly);
      }
      return sum;
    }
  };

  struct VertexCountAreaSum
  {
    size_t num;
    explicit VertexCountAreaSum(size_t n):
      num(n)
    {}

    double operator()(const std::vector< Polygon >& polygons) const
    {
      return std::accumulate(polygons.begin(), polygons.end(), 0.0, VertexCountAreaAdder(num));
    }
  };

  struct AreaComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return calculateArea(a) < calculateArea(b);
    }
  };

  struct VertexCountComparator
  {
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return a.points.size() < b.points.size();
    }
  };

  struct EvenAreaSum
  {
    double operator()(const std::vector< Polygon >& plgs) const
    {
      return std::accumulate(plgs.begin(), plgs.end(), 0.0, EvenAreaAdder());
    }
  };

  struct OddAreaSum
  {
    double operator()(const std::vector< Polygon >& plgs) const
    {
      return std::accumulate(plgs.begin(), plgs.end(), 0.0, OddAreaAdder());
    }
  };

  struct MeanAreaSum
  {
    double operator()(const std::vector< Polygon >& plgs) const
    {
      if (plgs.empty())
      {
        throw std::runtime_error("Error");
      }
      return std::accumulate(plgs.begin(), plgs.end(), 0.0, AreaAdder()) / plgs.size();
    }
  };

  struct MaxArea
  {
    void operator()(const std::vector< Polygon >& plgs, std::ostream& out) const
    {
      if (plgs.empty())
      {
        throw std::runtime_error("Error");
      }
      auto it = std::max_element(plgs.begin(), plgs.end(), AreaComparator());
      out << calculateArea(*it);
    }
  };

  struct MaxVertexCount
  {
    void operator()(const std::vector< Polygon >& plgs, std::ostream& out) const
    {
      if (plgs.empty())
      {
        throw std::runtime_error("Error");
      }
      auto it = std::max_element(plgs.begin(), plgs.end(), VertexCountComparator());
      out << it->points.size();
    }
  };

  struct MinArea
  {
    void operator()(const std::vector< Polygon >& plgs, std::ostream& out) const
    {
      if (plgs.empty())
      {
        throw std::runtime_error("Error");
      }
      auto it = std::min_element(plgs.begin(), plgs.end(), AreaComparator());
      out << calculateArea(*it);
    }
  };

  struct MinVertexCount
  {
    void operator()(const std::vector< Polygon >& plgs, std::ostream& out) const
    {
      if (plgs.empty())
      {
        throw std::runtime_error("Error");
      }
      auto it = std::min_element(plgs.begin(), plgs.end(), VertexCountComparator());
      out << it->points.size();
    }
  };

  struct CountEven
  {
    size_t operator()(const std::vector< Polygon >& plgs) const
    {
      return std::count_if(plgs.begin(), plgs.end(), EvenVertexCounter());
    }
  };

  struct CountOdd
  {
    size_t operator()(const std::vector< Polygon >& plgs) const
    {
      return std::count_if(plgs.begin(), plgs.end(), OddVertexCounter());
    }
  };

  struct CountIfAreaLess
  {
    double threshold;
    explicit CountIfAreaLess(double t):
      threshold(t)
    {}

    bool operator()(const Polygon& p) const
    {
      return calculateArea(p) < threshold;
    }
  };

  struct AdjacentDuplicateFinder
  {
    const Polygon& ref;
    bool& first;

    bool operator()(const Polygon& a, const Polygon& b) const
    {
      if (a == ref && b == ref)
      {
        bool result = !first;
        first = false;
        return result;
      }
      first = true;
      return false;
    }
  };
}

void duhanina::printAreaSum(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  std::string param;
  in >> param;
  out << std::fixed << std::setprecision(1);
  std::map< std::string, std::function< double(const std::vector< Polygon >&) > > commands;
  commands["EVEN"] = EvenAreaSum();
  commands["ODD"]  = OddAreaSum();
  commands["MEAN"] = MeanAreaSum();
  if (std::isdigit(param[0]))
  {
    size_t num = std::stoull(param);
    if (num < 3)
    {
      throw std::invalid_argument("Error");
    }
    commands[param] = VertexCountAreaSum(num);
  }
  out << commands.at(param)(plgs);
}

void duhanina::printMaxValue(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  if (plgs.empty())
  {
    throw std::invalid_argument("Error");
  }
  std::string param;
  in >> param;
  out << std::fixed << std::setprecision(1);
  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream& out) > > commands;
  commands["AREA"] = MaxArea();
  commands["VERTEXES"] = MaxVertexCount();
  commands.at(param)(plgs, out);
}

void duhanina::printMinValue(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  if (plgs.empty())
  {
    throw std::invalid_argument("Error");
  }
  std::string param;
  in >> param;
  out << std::fixed << std::setprecision(1);
  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream& out) > > commands;
  commands["AREA"] = MinArea();
  commands["VERTEXES"] = MinVertexCount();
  commands.at(param)(plgs, out);
}

void duhanina::printCount(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  std::string param;
  in >> param;
  std::map< std::string, std::function< size_t(const std::vector< Polygon >&) > > commands;
  commands["EVEN"] = CountEven();
  commands["ODD"] = CountOdd();
  if (std::isdigit(param[0]))
  {
    size_t num = std::stoull(param);
    if (num < 3)
    {
      throw std::invalid_argument("Error");
    }
    out << std::count_if(plgs.begin(), plgs.end(), ExactVertexCounter(num));
    return;
  }
  out << commands.at(param)(plgs);
}

void duhanina::printLessArea(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("Error");
  }
  Polygon ref;
  in >> ref;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::invalid_argument("Error");
  }
  double refArea = calculateArea(ref);
  size_t count = std::count_if(polygons.begin(), polygons.end(), CountIfAreaLess(refArea));
  out << count;
}

void duhanina::printRmecho(std::istream& in, std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("Error");
  }
  Polygon ref;
  in >> ref;
  bool first_flag = true;
  auto new_end = std::unique(polygons.begin(), polygons.end(), AdjacentDuplicateFinder{ ref, first_flag });
  size_t removed_count = std::distance(new_end, polygons.end());
  polygons.erase(new_end, polygons.end());
  out << removed_count;
}
