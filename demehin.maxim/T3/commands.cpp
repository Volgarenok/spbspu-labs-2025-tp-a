#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
#include <unordered_map>
#include <scope_guard.hpp>
#include "geometry.hpp"

namespace
{
  using demehin::Polygon;

  bool isEvenVrts(const Polygon& plg)
  {
    return plg.points.size() % 2 == 0;
  }

  bool isOddVrts(const Polygon& plg)
  {
    return !(isEvenVrts(plg));
  }

  struct VrtCntCheck
  {
    size_t cnt;

    bool operator()(const Polygon& plg) const
    {
      return plg.points.size() == cnt;
    }
  };

  bool compareVrtCnt(const Polygon& plg1, const Polygon& plg2)
  {
    return plg1.points.size() < plg2.points.size();
  }

  bool compareArea(const Polygon& plg1, const Polygon& plg2)
  {
    return getPlgArea(plg1) < getPlgArea(plg2);
  }

  bool isPerm(const Polygon& plg1, const Polygon& plg2)
  {
    if (plg1.points.size() != plg2.points.size())
    {
      return false;
    }
    return std::is_permutation(plg1.points.cbegin(), plg1.points.cend(), plg2.points.cbegin());
  }

  struct RightAngleCheck
  {
    const demehin::Polygon& plg;

    bool operator()(size_t ind) const
    {
      size_t n  = plg.points.size();
      auto prev = plg.points[(ind + n - 1) % n];
      auto curr = plg.points[ind];
      auto next = plg.points[(ind + 1) % n];

      double dx1 = curr.x - prev.x;
      double dy1 = curr.y - prev.y;
      double dx2 = next.x - curr.x;
      double dy2 = next.y - curr.y;

      double scalar = dx1 * dx2 + dy1 * dy2;
      return scalar == 0;
    }
  };

  bool hasRights(const Polygon& plg)
  {
    std::vector< size_t > inds(plg.points.size());
    std::iota(inds.begin(), inds.end(), 0);

    return std::any_of(inds.begin(), inds.end(), RightAngleCheck{ plg });
  }

  size_t countEven(const std::vector< Polygon >& plgs)
  {
    return std::count_if(plgs.cbegin(), plgs.cend(), isEvenVrts);
  }

  size_t countOdd(const std::vector< Polygon >& plgs)
  {
    return std::count_if(plgs.cbegin(), plgs.cend(), isOddVrts);
  }

  size_t countVrt(const std::vector< Polygon >& plgs, size_t vrt_cnt)
  {
    return std::count_if(plgs.cbegin(), plgs.cend(), VrtCntCheck{ vrt_cnt });
  }

  using Predicate = std::function< bool(const Polygon&) >;

  std::vector< double > getAreasIf(const std::vector< Polygon >& plgs, const Predicate& pred)
  {
    std::vector< double > areas;
    std::vector< Polygon > filteredPlgs;
    std::copy_if(plgs.begin(), plgs.end(), std::back_inserter(filteredPlgs), pred);
    std::transform(filteredPlgs.begin(), filteredPlgs.end(), std::back_inserter(areas), demehin::getPlgArea);

    return areas;
  }

  double sumAreaIf(const std::vector< Polygon >& plgs, const Predicate& pred)
  {
    auto areas = getAreasIf(plgs, pred);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double sumAreaEven(const std::vector< Polygon >& plgs)
  {
    return sumAreaIf(plgs, isEvenVrts);
  }

  double sumAreaOdd(const std::vector< Polygon >& plgs)
  {
    return sumAreaIf(plgs, isOddVrts);
  }

  bool noFilter(const Polygon&)
  {
    return true;
  }

  double sumAreaAll(const std::vector< Polygon >& plgs)
  {
    return sumAreaIf(plgs, noFilter);
  }

  double sumAreaVrt(const std::vector< Polygon >& plgs, size_t vrt_cnt)
  {
    return sumAreaIf(plgs, VrtCntCheck{ vrt_cnt });
  }

  struct MeanCalc
  {
    double operator()(const std::vector< Polygon >& plgs) const
    {
      if (plgs.empty())
      {
        throw std::invalid_argument("not enough shapes");
      }
      return sumAreaAll(plgs) / plgs.size();
    }
  };

  void printMaxVrt(std::ostream& out, const std::vector< Polygon >& plgs)
  {
    auto plg = *std::max_element(plgs.begin(), plgs.end(), compareVrtCnt);
    out << plg.points.size();
  }

  void printMaxArea(std::ostream& out, const std::vector< Polygon >& plgs)
  {
    auto plg = *std::max_element(plgs.begin(), plgs.end(), compareArea);
    out << std::setprecision(1) << std::fixed << demehin::getPlgArea(plg);
  }

  void printMinVrt(std::ostream& out, const std::vector< Polygon >& plgs)
  {
    auto plg = *std::min_element(plgs.begin(), plgs.end(), compareVrtCnt);
    out << plg.points.size();
  }

  void printMinArea(std::ostream& out, const std::vector< Polygon >& plgs)
  {
    auto plg = *std::min_element(plgs.begin(), plgs.end(), compareArea);
    out << std::setprecision(1) << std::fixed << demehin::getPlgArea(plg);
  }
}

void demehin::printAreaSum(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  std::unordered_map< std::string, std::function< double() > > subcmds;
  subcmds["EVEN"] = std::bind(sumAreaEven, std::cref(plgs));
  subcmds["ODD"] = std::bind(sumAreaOdd, std::cref(plgs));
  subcmds["MEAN"] = std::bind(MeanCalc{ }, std::cref(plgs));

  std::string subcommand;
  in >> subcommand;
  double res;

  try
  {
    res = subcmds.at(subcommand)();
  }
  catch (...)
  {
    size_t vrt_cnt = std::stoull(subcommand);
    if (vrt_cnt < 3)
    {
      throw std::invalid_argument("not enough vertexes");
    }
    res = sumAreaVrt(plgs, vrt_cnt);
  }

  iofmtguard fmtguard(out);
  out << std::setprecision(1) << std::fixed << res;
}

void demehin::printMaxValueOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  if (plgs.size() == 0)
  {
    throw std::invalid_argument("not enough shapes");
  }

  std::unordered_map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(printMaxArea, std::ref(out), std::cref(plgs));
  subcmds["VERTEXES"] = std::bind(printMaxVrt, std::ref(out), std::cref(plgs));

  std::string subcommand;
  in >> subcommand;

  subcmds.at(subcommand)();
}

void demehin::printMinValueOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  if (plgs.size() == 0)
  {
    throw std::invalid_argument("not enough shapes");
  }

  std::unordered_map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(printMinArea, std::ref(out), std::cref(plgs));
  subcmds["VERTEXES"] = std::bind(printMinVrt, std::ref(out), std::cref(plgs));

  std::string subcommand;
  in >> subcommand;

  subcmds.at(subcommand)();
}

void demehin::printCountOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  std::unordered_map< std::string, std::function< size_t() > > subcmds;
  subcmds["EVEN"] = std::bind(countEven, std::cref(plgs));
  subcmds["ODD"] = std::bind(countOdd, std::cref(plgs));

  std::string subcommand;
  in >> subcommand;
  size_t cnt;
  try
  {
    cnt = subcmds.at(subcommand)();
  }
  catch (...)
  {
    size_t vrt_cnt = std::stoull(subcommand);
    if (vrt_cnt < 3)
    {
      throw std::invalid_argument("wrong parameter");
    }
    cnt = countVrt(plgs, vrt_cnt);
  }

  out << cnt;
}

void demehin::printPermsCnt(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  Polygon plg;
  in >> plg;
  if (plg.points.size() < 3)
  {
    throw std::invalid_argument("incorrect shape");
  }
  out << std::count_if(plgs.cbegin(), plgs.cend(), std::bind(isPerm, std::placeholders::_1, plg));
}

void demehin::printRightsCnt(const std::vector< Polygon >& plgs, std::ostream& out)
{
  out << count_if(plgs.cbegin(), plgs.cend(), hasRights);
}
