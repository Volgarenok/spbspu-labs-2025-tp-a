#include <algorithm>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>
#include <numeric>
#include <iterator>
#include <cctype>
#include "streamGuard.hpp"
#include "commands.hpp"

namespace ivanova
{
  class EvenAreaSum
  {
  public:
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3 && poly.points.size() % 2 == 0)
      {
        sum += calculateArea(poly);
      }
    }
    double getSum() const
    {
      return sum;
    }
  private:
    double sum = 0.0;
  };

  class OddAreaSum
  {
  public:
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3 && poly.points.size() % 2 == 1)
      {
        sum += calculateArea(poly);
      }
    }
    double getSum() const
    {
      return sum;
    }
  private:
    double sum = 0.0;
  };

  class NumVerticesAreaSum
  {
  public:
    explicit NumVerticesAreaSum(size_t n):
      num(n)
    {}
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3 && poly.points.size() == num)
      {
        sum += calculateArea(poly);
      }
    }
    double getSum() const
    {
      return sum;
    }
  private:
    size_t num;
    double sum = 0.0;
  };

  class TotalAreaSum
  {
  public:
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3)
      {
        sum += calculateArea(poly);
      }
    }
    double getSum() const
    {
      return sum;
    }
  private:
    double sum = 0.0;
  };

  class AreaComparator
  {
  public:
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return calculateArea(a) < calculateArea(b);
    }
  };

  class VertexCountComparator
  {
  public:
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      return a.points.size() < b.points.size();
    }
  };

  class EvenCount
  {
  public:
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3 && poly.points.size() % 2 == 0)
      {
        count++;
      }
    }
    size_t getCount() const
    {
      return count;
    }
  private:
    size_t count = 0;
  };

  class OddCount
  {
  public:
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3 && poly.points.size() % 2 == 1)
      {
        count++;
      }
    }
    size_t getCount() const
    {
      return count;
    }
  private:
    size_t count = 0;
  };

  class NumVerticesCount
  {
  public:
    explicit NumVerticesCount(size_t n):
      num(n)
    {}
    void operator()(const Polygon& poly)
    {
      if (poly.points.size() >= 3 && poly.points.size() == num)
      {
        count++;
      }
    }
    size_t getCount() const
    {
      return count;
    }
  private:
    size_t num;
    size_t count = 0;
  };

  class IsDigit
  {
  public:
    bool operator()(char c) const
    {
      return std::isdigit(static_cast<unsigned char>(c));
    }
  };

  class AccumulateDigit
  {
  public:
    size_t operator()(size_t acc, char c) const
    {
      return acc * 10 + (c - '0');
    }
  };

  class ValidPolygonFilter
  {
  public:
    bool operator()(const Polygon& poly) const
    {
      return poly.points.size() >= 3;
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
      out << accumulator.getSum() << '\n';
    }
    else if (param == "ODD")
    {
      OddAreaSum accumulator;
      std::for_each(polygons.begin(), polygons.end(), std::ref(accumulator));
      out << accumulator.getSum() << '\n';
    }
    else if (param == "MEAN")
    {
      std::vector< Polygon > validPolygons;
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(validPolygons), ValidPolygonFilter());

      if (validPolygons.empty())
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      TotalAreaSum accumulator;
      std::for_each(validPolygons.begin(), validPolygons.end(), std::ref(accumulator));
      out << (accumulator.getSum() / validPolygons.size()) << '\n';
    }
    else
    {
      if (!std::all_of(param.begin(), param.end(), IsDigit()))
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      size_t numVertices = std::accumulate(param.begin(), param.end(), 0UL, AccumulateDigit());

      if (numVertices < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      NumVerticesAreaSum accumulator(numVertices);
      std::for_each(polygons.begin(), polygons.end(), std::ref(accumulator));
      out << accumulator.getSum() << '\n';
    }
  }

  void max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string param;
    in >> param;

    std::vector< Polygon > validPolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(validPolygons), ValidPolygonFilter());

    if (validPolygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    StreamGuard guard(out);

    if (param == "AREA")
    {
      auto maxIt = std::max_element(validPolygons.begin(), validPolygons.end(), AreaComparator());
      out << std::fixed << std::setprecision(1) << calculateArea(*maxIt) << '\n';
    }
    else if (param == "VERTEXES")
    {
      auto maxIt = std::max_element(validPolygons.begin(), validPolygons.end(), VertexCountComparator());
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

    std::vector< Polygon > validPolygons;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(validPolygons), ValidPolygonFilter());

    if (validPolygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "AREA")
    {
      auto minIt = std::min_element(validPolygons.begin(), validPolygons.end(), AreaComparator());
      out << calculateArea(*minIt) << '\n';
    }
    else if (param == "VERTEXES")
    {
      auto minIt = std::min_element(validPolygons.begin(), validPolygons.end(), VertexCountComparator());
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
      out << counter.getCount() << '\n';
    }
    else if (param == "ODD")
    {
      OddCount counter;
      std::for_each(polygons.begin(), polygons.end(), std::ref(counter));
      out << counter.getCount() << '\n';
    }
    else
    {
      if (!std::all_of(param.begin(), param.end(), IsDigit()))
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      size_t numVertices = std::accumulate(param.begin(), param.end(), 0UL, AccumulateDigit());

      if (numVertices < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      NumVerticesCount counter(numVertices);
      std::for_each(polygons.begin(), polygons.end(), std::ref(counter));
      out << counter.getCount() << '\n';
    }
  }

  class EchoProcessor
  {
  public:
    EchoProcessor(const Polygon& target, std::vector< Polygon >& result):
      target_(target),
      result_(result),
      count_(0)
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
    size_t getCount() const
    {
      return count_;
    }
  private:
    const Polygon& target_;
    std::vector< Polygon >& result_;
    size_t count_;
  };

  void echo(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons)
  {
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::vector< Polygon > result;
    EchoProcessor processor(target, result);
    std::for_each(polygons.begin(), polygons.end(), std::ref(processor));
    polygons = std::move(result);
    out << processor.getCount() << '\n';
  }

  class DistanceCalculator
  {
  public:
    double operator()(const Point& a, const Point& b) const
    {
      double dx = b.x - a.x;
      double dy = b.y - a.y;
      return std::sqrt(dx * dx + dy * dy);
    }
  };

  class DoubleApproxEqual
  {
  public:
    bool operator()(double d1, double d2) const
    {
      return std::abs(d1 - d2) < 1e-6;
    }
  };

  class IndexedDistanceCalculator
  {
  public:
    IndexedDistanceCalculator(const Polygon& poly, size_t idx):
      poly_(poly),
      index_(idx)
    {}
    double operator()() const
    {
      size_t next_index = (index_ + 1) % poly_.points.size();
      DistanceCalculator dist_calc;
      return dist_calc(poly_.points[index_], poly_.points[next_index]);
    }
  private:
    const Polygon& poly_;
    size_t index_;
  };

  class PolygonDistanceGenerator
  {
  public:
    explicit PolygonDistanceGenerator(const Polygon& poly):
      poly_(poly)
    {}
    
    class Iterator
    {
    public:
      Iterator(const Polygon& poly, size_t index):
        poly_(poly),
        index_(index)
      {}
      
      double operator*() const
      {
        IndexedDistanceCalculator calc(poly_, index_);
        return calc();
      }
      
      Iterator& operator++()
      {
        index_++;
        return *this;
      }
      
      bool operator!=(const Iterator& other) const
      {
        return index_ != other.index_;
      }
      
    private:
      const Polygon& poly_;
      size_t index_;
    };
    
    Iterator begin() const
    {
      return Iterator(poly_, 0);
    }
    
    Iterator end() const
    {
      return Iterator(poly_, poly_.points.size());
    }
    
  private:
    const Polygon& poly_;
  };

  class CompatibilityChecker
  {
  public:
    bool operator()(const Polygon& a, const Polygon& b) const
    {
      if (a.points.size() != b.points.size())
      {
        return false;
      }

      std::vector< double > a_distances;
      std::vector< double > b_distances;

      PolygonDistanceGenerator a_generator(a);
      PolygonDistanceGenerator b_generator(b);

      std::copy(a_generator.begin(), a_generator.end(), std::back_inserter(a_distances));
      std::copy(b_generator.begin(), b_generator.end(), std::back_inserter(b_distances));

      return std::search(a_distances.begin(), a_distances.end(),
        b_distances.begin(), b_distances.end(),
        DoubleApproxEqual()) != a_distances.end();
    }
  };

  class SameCounter
  {
  public:
    explicit SameCounter(const Polygon& target):
      target_(target),
      count_(0)
    {}
    void operator()(const Polygon& poly)
    {
      CompatibilityChecker checker;
      if (checker(poly, target_))
      {
        count_++;
      }
    }
    size_t getCount() const
    {
      return count_;
    }
  private:
    const Polygon target_;
    size_t count_;
  };

  void same(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    SameCounter counter(target);
    std::for_each(polygons.begin(), polygons.end(), std::ref(counter));
    out << counter.getCount() << '\n';
  }

  void inframe(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  void intersections(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  void maxseq(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  void perms(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  void rects(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  void rightshapes(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }

  void rmecho(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons)
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}
