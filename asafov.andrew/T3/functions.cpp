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

void asafov::processCommand(const std::vector<Polygon>& polygons, const std::string& cmd)
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

void asafov::handleAreaCommands(const std::vector<Polygon>& polygons, const std::string& arg)
{
  if (arg == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("No polygons");
    }

    double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [](double sum, const Polygon& poly)
      {
        return sum + asafov::computeArea(poly);
      });

    printArea(total / polygons.size());
  }
  else if (arg == "EVEN")
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [](double sum, const Polygon& poly)
      {
        return poly.points.size() % 2 == 0 ? sum + asafov::computeArea(poly) : sum;
      });

    printArea(sum);
  }
  else if (arg == "ODD")
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [](double sum, const Polygon& poly)
      {
        return poly.points.size() % 2 != 0 ? sum + asafov::computeArea(poly) : sum;
      });

    printArea(sum);
  }
  else
  {
    size_t vertexCount = 0;
    size_t pos = 0;

    if (!asafov::tryParseNumber(arg, pos, vertexCount) || pos != arg.size() || vertexCount < 3)
    {
      throw std::invalid_argument("Invalid AREA argument");
    }

    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [vertexCount](double sum, const Polygon& poly)
      {
        return poly.points.size() == vertexCount ? sum + asafov::computeArea(poly) : sum;
      });

    printArea(sum);
  }
}

void asafov::handleMaxCommands(const std::vector<Polygon>& polygons, const std::string& arg)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons");
  }

  if (arg == "AREA")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(),
      [](const Polygon& a, const Polygon& b)
      {
        return asafov::computeArea(a) < asafov::computeArea(b);
      });

    printArea(asafov::computeArea(*it));
  }
  else if (arg == "VERTEXES")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(),
      [](const Polygon& a, const Polygon& b)
      {
        return a.points.size() < b.points.size();
      });

    printCount(it->points.size());
  }
  else
  {
    throw std::invalid_argument("Invalid MAX argument");
  }
}

void asafov::handleMinCommands(const std::vector<Polygon>& polygons, const std::string& arg)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("No polygons");
  }

  if (arg == "AREA")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(),
      [](const Polygon& a, const Polygon& b)
      {
        return asafov::computeArea(a) < asafov::computeArea(b);
      });

    printArea(asafov::computeArea(*it));
  }
  else if (arg == "VERTEXES")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(),
      [](const Polygon& a, const Polygon& b)
      {
        return a.points.size() < b.points.size();
      });

    printCount(it->points.size());
  }
  else
  {
    throw std::invalid_argument("Invalid MIN argument");
  }
}

void asafov::handleCountCommands(const std::vector<Polygon>& polygons, const std::string& arg)
{
  if (arg == "EVEN")
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly)
      {
        return poly.points.size() % 2 == 0;
      });

    printCount(count);
  }
  else if (arg == "ODD")
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly)
      {
        return poly.points.size() % 2 != 0;
      });

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

    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [vertexCount](const Polygon& poly)
      {
        return poly.points.size() == vertexCount;
      });

    printCount(count);
  }
}

void asafov::handleIntersectionsCommand(const std::vector<Polygon>& polygons, const std::string& arg)
{
  Polygon target = asafov::parsePolygonFromString(arg);

  size_t count = std::count_if(polygons.begin(), polygons.end(),
    [&target](const Polygon& poly)
    {
      return asafov::doPolygonsIntersect(poly, target);
    });

  printCount(count);
}

void asafov::handleSameCommand(const std::vector<Polygon>& polygons, const std::string& arg)
{
  Polygon target = asafov::parsePolygonFromString(arg);

  size_t count = std::count_if(polygons.begin(), polygons.end(),
    [&target](const Polygon& poly)
    {
      return asafov::arePolygonsSame(poly, target);
    });

  printCount(count);
}
