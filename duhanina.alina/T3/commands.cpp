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
    ExactVertexCounter(size_t n):
      num(n)
    {}

    bool operator()(const Polygon& poly) const
    {
      return poly.points.size() == num;
    }

  private:
    size_t num;
  };

  struct VertexCountAreaAdder
  {
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

  private:
    size_t num;
  };

  struct VertexCountAreaSum
  {
    explicit VertexCountAreaSum(size_t n):
      num(n)
    {}

    double operator()(const std::vector< Polygon >& polygons) const
    {
      return calculateRecursive(polygons, 0, 0.0);
    }

  private:
    size_t num;
    double calculateRecursive(const std::vector< Polygon >& polygons, size_t index, double sum) const
    {
      if (index >= polygons.size())
      {
        return sum;
      }
      const Polygon& poly = polygons[index];
      double currentSum = sum;
      if (poly.points.size() == num)
      {
        currentSum += calculateArea(poly);
      }
      return calculateRecursive(polygons, index + 1, currentSum);
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
      return calculateRecursive(plgs, 0, 0.0);
    }
  private:
     double calculateRecursive(const std::vector< Polygon >& polygons, size_t index, double sum) const
     {
       if (index >= polygons.size())
       {
         return sum;
       }
       const Polygon& poly = polygons[index];
       double currentSum = sum;
       if (poly.points.size() % 2 == 0)
       {
         currentSum += calculateArea(poly);
       }
       return calculateRecursive(polygons, index + 1, currentSum);
     }
  };

  struct OddAreaSum
  {
    double operator()(const std::vector< Polygon >& plgs) const
    {
      return calculateRecursive(plgs, 0, 0.0);
    }
  private:
     double calculateRecursive(const std::vector< Polygon >& polygons, size_t index, double sum) const
     {
       if (index >= polygons.size())
       {
         return sum;
       }
       const Polygon& poly = polygons[index];
       double currentSum = sum;
       if (poly.points.size() % 2 != 0)
       {
         currentSum += calculateArea(poly);
       }
       return calculateRecursive(polygons, index + 1, currentSum);
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
      double total = calculateRecursive(plgs, 0, 0.0);
      return total / plgs.size();
    }
  private:
    double calculateRecursive(const std::vector< Polygon >& polygons, size_t index, double sum) const
    {
      if (index >= polygons.size())
      {
        return sum;
      }
      return calculateRecursive(polygons, index + 1, sum + calculateArea(polygons[index]));
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
    explicit CountIfAreaLess(double t):
      threshold(t)
    {}

    bool operator()(const Polygon& p) const
    {
      return calculateArea(p) < threshold;
    }

  private:
    double threshold;
  };

  struct AdjacentDuplicateFinder
  {
    AdjacentDuplicateFinder(const Polygon& p, bool& f):
      ref(p),
      first(f)
    {}

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

  private:
    const Polygon& ref;
    bool& first;
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
  try
  {
    out << commands.at(param)(plgs);
  }
  catch (...)
  {
    size_t num = std::stoull(param);
    if (num < 3)
    {
      throw std::invalid_argument("Error in input");
    }
    out << VertexCountAreaSum(num)(plgs);
  }
}

void duhanina::printMaxValue(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  if (plgs.empty())
  {
    throw std::invalid_argument("Empty polygons");
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
    throw std::invalid_argument("Empty polygons");
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
  try
  {
    out << commands.at(param)(plgs);
  }
  catch (...)
  {
    size_t num = std::stoull(param);
    if (num < 3)
    {
      throw std::invalid_argument("Error in input");
    }
    out << std::count_if(plgs.begin(), plgs.end(), ExactVertexCounter(num));
  }
}

void duhanina::printLessArea(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("Empty polygons");
  }
  Polygon ref;
  in >> ref;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::invalid_argument("Error in input");
  }
  double refArea = calculateArea(ref);
  size_t count = std::count_if(polygons.begin(), polygons.end(), CountIfAreaLess(refArea));
  out << count;
}

void duhanina::eraseRmecho(std::istream& in, std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("Empty polygons");
  }
  Polygon ref;
  in >> ref;
  bool first_flag = true;
  auto new_end = std::unique(polygons.begin(), polygons.end(), AdjacentDuplicateFinder{ ref, first_flag });
  size_t removed_count = std::distance(new_end, polygons.end());
  polygons.erase(new_end, polygons.end());
  out << removed_count;
}
