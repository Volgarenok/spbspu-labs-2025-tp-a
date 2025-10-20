#include <algorithm>
#include <iomanip>
#include <string>
#include <cmath>
#include <sstream>
#include <functional>
#include <numeric>
#include "streamGuard.hpp"
#include "commands.hpp"

namespace ivanova
{
  struct EvenAreaSum
  {
    double sum = 0.0;
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() % 2 == 0)
      {
        sum += calculateArea(poly);
      }
    }
  };

  struct OddAreaSum
  {
    double sum = 0.0;
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() % 2 == 1)
      {
        sum += calculateArea(poly);
      }
    }
  };

  struct NumVerticesAreaSum
  {
    size_t num;
    double sum = 0.0;
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() == num)
      {
        sum += calculateArea(poly);
      }
    }
  };

  struct TotalAreaSum
  {
    double sum = 0.0;
    void operator()(const Polygon& poly)
    {
      sum += calculateArea(poly);
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

  struct EvenCount
  {
    size_t count = 0;
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3 && poly.points.size() % 2 == 0)
      {
        count++;
      }
    }
  };

  struct OddCount
  {
    size_t count = 0;
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3 && poly.points.size() % 2 == 1)
      {
        count++;
      }
    }
  };

  struct NumVerticesCount
  {
    size_t num;
    size_t count = 0;
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3 && poly.points.size() == num)
      {
        count++;
      }
    }
  };

  void area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string param;
    in >> param;

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "EVEN")
    {
      EvenAreaSum accumulator;
      std::for_each(polygons.begin(), polygons.end(), std::ref(accumulator));
      out << accumulator.sum << '\n';
    }
    else if (param == "ODD")
    {
      OddAreaSum accumulator;
      std::for_each(polygons.begin(), polygons.end(), std::ref(accumulator));
      out << accumulator.sum << '\n';
    }
    else if (param == "MEAN")
    {
      if (polygons.empty())
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      TotalAreaSum accumulator;
      std::for_each(polygons.begin(), polygons.end(), std::ref(accumulator));
      out << (accumulator.sum / polygons.size()) << '\n';
    }
    else
    {
      size_t numVertices = 0;
      try
      {
        numVertices = std::stoul(param);
      }
      catch (const std::invalid_argument&)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      catch (const std::out_of_range&)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      
      if (numVertices < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      
      NumVerticesAreaSum accumulator{numVertices};
      std::for_each(polygons.begin(), polygons.end(), std::ref(accumulator));
      out << accumulator.sum << '\n';
    }
  }

  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string param;
    in >> param;

    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    StreamGuard guard(out);

    if (param == "AREA")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), AreaComparator());
      out << std::fixed << std::setprecision(1) << calculateArea(*maxIt) << '\n';
    }
    else if (param == "VERTEXES")
    {
      auto maxIt = std::max_element(polygons.begin(), polygons.end(), VertexCountComparator());
      out << maxIt->points.size() << '\n';
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string param;
    in >> param;

    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "AREA")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), AreaComparator());
      out << calculateArea(*minIt) << '\n';
    }
    else if (param == "VERTEXES")
    {
      auto minIt = std::min_element(polygons.begin(), polygons.end(), VertexCountComparator());
      out << minIt->points.size() << '\n';
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string param;
    in >> param;

    if (param == "EVEN")
    {
      EvenCount counter;
      std::for_each(polygons.begin(), polygons.end(), std::ref(counter));
      out << counter.count << '\n';
    }
    else if (param == "ODD")
    {
      OddCount counter;
      std::for_each(polygons.begin(), polygons.end(), std::ref(counter));
      out << counter.count << '\n';
    }
    else
    {
      size_t numVertices = 0;
      try
      {
        numVertices = std::stoul(param);
      }
      catch (const std::invalid_argument&)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      catch (const std::out_of_range&)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      
      if (numVertices < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      
      NumVerticesCount counter{numVertices};
      std::for_each(polygons.begin(), polygons.end(), std::ref(counter));
      out << counter.count << '\n';
    }
  }

  void echo(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons)
  {
    std::string line;
    std::getline(in, line);

    if (line.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::istringstream iss(line);
    Polygon target;
    iss >> target;

    if (!iss || target.points.size() < 3 || !iss.eof())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    struct EchoProcessor
    {
      const Polygon& target_;
      std::vector< Polygon >& result_;
      size_t count_ = 0;

      EchoProcessor(const Polygon& target, std::vector< Polygon >& result):
        target_(target),
        result_(result)
      {}

      void operator()(const Polygon& poly)
      {
        result_.push_back(poly);
        if (poly == target_)
        {
          result_.push_back(target_);
          count_++;
        }
      }
    };

    std::vector< Polygon > result;
    EchoProcessor processor(target, result);
    std::for_each(polygons.begin(), polygons.end(), std::ref(processor));
    polygons = std::move(result);
    out << processor.count_ << '\n';
  }

  double distance(const Point& a, const Point& b)
  {
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
  }

  bool areCompatible(const Polygon& a, const Polygon& b)
  {
    if (a.points.size() != b.points.size())
    {
      return false;
    }

    size_t n = a.points.size();

    for (size_t shift = 0; shift < n; ++shift)
    {
      bool compatible = true;

      for (size_t i = 0; i < n && compatible; ++i)
      {
        size_t j = (i + 1) % n;
        size_t i_shift = (i + shift) % n;
        size_t j_shift = (j + shift) % n;

        double dist_a = distance(a.points[i], a.points[j]);
        double dist_b = distance(b.points[i_shift], b.points[j_shift]);

        if (std::abs(dist_a - dist_b) > 1e-6)
        {
          compatible = false;
        }
      }

      if (compatible)
      {
        return true;
      }
    }

    return false;
  }

  void same(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string line;
    std::getline(in, line);

    if (line.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::istringstream iss(line);
    Polygon target;
    iss >> target;

    if (!iss || target.points.size() < 3 || !iss.eof())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    struct SameCounter
    {
      const Polygon target_;
      size_t count_ = 0;

      SameCounter(const Polygon& target):
        target_(target)
      {}

      void operator()(const Polygon& poly)
      {
        if (areCompatible(poly, target_))
        {
          count_++;
        }
      }
    };

    SameCounter counter(target);
    std::for_each(polygons.begin(), polygons.end(), std::ref(counter));
    out << counter.count_ << '\n';
  }
}
