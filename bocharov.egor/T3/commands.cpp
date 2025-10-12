#include "commands.hpp"
#include <functional>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <string>
#include <map>
#include <streamGuard.hpp>

namespace
{
  using namespace bocharov;
  using Predicate = std::function< bool(const Polygon &) >;

  struct AreaSumCalculator
  {
    Predicate pred;
    double operator()(const std::vector< Polygon > & polygons) const
    {
      std::vector<Polygon> filteredPolygons;
      std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filteredPolygons), pred);

      std::vector< double > areas(filteredPolygons.size());

      std::transform(filteredPolygons.begin(), filteredPolygons.end(), std::back_inserter(areas), getPolygonArea);
      return std::accumulate(areas.begin(), areas.end(), 0.0);
    }
  };

  struct AlwaysTrue
  {
    bool operator()(const Polygon &) const noexcept
    {
      return true;
    }
  };

  struct SequenceCounter
  {
    const Polygon & target;
    size_t maxCount = 0;
    size_t currentCount = 0;

    void process(const Polygon & poly)
    {
      if (poly == target)
      {
        currentCount++;
        maxCount = std::max(maxCount, currentCount);
      }
      else
      {
        currentCount = 0;
      }
    }
  };

  class ToFlag
  {
  public:
    explicit ToFlag(const Polygon & target):
      target_(target)
    {}

    size_t operator()(const Polygon & poly) const
    {
      return poly == target_ ? 1 : 0;
    }
  private:
    const Polygon & target_;
  };

  struct ResetableAdder
  {
    size_t operator()(size_t prev, size_t x) const
    {
      return x ? prev + 1 : 0;
    }
  };

  template<typename Compare>
  void getExtremumArea(std::ostream & out, const std::vector< Polygon > & polygons, Compare comp)
  {
    StreamGuard guard(out);
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getPolygonArea);
    auto extremum_it = std::min_element(areas.begin(), areas.end(), comp);
    out << std::fixed << std::setprecision(1) << *extremum_it;
  }

  void getAreaByPredicate(std::ostream & out, const std::vector< Polygon > & polygons, Predicate pred)
  {
    StreamGuard guard(out);
    AreaSumCalculator calculator{pred};
    double result = calculator(polygons);
    out << std::fixed << std::setprecision(1) << result;
  }

  void getAreaMean(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    if (polygons.empty())
    {
      throw std::runtime_error("ERROR: there are no polygons");
    }
    StreamGuard guard(out);
    AreaSumCalculator calculator{AlwaysTrue{}};
    double result = calculator(polygons) / polygons.size();
    out << std::fixed << std::setprecision(1) << result;
  }

  void getAreaVertexes(std::ostream & out, const std::vector< Polygon > & polygons, size_t num)
  {
    StreamGuard guard(out);
    auto pred = std::bind(hasNVertexes, std::placeholders::_1, num);
    AreaSumCalculator calculator{pred};
    double result = calculator(polygons);
    out << std::fixed << std::setprecision(1) << result;
  }

  void getMaxVertexes(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    StreamGuard guard(out);
    auto result = *std::max_element(polygons.begin(), polygons.end(), compareVertexes);
    out << result.points.size();
  }

  void getMinVertexes(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    StreamGuard guard(out);
    auto result = *std::min_element(polygons.begin(), polygons.end(), compareVertexes);
    out << std::fixed << std::setprecision(1) << result.points.size();
  }

  void getCountEven(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), isEven);
  }

  void getCountOdd(std::ostream & out, const std::vector< Polygon > & polygons)
  {
    out << std::count_if(polygons.cbegin(), polygons.cend(), isOdd);
  }

  void getCountVertexes(std::ostream & out, const std::vector< Polygon > & polygons, size_t num)
  {
    auto pred = std::bind(hasNVertexes, std::placeholders::_1, num);
    out << std::count_if(polygons.cbegin(), polygons.cend(), pred);
  }

  struct RightAngleCheck
  {
    const Polygon & plg;

    bool operator()(size_t ind) const
    {
      size_t n  = plg.points.size();
      auto prev = plg.points[(ind + n - 1) % n];
      auto curr = plg.points[ind];
      auto next = plg.points[(ind + 1) % n];

      return bocharov::scalarProduct(prev, curr, next) == 0;
    }
  };

  bool hasRights(const Polygon & plg)
  {
    std::vector< size_t > inds(plg.points.size());
    std::iota(inds.begin(), inds.end(), 0);

    return std::any_of(inds.begin(), inds.end(), RightAngleCheck{ plg });
  }
}

void bocharov::getArea(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getAreaByPredicate, std::ref(out), std::cref(polygons), isEven);
  subcmds["ODD"] = std::bind(getAreaByPredicate, std::ref(out), std::cref(polygons), isOdd);
  subcmds["MEAN"] = std::bind(getAreaMean, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &)
  {
    size_t num = std::stoul(subcmd);
    if (num < 3)
    {
      throw std::logic_error("ERROR: the polygon consists of at least 3 vertices");
    }
    getAreaVertexes(out, polygons, num);
  }
}

void bocharov::getMax(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("ERROR: there are no polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getExtremumArea< std::greater< double > >, std::ref(out), std::cref(polygons), std::greater< double >());
  subcmds["VERTEXES"] = std::bind(getMaxVertexes, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  subcmds.at(subcmd)();
}

void bocharov::getMin(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("ERROR: there are no polygons");
  }
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getExtremumArea< std::less< double > >, std::ref(out), std::cref(polygons), std::less< double >());
  subcmds["VERTEXES"] = std::bind(getMinVertexes, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  subcmds.at(subcmd)();
}

void bocharov::getCount(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getCountEven, std::ref(out), std::cref(polygons));
  subcmds["ODD"] = std::bind(getCountOdd, std::ref(out), std::cref(polygons));

  std::string subcmd;
  in >> subcmd;
  try
  {
    subcmds.at(subcmd)();
  }
  catch (const std::exception &)
  {
    size_t num = std::stoul(subcmd);
    if (num < 3)
    {
      throw std::logic_error("ERROR: the polygon consists of at least 3 vertices");
    }
    getCountVertexes(out, polygons, num);
  }
}

void bocharov::getMaxSeqCommand(std::istream & in, std::ostream & out, const std::vector< Polygon > & polygons)
{
  Polygon target;
  in >> target;
  if (!in || in.peek() != '\n')
  {
    throw std::logic_error("<INVALID COMMAND>");
  }

  if (polygons.empty())
  {
    out << 0;
    return;
  }

  std::vector< size_t > flags;
  flags.reserve(polygons.size());
  std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(flags), ToFlag(target));

  std::vector< size_t > sequenceLengths(flags.size());
  std::partial_sum(flags.cbegin(), flags.cend(), sequenceLengths.begin(), ResetableAdder{});

  size_t maxCount = *std::max_element(sequenceLengths.cbegin(), sequenceLengths.cend());
  out << maxCount;
}

void bocharov::getRightsCnt(std::ostream & out, const std::vector< Polygon > & polygons)
{
  out << count_if(polygons.cbegin(), polygons.cend(), hasRights);
}
