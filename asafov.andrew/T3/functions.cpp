#include "functions.h"
#include <cctype>
#include <numeric>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <functional>
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

  void printBool(bool value)
  {
    std::cout << (value ? "<TRUE>" : "<FALSE>") << '\n';
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

  std::string command;
  std::string arg;
  asafov::parseCommand(cmd, command, arg);

  try
  {
    if (command == "AREA")
    {
      asafov::handleAreaCommands(polygons, arg);
    }
    else if (command == "MAX")
    {
      asafov::handleMaxCommands(polygons, arg);
    }
    else if (command == "MIN")
    {
      asafov::handleMinCommands(polygons, arg);
    }
    else if (command == "COUNT")
    {
      asafov::handleCountCommands(polygons, arg);
    }
    else if (command == "PERMS")
    {
      asafov::arePolygonsPermutations(polygons, arg);
    }
    else if (command == "MAXSEQ")
    {
      asafov::handleMaxSeqCommand(polygons, arg);
    }
    else if (command == "RMECHO")
    {
      asafov::handleRmechoCommand(const_cast<std::vector<Polygon>&>(polygons), arg);
    }
    else if (command == "ECHO")
    {
      asafov::handleEchoCommand(const_cast<std::vector<Polygon>&>(polygons), arg);
    }
    else if (command == "LESSAREA")
    {
      asafov::handleLessAreaCommand(polygons, arg);
    }
    else if (command == "INFRAME")
    {
      asafov::handleInFrameCommand(polygons, arg);
    }
    else if (command == "INTERSECTIONS")
    {
      asafov::handleIntersectionsCommand(polygons, arg);
    }
    else if (command == "SAME")
    {
      asafov::handleSameCommand(polygons, arg);
    }
    else if (command == "RECTS")
    {
      asafov::handleRectsCommand(polygons);
    }
    else if (command == "RIGHTSHAPES")
    {
      asafov::handleRightShapesCommand(polygons);
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
    if (polygons.empty()) throw std::invalid_argument("No polygons");

    double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [](double sum, const Polygon& poly) { return sum + asafov::computeArea(poly); });

    printArea(total / polygons.size());
  }
  else if (arg == "EVEN")
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [](double sum, const Polygon& poly) {
        return poly.points.size() % 2 == 0 ? sum + asafov::computeArea(poly) : sum;
      });

    printArea(sum);
  }
  else if (arg == "ODD")
  {
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
      [](double sum, const Polygon& poly) {
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
      [vertexCount](double sum, const Polygon& poly) {
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
      [](const Polygon& a, const Polygon& b) {
        return asafov::computeArea(a) < asafov::computeArea(b);
      });

    printArea(asafov::computeArea(*it));
  }
  else if (arg == "VERTEXES")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(),
      [](const Polygon& a, const Polygon& b) {
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
      [](const Polygon& a, const Polygon& b) {
        return asafov::computeArea(a) < asafov::computeArea(b);
      });

    printArea(asafov::computeArea(*it));
  }
  else if (arg == "VERTEXES")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(),
      [](const Polygon& a, const Polygon& b) {
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
      [](const Polygon& poly) { return poly.points.size() % 2 == 0; });

    printCount(count);
  }
  else if (arg == "ODD")
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly) { return poly.points.size() % 2 != 0; });

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
      [vertexCount](const Polygon& poly) { return poly.points.size() == vertexCount; });

    printCount(count);
  }
}

void asafov::handlePermsCommand(const std::vector<Polygon>& polygons, const std::string& arg)
{
  try
  {
    Polygon target = asafov::parsePolygonFromString(arg);
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [&target](const Polygon& poly) { return asafov::arePolygonsSame(poly, target); });

    printCount(count);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Invalid PERMS argument");
  }
}

void asafov::handleMaxSeqCommand(const std::vector<Polygon>& polygons, const std::string& arg)
{
  try
  {
    Polygon target = asafov::parsePolygonFromString(arg);
    size_t maxSeq = 0;
    size_t currentSeq = 0;

    for (const auto& poly : polygons)
    {
      if (asafov::arePolygonsSame(poly, target))
      {
        ++currentSeq;
        maxSeq = std::max(maxSeq, currentSeq);
      }
      else
      {
        currentSeq = 0;
      }
    }

    printCount(maxSeq);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Invalid MAXSEQ argument");
  }
}

void asafov::handleRmechoCommand(std::vector<Polygon>& polygons, const std::string& arg)
{
  try
  {
    Polygon target = asafov::parsePolygonFromString(arg);
    size_t removed = 0;
    auto newEnd = std::unique(polygons.begin(), polygons.end(),
      [&target, &removed](const Polygon& a, const Polygon& b) {
        if (asafov::arePolygonsSame(a, target) && asafov::arePolygonsSame(b, target))
        {
          ++removed;
          return true;
        }
        return false;
      });

    polygons.erase(newEnd, polygons.end());
    printCount(removed);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Invalid RMECHO argument");
  }
}

void asafov::handleEchoCommand(std::vector<Polygon>& polygons, const std::string& arg)
{
  try
  {
    Polygon target = asafov::parsePolygonFromString(arg);
    size_t added = 0;
    std::vector<Polygon> result;

    for (const auto& poly : polygons)
    {
      result.push_back(poly);
      if (asafov::arePolygonsSame(poly, target))
      {
        result.push_back(poly);
        ++added;
      }
    }

    polygons = result;
    printCount(added);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Invalid ECHO argument");
  }
}

void asafov::handleLessAreaCommand(const std::vector<Polygon>& polygons, const std::string& arg)
{
  try
  {
    Polygon target = asafov::parsePolygonFromString(arg);
    double targetArea = asafov::computeArea(target);

    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [targetArea](const Polygon& poly) { return asafov::computeArea(poly) < targetArea; });

    printCount(count);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Invalid LESSAREA argument");
  }
}

void asafov::handleInFrameCommand(const std::vector<Polygon>& polygons, const std::string& arg)
{
  try
  {
    Polygon target = asafov::parsePolygonFromString(arg);
    Polygon bbox = asafov::getBoundingBox(polygons);

    bool allInside = std::all_of(target.points.begin(), target.points.end(),
      [&bbox](const Point& point) { return asafov::isPointInPolygon(point, bbox); });

    printBool(allInside);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Invalid INFRAME argument");
  }
}

void asafov::handleIntersectionsCommand(const std::vector<Polygon>& polygons, const std::string& arg)
{
  try
  {
    Polygon target = asafov::parsePolygonFromString(arg);

    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [&target](const Polygon& poly) { return asafov::doPolygonsIntersect(poly, target); });

    printCount(count);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Invalid INTERSECTIONS argument");
  }
}

void asafov::handleSameCommand(const std::vector<Polygon>& polygons, const std::string& arg)
{
  try
  {
    Polygon target = asafov::parsePolygonFromString(arg);

    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [&target](const Polygon& poly) { return asafov::arePolygonsSame(poly, target); });

    printCount(count);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Invalid SAME argument");
  }
}

void asafov::handleRectsCommand(const std::vector<Polygon>& polygons)
{
  size_t count = std::count_if(polygons.begin(), polygons.end(),
    [](const Polygon& poly) { return asafov::isPolygonRectangle(poly); });

  printCount(count);
}

void asafov::handleRightShapesCommand(const std::vector<Polygon>& polygons)
{
  size_t count = std::count_if(polygons.begin(), polygons.end(),
    [](const Polygon& poly) { return asafov::hasRightAngle(poly); });

  printCount(count);
}
