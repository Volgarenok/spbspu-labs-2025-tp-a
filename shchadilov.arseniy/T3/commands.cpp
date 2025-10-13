#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>
#include "streamGuard.hpp"

namespace
{
  using namespace shchadilov;

  struct IsEven
  {
    bool operator()(const Polygon& poly) const
    {
      return poly.points.size() % 2 == 0;
    }
  };

  struct IsOdd
  {
    bool operator()(const Polygon& poly) const
    {
      return poly.points.size() % 2 != 0;
    }
  };

  struct AreaAccumulator
  {
    double operator()(double sum, const Polygon& poly) const
    {
      return sum + getArea(poly);
    }
  };

  struct VertexCountChecker
  {
    size_t count_;

    VertexCountChecker(size_t cnt) :
      count_(cnt)
    {
    }

    bool operator()(const Polygon& poly) const
    {
      return poly.points.size() == count_;
    }
  };

  struct Angle
  {
    Point a_, b_, c_;
    Angle(const Point& a, const Point& b, const Point& c):
      a_(a),
      b_(b),
      c_(c)
    {}
  };

  struct IsRightAngle
  {
    bool operator()(const Angle& angle) const
    {
      int dx1 = angle.b_.x - angle.a_.x;
      int dy1 = angle.b_.y - angle.a_.y;
      int dx2 = angle.c_.x - angle.b_.x;
      int dy2 = angle.c_.y - angle.b_.y;

      return dx1 * dx2 + dy1 * dy2 == 0;
    }
  };


  class AngleGenerator
  {
    public:
      explicit AngleGenerator(const Polygon& polygon):
        polygon_(polygon),
        currentIndex_(0),
        vertexCount_(polygon.points.size())
      {}

    Angle operator()()
    {
      const auto& points = polygon_.points;
      size_t n = vertexCount_;

      size_t i = currentIndex_;
      size_t next_i =  (i + 1) % n;
      size_t next_next_i = (i + 2) % n;

      const Point& a = points[i];
      const Point& b = points[next_i];
      const Point& c = points[next_next_i];

      currentIndex_++;
      return Angle(a, b, c);
    }

    void reset()
    {
      currentIndex_ = 0;
    }

    bool hasNext() const
    {
      return false;
    }

  private:
    const Polygon& polygon_;
    size_t currentIndex_;
    size_t vertexCount_;
  };

  bool hasRightAngle(const shchadilov::Polygon& poly)
  {
    if (poly.points.size() < 3)
    {
      return false;
    }

    AngleGenerator generator(poly);
    std::vector< Angle > angles;
    angles.reserve(poly.points.size());

    std::generate_n(std::back_inserter(angles), poly.points.size(), generator);

    return std::any_of(angles.begin(), angles.end(), IsRightAngle());

  };

  double accumulateArea(const std::vector< Polygon >& polygons, std::function< bool(const Polygon&) > pred)
  {
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);

    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), getArea);

    return std::accumulate(areas.begin(), areas.end(), 0.0);
  };

  size_t countIf(const std::vector< Polygon >& polygons, std::function<bool(const Polygon&)> pred)
  {
    return std::count_if(polygons.begin(), polygons.end(), pred);
  }

}

void shchadilov::printArea(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
{
  std::string param;
  in >> param;
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);

  if (param == "EVEN")
  {
    out << accumulateArea(polygons, IsEven());
  }
  else if (param == "ODD")
  {
    out << accumulateArea(polygons, IsOdd());
  }
  else if (param == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("No polygons for MEAN");
    }
    std::vector< double > allAreas;
    allAreas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(allAreas), getArea);
    double totalArea = std::accumulate(allAreas.begin(), allAreas.end(), 0.0);
    out << totalArea / polygons.size();
  }
  else
  {
    size_t num = std::stoul(param);
    if (num < 3)
    {
      throw std::invalid_argument("Invalid vertex count");
    }
    out << accumulateArea(polygons, VertexCountChecker(num));
  }
}

struct AreaComparator
{
  bool operator()(const shchadilov::Polygon& a, const shchadilov::Polygon& b) const
  {
    return getArea(a) < getArea(b);
  }
};

struct VertexCountComparator
{
  bool operator()(const shchadilov::Polygon& a, const shchadilov::Polygon& b) const
  {
    return a.points.size() < b.points.size();
  }
};

void shchadilov::printMax(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
{
  std::string param;
  in >> param;
  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons for MAX");
  }

  if (param == "AREA")
  {
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);
    AreaComparator comparator;
    auto it = std::max_element(polygons.begin(), polygons.end(), comparator);
    out << getArea(*it);
  }
  else if (param == "VERTEXES")
  {
    VertexCountComparator comparator;
    auto it = std::max_element(polygons.begin(), polygons.end(), comparator);
    out << it->points.size();
  }
  else
  {
    throw std::invalid_argument("Invalid MAX parameter");
  }
}

void shchadilov::printMin(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
{
  std::string param;
  in >> param;
  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons for MIN");
  }

  if (param == "AREA")
  {
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);
    AreaComparator comparator;
    auto it = std::min_element(polygons.begin(), polygons.end(), comparator);
    out << getArea(*it);
  }
  else if (param == "VERTEXES")
  {
    VertexCountComparator comparator;
    auto it = std::min_element(polygons.begin(), polygons.end(), comparator);
    out << it->points.size();
  }
  else
  {
    throw std::invalid_argument("Invalid MIN parameter");
  }
}

void shchadilov::printCount(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons)
{
  std::string param;
  in >> param;

  if (param == "EVEN")
  {
    out << countIf(polygons, IsEven());
  }
  else if (param == "ODD")
  {
    out << countIf(polygons, IsOdd());
  }
  else
  {
    size_t num = std::stoul(param);
    if (num < 3)
    {
      throw std::invalid_argument("Invalid vertex count");
    }
    out << countIf(polygons, VertexCountChecker(num));
  }
}

void shchadilov::printRights(std::ostream& out, const std::vector<Polygon>& polygons)
{
  size_t count = std::count_if(polygons.begin(), polygons.end(), hasRightAngle);
  out << count;
}

struct PolygonEqual
{
  bool operator()(const shchadilov::Polygon& a, const shchadilov::Polygon& b) const
  {
    return a == b;
  }
};

struct TargetDuplicate
{
  Polygon target_;
  PolygonEqual eq_;

  TargetDuplicate(const Polygon& target, const PolygonEqual& eq)
    : target_(target), eq_(eq)
  {}
  bool operator()(const Polygon& a, const Polygon& b) const
  {
    return eq_(a, target_) && eq_(b, target_);
  }
};

void shchadilov::printRmEcho(std::istream& in, std::ostream& out, std::vector<Polygon>& polygons)
{
  Polygon target;
  if (!(in >> target))
  {
    throw std::invalid_argument("Invalid polygon for RMECHO");
  }

  TargetDuplicate pred(target, PolygonEqual{});
  auto newEnd = std::unique(polygons.begin(), polygons.end(), pred);

  size_t removed = std::distance(newEnd, polygons.end());
  polygons.erase(newEnd, polygons.end());

  out << removed;
}
