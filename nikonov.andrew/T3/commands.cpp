#include "commands.hpp"
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <functional>
#include <StreamGuard.hpp>
namespace
{
  struct IsEvenPredicate
  {
    bool operator()(const nikonov::Polygon& poly) const
    {
      return (poly.points.size() % 2) == 0;
    }
  };

  struct IsOddPredicate
  {
    bool operator()(const nikonov::Polygon& poly) const
    {
      return (poly.points.size() % 2) == 1;
    }
  };

  struct AnyPredicate
  {
    bool operator()(const nikonov::Polygon&) const
    {
      return true;
    }
  };

  struct HasVertexCountPredicate
  {
    size_t n;
    explicit HasVertexCountPredicate(std::size_t cnt):
      n(cnt)
    {}
    bool operator()(const nikonov::Polygon& poly) const
    {
      return poly.points.size() == n;
    }
  };

  double gaussArea(const nikonov::Point& a, const nikonov::Point& b)
  {
    return a.x * b.y - a.y * b.x;
  }

  double getPolygonArea(const nikonov::Polygon& poly)
  {
    const auto& pts  = poly.points;
    double sum = std::inner_product(pts.begin(), pts.end() - 1, pts.begin() + 1, 0.0, std::plus< double >{}, gaussArea);
    sum += gaussArea(pts.back(), pts.front());
    return std::abs(sum) / 2.0;
  }

  template < typename Pred >
  double calculateAreaIf(const std::vector< nikonov::Polygon >& data, Pred condition)
  {
    std::vector< nikonov::Polygon > tempPoly;
    std::copy_if(data.begin(), data.end(), std::back_inserter(tempPoly), condition);
    std::vector< double > areas;
    std::transform(tempPoly.begin(), tempPoly.end(), std::back_inserter(areas), getPolygonArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  bool pAreaComparator(const nikonov::Polygon& a, const nikonov::Polygon& b)
  {
    return getPolygonArea(a) < getPolygonArea(b);
  }

  bool pVertexesComparator(const nikonov::Polygon& a, const nikonov::Polygon& b)
  {
    return a.points.size() < b.points.size();
  }

  size_t countEven(const std::vector< nikonov::Polygon >& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), IsEvenPredicate());
  }

  size_t countOdd(const std::vector< nikonov::Polygon >& data)
  {
    return std::count_if(data.begin(), data.end(), IsOddPredicate());
  }

  size_t countNum(const std::vector< nikonov::Polygon >& data, size_t numOfVertexes)
  {
    return std::count_if(data.begin(), data.end(), HasVertexCountPredicate(numOfVertexes));
  }

  bool isPermutation(const nikonov::Polygon& a, const nikonov::Polygon& b)
  {
    if (a.points.size() != b.points.size())
    {
      return false;
    }
    return std::is_permutation(a.points.begin(), a.points.end(), b.points.begin());
  }

  struct SeqState
  {
    std::size_t curr;
    std::size_t max;
    SeqState(size_t c = 0, size_t m = 0):
      curr(c),
      max(m)
    {}
  };

  struct SeqAccumOp
  {
    const nikonov::Polygon& target;
    explicit SeqAccumOp(const nikonov::Polygon& t):
      target(t)
    {}
    SeqState operator()(SeqState acc, const nikonov::Polygon& p) const
    {
      if (p == target)
      {
        ++acc.curr;
        if (acc.curr > acc.max)
        {
          acc.max = acc.curr;
        }
      }
      else
      {
        acc.curr = 0;
      }
      return acc;
    }
  };
}

void nikonov::getArea(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  if (subcommand == "EVEN")
  {
    out << calculateAreaIf(data, IsEvenPredicate());
  }
  else if (subcommand == "ODD")
  {
    out << calculateAreaIf(data, IsOddPredicate());
  }
  else if (subcommand == "MEAN")
  {
    if (data.empty())
    {
      throw std::logic_error("Mean cant be calculated!");
    }
    out << calculateAreaIf(data, AnyPredicate()) / data.size();
  }
  else
  {
    size_t cnt = std::stoul(subcommand);
    if (cnt < 3)
    {
      throw std::invalid_argument("At least 3 vertex needed!");
    }
    out << calculateAreaIf(data, HasVertexCountPredicate(cnt));
  }
}

void nikonov::getMax(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  if (data.empty())
  {
    throw std::logic_error("Empty!");
  }
  std::string subcommand;
  in >> subcommand;
  StreamGuard guard(out);
  if (subcommand == "AREA")
  {
    auto it = std::max_element(data.begin(), data.end(), pAreaComparator);
    out << std::fixed << std::setprecision(1) << getPolygonArea(*it);
  }
  else if (subcommand == "VERTEXES")
  {
    auto it = std::max_element(data.begin(), data.end(), pVertexesComparator);
    out << it->points.size();
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void nikonov::getMin(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  if (data.empty())
  {
    throw std::logic_error("Empty!");
  }
  std::string subcommand;
  in >> subcommand;
  StreamGuard guard(out);
  if (subcommand == "AREA")
  {
    auto it = std::min_element(data.begin(), data.end(), pAreaComparator);
    out << std::fixed << std::setprecision(1) << getPolygonArea(*it);
  }
  else if (subcommand == "VERTEXES")
  {
    auto it = std::min_element(data.begin(), data.end(), pVertexesComparator);
    out << it->points.size();
  }
  else
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
}

void nikonov::getCount(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1);
  if (subcommand == "EVEN")
  {
    out << countEven(data);
  }
  else if (subcommand == "ODD")
  {
    out << countOdd(data);
  }
  else
  {
    size_t cnt = std::stoul(subcommand);
    if (cnt < 3)
    {
      throw std::invalid_argument("At least 3 vertex needed!");
    }
    out << countNum(data, cnt);
  }
}

void nikonov::getPerms(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  Polygon polygon;
  in >> polygon;
  if (polygon.points.size() < 3)
  {
    throw std::logic_error("At least 3 vertex needed!");
  }
  using namespace std::placeholders;
  out << std::count_if(data.begin(), data.end(), std::bind(isPermutation, _1, polygon));
}

void nikonov::getMaxSeq(const std::vector<Polygon>& data, std::istream& in, std::ostream& out)
{
  nikonov::Polygon polygon;
  in >> polygon;
  if (!in || polygon.points.size() < 3)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (polygon.points.size() < 3)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  if (std::count(data.begin(), data.end(), polygon) == 0)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  SeqState init(0, 0);
  SeqAccumOp op(polygon);
  SeqState result = std::accumulate(data.begin(), data.end(), init, op);
  out << result.max;
}
