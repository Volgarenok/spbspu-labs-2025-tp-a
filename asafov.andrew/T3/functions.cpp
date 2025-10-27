#include "functions.h"
#include <cctype>
#include <numeric>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <map>
#include "io.h"
#include "geometry.h"

namespace
{
  void printArea(double area)
  {
    std::cout << std::fixed << std::setprecision(1) << area << '\n';
  }

  void printCount(size_t count)
  {
    std::cout << count << '\n';
  }

  bool isWhitespace(char c)
  {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
  }
}

void asafov::parseCommand(const std::string& cmd, std::string& command, std::string& arg)
{
  command.clear();
  arg.clear();

  size_t pos = 0;
  asafov::skipWhitespace(cmd, pos);

  while (pos < cmd.size() && !isWhitespace(cmd[pos]))
  {
    command += cmd[pos];
    ++pos;
  }

  asafov::skipWhitespace(cmd, pos);

  while (pos < cmd.size())
  {
    arg += cmd[pos];
    ++pos;
  }
}

bool asafov::tryParseNumber(const std::string& str, size_t& pos, size_t& result)
{
  result = 0;
  bool foundDigit = false;

  while (pos < str.size() && std::isdigit(str[pos]))
  {
    result = result * 10 + (str[pos] - '0');
    ++pos;
    foundDigit = true;
  }

  return foundDigit;
}

size_t asafov::parseVertexCount(const std::string& str, size_t& pos)
{
  asafov::skipWhitespace(str, pos);

  size_t vertexCount = 0;
  if (!asafov::tryParseNumber(str, pos, vertexCount))
  {
    throw std::invalid_argument("Invalid vertex count");
  }

  return vertexCount;
}

void asafov::processCommand(const std::vector< Polygon >& polygons, const std::string& cmd)
{
  if (cmd.empty())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  const std::map<std::string, std::function<void(const std::vector<Polygon>&, const std::string&)>> COMMAND_MAP =
  {
    {"AREA", handleAreaCommands},
    {"MAX", handleMaxCommands},
    {"MIN", handleMinCommands},
    {"COUNT", handleCountCommands},
    {"INTERSECTIONS", handleIntersectionsCommand},
    {"SAME", handleSameCommand}
  };


  std::string command;
  std::string arg;
  asafov::parseCommand(cmd, command, arg);

  try
  {
    auto it = COMMAND_MAP.find(command);
    if (it != COMMAND_MAP.end())
    {
      it->second(polygons, arg);
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  catch (const std::exception&)
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void asafov::handleAreaCommands(const std::vector< Polygon >& polygons, const std::string& arg)
{
  if (arg == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("No polygons");
    }

    struct AreaSummer
    {
      double operator()(const Polygon& poly)
      {
        double area = asafov::computeArea(poly);
        sum += area;
        return area;
      }

      double sum = 0.0;
    };

    AreaSummer summer;
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), std::ref(summer));

    printArea(summer.sum / polygons.size());
  }
  else if (arg == "EVEN")
  {
    struct EvenAreaSummer
    {
      double operator()(const Polygon& poly)
      {
        double area = asafov::computeArea(poly);
        if (poly.points.size() % 2 == 0)
        {
          sum += area;
        }
        return area;
      }

      double sum = 0.0;
    };

    EvenAreaSummer summer;
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), std::ref(summer));

    printArea(summer.sum);
  }
  else if (arg == "ODD")
  {
    struct OddAreaSummer
    {
      double operator()(const Polygon& poly)
      {
        double area = asafov::computeArea(poly);
        if (poly.points.size() % 2 != 0)
        {
          sum += area;
        }
        return area;
      }

      double sum = 0.0;
    };

    OddAreaSummer summer;
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), std::ref(summer));

    printArea(summer.sum);
  }
  else
  {
    size_t vertexCount = 0;
    size_t pos = 0;

    if (!asafov::tryParseNumber(arg, pos, vertexCount) || pos != arg.size() || vertexCount < 3)
    {
      throw std::invalid_argument("Invalid AREA argument");
    }

    struct VertexAreaSummer
    {
      double operator()(const Polygon& poly)
      {
        double area = asafov::computeArea(poly);
        if (poly.points.size() == vertexCount)
        {
          sum += area;
        }
        return area;
      }

      size_t vertexCount;
      double sum = 0.0;
    };

    VertexAreaSummer summer{ vertexCount };
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), std::ref(summer));

    printArea(summer.sum);
  }
}

void asafov::handleMaxCommands(const std::vector< Polygon >& polygons, const std::string& arg)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons");
  }

  if (arg == "AREA")
  {
    struct AreaComparator
    {
      bool operator()(const Polygon& a, const Polygon& b) const
      {
        return asafov::computeArea(a) < asafov::computeArea(b);
      }
    };

    auto it = std::max_element(polygons.begin(), polygons.end(), AreaComparator{});
    printArea(asafov::computeArea(*it));
  }
  else if (arg == "VERTEXES")
  {
    struct VertexCountComparator
    {
      bool operator()(const Polygon& a, const Polygon& b) const
      {
        return a.points.size() < b.points.size();
      }
    };

    auto it = std::max_element(polygons.begin(), polygons.end(), VertexCountComparator{});
    printCount(it->points.size());
  }
  else
  {
    throw std::invalid_argument("Invalid MAX argument");
  }
}

void asafov::handleMinCommands(const std::vector< Polygon >& polygons, const std::string& arg)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons");
  }

  if (arg == "AREA")
  {
    struct AreaComparator
    {
      bool operator()(const Polygon& a, const Polygon& b) const
      {
        return asafov::computeArea(a) < asafov::computeArea(b);
      }
    };

    auto it = std::min_element(polygons.begin(), polygons.end(), AreaComparator{});
    printArea(asafov::computeArea(*it));
  }
  else if (arg == "VERTEXES")
  {
    struct VertexCountComparator
    {
      bool operator()(const Polygon& a, const Polygon& b) const
      {
        return a.points.size() < b.points.size();
      }
    };

    auto it = std::min_element(polygons.begin(), polygons.end(), VertexCountComparator{});
    printCount(it->points.size());
  }
  else
  {
    throw std::invalid_argument("Invalid MIN argument");
  }
}

void asafov::handleCountCommands(const std::vector< Polygon >& polygons, const std::string& arg)
{
  if (arg == "EVEN")
  {
    struct EvenChecker
    {
      bool operator()(const Polygon& poly) const
      {
        return poly.points.size() % 2 == 0;
      }
    };

    size_t count = std::count_if(polygons.begin(), polygons.end(), EvenChecker{});
    printCount(count);
  }
  else if (arg == "ODD")
  {
    struct OddChecker
    {
      bool operator()(const Polygon& poly) const
      {
        return poly.points.size() % 2 != 0;
      }
    };

    size_t count = std::count_if(polygons.begin(), polygons.end(), OddChecker{});
    printCount(count);
  }
  else
  {
    size_t vertexCount = 0;
    size_t pos = 0;

    if (!asafov::tryParseNumber(arg, pos, vertexCount) || pos != arg.size() || vertexCount < 3)
    {
      throw std::invalid_argument("Invalid COUNT argument");
    }

    struct VertexCountChecker
    {
      bool operator()(const Polygon& poly) const
      {
        return poly.points.size() == vertexCount;
      }

      size_t vertexCount;
    };

    size_t count = std::count_if(polygons.begin(), polygons.end(), VertexCountChecker{ vertexCount });
    printCount(count);
  }
}

void asafov::handleIntersectionsCommand(const std::vector< Polygon >& polygons, const std::string& arg)
{
  Polygon target = asafov::parsePolygonFromString(arg);

  struct IntersectionChecker
  {
    bool operator()(const Polygon& poly) const
    {
      return asafov::doPolygonsIntersect(poly, target);
    }

    const Polygon& target;
  };

  size_t count = std::count_if(polygons.begin(), polygons.end(), IntersectionChecker{ target });
  printCount(count);
}

void asafov::handleSameCommand(const std::vector< Polygon >& polygons, const std::string& arg)
{
  Polygon target = asafov::parsePolygonFromString(arg);

  struct Checker
  {
    bool operator()(const Polygon& p) const
    {
      return arePolygonsSame(p, target);
    }

    const Polygon& target;
  };

  size_t count = std::count_if(polygons.begin(), polygons.end(), Checker{ target });

  printCount(count);
}
