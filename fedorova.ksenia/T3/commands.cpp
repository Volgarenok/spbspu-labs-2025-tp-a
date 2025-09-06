#include "commands.h"
#include "polygon.h"
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <unordered_map>

namespace fedorova
{
  namespace
  {
    bool isEvenVrts(const Polygon& plg)
    {
      return plg.points.size() % 2 == 0;
    }

    bool isOddVrts(const Polygon& plg)
    {
      return !isEvenVrts(plg);
    }

    struct VrtCntCheck
    {
      size_t cnt;
      bool operator()(const Polygon& plg) const
      {
        return plg.points.size() == cnt;
      }
    };

    double sumAreaIf(const std::vector<Polygon>& plgs, std::function<bool(const Polygon&)> pred)
    {
      return std::accumulate(plgs.begin(), plgs.end(), 0.0,
        [pred](double sum, const Polygon& p) {
          return pred(p) ? sum + getArea(p) : sum;
        });
    }

    size_t countIf(const std::vector<Polygon>& plgs, std::function<bool(const Polygon&)> pred)
    {
      return std::count_if(plgs.begin(), plgs.end(), pred);
    }
  }

  void printAreaSum(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out)
  {
    std::string subcommand;
    in >> subcommand;

    std::unordered_map<std::string, std::function<double()>> subcmds = {
      {"EVEN", [&]() { return sumAreaIf(plgs, isEvenVrts); }},
      {"ODD", [&]() { return sumAreaIf(plgs, isOddVrts); }},
      {"MEAN", [&]() {
        if (plgs.empty()) throw std::invalid_argument("not enough shapes");
        return sumAreaIf(plgs, [](const Polygon&) { return true; }) / plgs.size();
      }}
    };

    try
    {
      double res;
      if (subcmds.count(subcommand))
      {
        res = subcmds.at(subcommand)();
      }
      else
      {
        size_t vrt_cnt = std::stoull(subcommand);
        if (vrt_cnt < 3) throw std::invalid_argument("invalid vertex count");
        res = sumAreaIf(plgs, VrtCntCheck{vrt_cnt});
      }
      out << std::fixed << std::setprecision(1) << res;
    }
    catch (...)
    {
      out << "<INVALID COMMAND>";
    }
  }

  void printMaxValueOf(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out)
  {
    if (plgs.empty())
    {
      out << "<INVALID COMMAND>";
      return;
    }

    std::string subcommand;
    in >> subcommand;

    if (subcommand == "AREA")
    {
      auto it = std::max_element(plgs.begin(), plgs.end(),
        [](const Polygon& a, const Polygon& b) {
          return getArea(a) < getArea(b);
        });
      out << std::fixed << std::setprecision(1) << getArea(*it);
    }
    else if (subcommand == "VERTEXES")
    {
      auto it = std::max_element(plgs.begin(), plgs.end(),
        [](const Polygon& a, const Polygon& b) {
          return a.points.size() < b.points.size();
        });
      out << it->points.size();
    }
    else
    {
      out << "<INVALID COMMAND>";
    }
  }

  void printMinValueOf(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out)
  {
    if (plgs.empty())
    {
      out << "<INVALID COMMAND>";
      return;
    }

    std::string subcommand;
    in >> subcommand;

    if (subcommand == "AREA")
    {
      auto it = std::min_element(plgs.begin(), plgs.end(),
        [](const Polygon& a, const Polygon& b) {
          return getArea(a) < getArea(b);
        });
      out << std::fixed << std::setprecision(1) << getArea(*it);
    }
    else if (subcommand == "VERTEXES")
    {
      auto it = std::min_element(plgs.begin(), plgs.end(),
        [](const Polygon& a, const Polygon& b) {
          return a.points.size() < b.points.size();
        });
      out << it->points.size();
    }
    else
    {
      out << "<INVALID COMMAND>";
    }
  }

  void printCountOf(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out)
  {
    std::string subcommand;
    in >> subcommand;

    try
    {
      if (subcommand == "EVEN")
      {
        out << std::count_if(plgs.begin(), plgs.end(),
          [](const Polygon& p) { return p.points.size() % 2 == 0; });
      }
      else if (subcommand == "ODD")
      {
        out << std::count_if(plgs.begin(), plgs.end(),
          [](const Polygon& p) { return p.points.size() % 2 != 0; });
      }
      else
      {
        size_t vrt_cnt = std::stoull(subcommand);
        if (vrt_cnt < 3)
        {
          throw std::invalid_argument("invalid vertex count");
        }
        out << std::count_if(plgs.begin(), plgs.end(),
          [vrt_cnt](const Polygon& p) { return p.points.size() == vrt_cnt; });
      }
    }
    catch (...)
    {
      out << "<INVALID COMMAND>";
    }
  }

  void printPermsCnt(std::istream& in, const std::vector<Polygon>& plgs, std::ostream& out)
  {
    Polygon target;
    if (!(in >> target) || target.points.size() < 3)
    {
      out << "<INVALID COMMAND>";
      return;
    }

    size_t cnt = countIf(plgs, [&target](const Polygon& p) {
      return isPermutation(p, target);
    });
    out << cnt;
  }

  void printRightsCnt(const std::vector<Polygon>& plgs, std::ostream& out)
  {
    size_t cnt = countIf(plgs, [](const Polygon& p) {
      return hasRightAngle(p);
    });
    out << cnt;
  }
}
