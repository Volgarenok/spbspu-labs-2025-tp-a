#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include "polygon.hpp"
#include "scopeguard.hpp"

namespace
{
  void maxAreaCmd(const std::vector<smirnov::Polygon>& p, std::ostream& out)
  {
    auto it = std::max_element(p.begin(), p.end(), smirnov::minArea);
    if (it != p.end())
    {
      out << smirnov::getArea(*it) << "\n";
    }
    else
    {
      throw std::logic_error("no max found");
    }
  }

  void minAreaCmd(const std::vector<smirnov::Polygon>& p, std::ostream& out)
  {
    auto it = std::min_element(p.begin(), p.end(), smirnov::minArea);
    if (it != p.end())
    {
      out << smirnov::getArea(*it) << "\n";
    }
    else
    {
      throw std::logic_error("no minimum area");
    }
  }

  void maxVertexesCmd(const std::vector<smirnov::Polygon>& p, std::ostream& out)
  {
    auto it = std::max_element(p.begin(), p.end(), smirnov::minVertexes);
    if (it != p.end())
    {
      out << it->points.size() << "\n";
    }
  }

  void minVertexesCmd(const std::vector<smirnov::Polygon>& p, std::ostream& out)
  {
    auto it = std::min_element(p.begin(), p.end(), smirnov::minVertexes);
    if (it != p.end())
    {
      out << it->points.size() << "\n";
    }
    else
    {
      throw std::logic_error("minimum vertexes not supported");
    }
  }

  void doMinMaxCmd(const std::vector< smirnov::Polygon >& p, std::istream& in, std::ostream& out, const std::string& name)
  {
    std::string s;
    in >> s;

    smirnov::ScopeGuard guard(out);

    if (p.empty())
    {
      throw std::logic_error("zero polygons");
    }

    out << std::fixed << std::setprecision(1);

    const std::map< std::string, std::map< std::string, std::function< void() > > > commands =
    {
      {
        "AREA",
        {
          {"max", std::bind(maxAreaCmd, std::cref(p), std::ref(out))},
          {"min", std::bind(minAreaCmd, std::cref(p), std::ref(out))}
        }
      },
      {
        "VERTEXES",
        {
          {"max", std::bind(maxVertexesCmd, std::cref(p), std::ref(out))},
          {"min", std::bind(minVertexesCmd, std::cref(p), std::ref(out))}
        }
      }
    };

    auto cmd = commands.find(s);
    if (cmd == commands.end())
    {
      throw std::logic_error("INVALID COMMAND");
    }

    auto subCmd = cmd->second.find(name);
    if (subCmd == cmd->second.end())
    {
      throw std::logic_error("INVALID COMMAND");
    }

    subCmd->second();
  }

  size_t countEvenCmd(const std::vector<smirnov::Polygon>& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), smirnov::isEvenCountVertexes);
  }

  size_t countOddCmd(const std::vector<smirnov::Polygon>& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), smirnov::isOddCountVertexes);
  }

  bool hasIntersection(const smirnov::Polygon& p1, const smirnov::Polygon& p2)
  {
    auto pair1 = std::minmax_element(p1.points.cbegin(), p1.points.cend());
    auto pair2 = std::minmax_element(p2.points.cbegin(), p2.points.cend());

    bool noOverlapX = (pair1.second->x < pair2.first->x) || (pair2.second->x < pair1.first->x);
    bool touchX = (pair1.second->x == pair2.first->x) && (pair1.second->y < pair2.first->y);
    bool touchX2 = (pair2.second->x == pair1.first->x) && (pair2.second->y < pair1.first->y);

    return !(noOverlapX || touchX || touchX2);
  }

  template < class UnaryPredicate >
  double getSumArea(const std::vector< smirnov::Polygon >& polygons, UnaryPredicate P)
  {
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, [&](double acc, const smirnov::Polygon& poly)
    {
      return P(poly) ? acc + smirnov::getArea(poly) : acc;
    });
  }

  double doAreaEven(const std::vector< smirnov::Polygon >& polygons)
  {
    return getSumArea(polygons, smirnov::isEvenCountVertexes);
  }

  double doAreaOdd(const std::vector< smirnov::Polygon >& polygons)
  {
    return getSumArea(polygons, smirnov::isOddCountVertexes);
  }

  struct SumAreas
  {
    double operator()(double acc, const smirnov::Polygon& p) const
    {
      return acc + smirnov::getArea(p);
    }
  };

  double doAreaMean(const std::vector<smirnov::Polygon>& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("NO POLYGONS FOR AREA MEAN COMMAND");
    }

    double result = std::accumulate(polygons.begin(), polygons.end(), 0.0, SumAreas{});
    result /= polygons.size();
    return result;
  }

  double doAreaNum(const std::vector< smirnov::Polygon >& polygons, size_t n)
  {
    return getSumArea(polygons, std::bind(smirnov::isNCountVertexes, std::placeholders::_1, n));
  }
}

void smirnov::doAreaCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  std::string s;
  in >> s;
  double result = 0.0;
  ScopeGuard guard(out);

  const std::map< std::string, std::function< double() > > subcommand =
  {
    { "EVEN", std::bind(doAreaEven, polygons) },
    { "ODD", std::bind(doAreaOdd, polygons) },
    { "MEAN", std::bind(doAreaMean, polygons) }
  };

  try
  {
    size_t n = std::stoull(s);
    if (n < 3)
    {
      throw std::logic_error("FEW VERTEXES");
    }
    result = doAreaNum(polygons, n);
  }
  catch (const std::invalid_argument&)
  {
    result = subcommand.at(s)();
  }
  out << std::fixed << std::setprecision(1) << result << "\n";
}

void smirnov::doMaxCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  doMinMaxCmd(polygons, in, out, "max");
}

void smirnov::doMinCommand(const std::vector< smirnov::Polygon >& polygons, std::istream& in, std::ostream& out)
{
  doMinMaxCmd(polygons, in, out, "min");
}

void smirnov::doCountCommand(const std::vector<Polygon>& polygons, std::istream& in, std::ostream& out)
{
  std::string s;
  in >> s;

  const std::map<std::string, std::function<size_t()>> commands =
  {
    {"EVEN", std::bind(countEvenCmd, std::cref(polygons))},
    {"ODD", std::bind(countOddCmd, std::cref(polygons))}
  };

  auto it = commands.find(s);
  if (it != commands.end())
  {
    out << it->second() << "\n";
    return;
  }

  if (std::all_of(s.cbegin(), s.cend(), ::isdigit))
  {
    size_t n = std::stoull(s);
    if (n < 3)
    {
      throw std::logic_error("FEW VERTEXES");
    }
    out << std::count_if(polygons.begin(), polygons.end(), std::bind(smirnov::isNCountVertexes, std::placeholders::_1, n)) << "\n";
    return;
  }

  throw std::logic_error("INVALID COMMAND");
}

void smirnov::doRectsCommand(const std::vector< Polygon >& polygons, std::ostream& out)
{
  out << std::count_if(polygons.begin(), polygons.end(), isRect) << "\n";
}

void smirnov::doIntersections(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  Polygon Polygon;
  in >> Polygon;
  if (!in)
  {
    throw std::logic_error("Wrong argument");
  }

  out << std::count_if(data.begin(), data.end(), std::bind(hasIntersection, std::placeholders::_1, Polygon)) << '\n';
}
