#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
#include <scope_guard.hpp>
#include "geometry.hpp"

namespace
{
  bool isEvenVrts(const demehin::Polygon& plg)
  {
    return plg.points.size() % 2 == 0;
  }

  bool isOddVrts(const demehin::Polygon& plg)
  {
    return !(isEvenVrts(plg));
  }

  struct VrtCntCheck
  {
    size_t cnt;

    bool operator()(const demehin::Polygon& plg) const
    {
      return plg.points.size() == cnt;
    }
  };

  bool compareVrtCnt(const demehin::Polygon& plg1, const demehin::Polygon& plg2)
  {
    return plg1.points.size() < plg2.points.size();
  }

  bool compareArea(const demehin::Polygon& plg1, const demehin::Polygon& plg2)
  {
    return getPlgArea(plg1) < getPlgArea(plg2);
  }

  bool isPerm(const demehin::Polygon& plg1, const demehin::Polygon& plg2)
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

  bool hasRights(const demehin::Polygon& plg)
  {
    std::vector< size_t > inds(plg.points.size());
    std::iota(inds.begin(), inds.end(), 0);

    return std::any_of(inds.begin(), inds.end(), RightAngleCheck{ plg });
  }
}

void demehin::printAreaSum(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  std::string subcommand;
  in >> subcommand;
  std::vector< Polygon > tmp;
  if (subcommand == "EVEN")
  {
    std::copy_if(plgs.cbegin(), plgs.cend(), std::back_inserter(tmp), isEvenVrts);
  }
  else if (subcommand == "ODD")
  {
    std::copy_if(plgs.cbegin(), plgs.cend(), std::back_inserter(tmp), isOddVrts);
  }
  else if (subcommand == "MEAN")
  {
    if (plgs.size() == 0)
    {
      throw std::invalid_argument("not enough shapes");
    }
    tmp = plgs;
  }
  else
  {
    size_t vrt_cnt = std::stoull(subcommand);
    if (vrt_cnt < 3)
    {
      throw std::invalid_argument("not enough vertexes");
    }

    VrtCntCheck check{ vrt_cnt };
    std::copy_if(plgs.cbegin(), plgs.cend(), std::back_inserter(tmp), check);
  }

  std::vector< double > areas;
  std::transform(tmp.begin(), tmp.end(), std::back_inserter(areas), getPlgArea);
  double res = std::accumulate(areas.begin(), areas.end(), 0);
  if (subcommand == "MEAN")
  {
    res /= plgs.size();
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

  std::string subcommand;
  in >> subcommand;
  if (subcommand == "AREA")
  {
    auto res = (*std::max_element(plgs.cbegin(), plgs.cend(), compareArea));
    iofmtguard fmtguard(out);
    out << std::setprecision(1) << std::fixed << getPlgArea(res);
  }
  else if (subcommand == "VERTEXES")
  {
    auto res = (*std::max_element(plgs.cbegin(), plgs.cend(), compareVrtCnt));
    out << res.points.size();
  }
  else
  {
    throw std::invalid_argument("unknown command");
  }
}

void demehin::printMinValueOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  if (plgs.size() == 0)
  {
    throw std::invalid_argument("not enough shapes");
  }

  std::string subcommand;
  in >> subcommand;
  iofmtguard fmtguard(out);
  if (subcommand == "AREA")
  {
    auto res = (*std::min_element(plgs.cbegin(), plgs.cend(), compareArea));
    out << std::setprecision(1) << std::fixed << getPlgArea(res);
  }
  else if (subcommand == "VERTEXES")
  {
    auto res = (*std::min_element(plgs.cbegin(), plgs.cend(), compareVrtCnt));
    out << std::setprecision(1) << std::fixed << res.points.size();
  }
  else
  {
    throw std::invalid_argument("unknown command");
  }
}

void demehin::printCountOf(std::istream& in, const std::vector< Polygon >& plgs, std::ostream& out)
{
  std::vector< Polygon > tmp;
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN")
  {
    std::copy_if(plgs.cbegin(), plgs.cend(), std::back_inserter(tmp), isEvenVrts);
  }
  else if (subcommand == "ODD")
  {
    std::copy_if(plgs.cbegin(), plgs.cend(), std::back_inserter(tmp), isOddVrts);
  }
  else
  {
    size_t vrt_cnt = std::stoull(subcommand);
    if (vrt_cnt < 3)
    {
      throw std::invalid_argument("wrong parameter");
    }
    VrtCntCheck check{ vrt_cnt };
    std::copy_if(plgs.cbegin(), plgs.cend(), std::back_inserter(tmp), check);
  }

  out << tmp.size();
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
