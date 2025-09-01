#include "functions.h"
#include "geometry.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <stdexcept>
#include <cctype>
#include <io.h>

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

  void skipWhitespace(const std::string& str, size_t& pos)
  {
    while (pos < str.size() && isWhitespace(str[pos]))
    {
      ++pos;
    }
  }
}

namespace asafov
{
  void parseCommand(const std::string& cmd, std::string& command, std::string& arg)
  {
    command.clear();
    arg.clear();

    size_t pos = 0;
    skipWhitespace(cmd, pos);

    while (pos < cmd.size() && !isWhitespace(cmd[pos]))
    {
      command += cmd[pos];
      ++pos;
    }

    skipWhitespace(cmd, pos);

    while (pos < cmd.size())
    {
      arg += cmd[pos];
      ++pos;
    }
  }

  bool tryParseNumber(const std::string& str, size_t& pos, size_t& result)
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

  size_t parseVertexCount(const std::string& str, size_t& pos)
  {
    skipWhitespace(str, pos);

    size_t vertexCount = 0;
    if (!tryParseNumber(str, pos, vertexCount))
    {
      throw std::invalid_argument("Invalid vertex count");
    }

    return vertexCount;
  }

  void processCommand(const std::vector<Polygon>& polygons, const std::string& cmd)
  {
    if (cmd.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    std::string command;
    std::string arg;
    parseCommand(cmd, command, arg);

    try
    {
      if (command == "AREA")
      {
        handleAreaCommands(polygons, arg);
      }
      else if (command == "MAX")
      {
        handleMaxCommands(polygons, arg);
      }
      else if (command == "MIN")
      {
        handleMinCommands(polygons, arg);
      }
      else if (command == "COUNT")
      {
        handleCountCommands(polygons, arg);
      }
      else if (command == "PERMS")
      {
        handlePermsCommand(polygons, arg);
      }
      else if (command == "MAXSEQ")
      {
        handleMaxSeqCommand(polygons, arg);
      }
      else if (command == "RMECHO")
      {
        handleRmechoCommand(const_cast<std::vector<Polygon>&>(polygons), arg);
      }
      else if (command == "ECHO")
      {
        handleEchoCommand(const_cast<std::vector<Polygon>&>(polygons), arg);
      }
      else if (command == "LESSAREA")
      {
        handleLessAreaCommand(polygons, arg);
      }
      else if (command == "INFRAME")
      {
        handleInFrameCommand(polygons, arg);
      }
      else if (command == "INTERSECTIONS")
      {
        handleIntersectionsCommand(polygons, arg);
      }
      else if (command == "SAME")
      {
        handleSameCommand(polygons, arg);
      }
      else if (command == "RECTS")
      {
        handleRectsCommand(polygons);
      }
      else if (command == "RIGHTSHAPES")
      {
        handleRightShapesCommand(polygons);
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

  void handleAreaCommands(const std::vector<Polygon>& polygons, const std::string& arg)
  {
    if (arg == "MEAN")
    {
      if (polygons.empty()) throw std::invalid_argument("No polygons");

      double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [](double sum, const Polygon& poly) { return sum + computeArea(poly); });

      printArea(total / polygons.size());
    }
    else if (arg == "EVEN")
    {
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [](double sum, const Polygon& poly) {
          return poly.points.size() % 2 == 0 ? sum + computeArea(poly) : sum;
        });

      printArea(sum);
    }
    else if (arg == "ODD")
    {
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [](double sum, const Polygon& poly) {
          return poly.points.size() % 2 != 0 ? sum + computeArea(poly) : sum;
        });

      printArea(sum);
    }
    else
    {
      size_t vertexCount = 0;
      size_t pos = 0;

      if (!tryParseNumber(arg, pos, vertexCount) || pos != arg.size())
      {
        throw std::invalid_argument("Invalid AREA argument");
      }

      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [vertexCount](double sum, const Polygon& poly) {
          return poly.points.size() == vertexCount ? sum + computeArea(poly) : sum;
        });

      printArea(sum);
    }
  }

  void handleMaxCommands(const std::vector<Polygon>& polygons, const std::string& arg)
  {
    if (polygons.empty()) throw std::invalid_argument("No polygons");

    if (arg == "AREA")
    {
      auto it = std::max_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b) {
          return computeArea(a) < computeArea(b);
        });

      printArea(computeArea(*it));
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

  void handleMinCommands(const std::vector<Polygon>& polygons, const std::string& arg)
  {
    if (polygons.empty()) throw std::invalid_argument("No polygons");

    if (arg == "AREA")
    {
      auto it = std::min_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b) {
          return computeArea(a) < computeArea(b);
        });

      printArea(computeArea(*it));
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

  void handleCountCommands(const std::vector<Polygon>& polygons, const std::string& arg)
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

      if (!tryParseNumber(arg, pos, vertexCount) || pos != arg.size())
      {
        throw std::invalid_argument("Invalid COUNT argument");
      }

      size_t count = std::count_if(polygons.begin(), polygons.end(),
        [vertexCount](const Polygon& poly) { return poly.points.size() == vertexCount; });

      printCount(count);
    }
  }

  void handlePermsCommand(const std::vector<Polygon>& polygons, const std::string& arg)
  {
    Polygon target = parsePolygonFromString(arg);
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [&target](const Polygon& poly) { return arePolygonsSame(poly, target); });

    printCount(count);
  }

  void handleMaxSeqCommand(const std::vector<Polygon>& polygons, const std::string& arg)
  {
    Polygon target = parsePolygonFromString(arg);
    size_t maxSeq = 0;
    size_t currentSeq = 0;

    for (const auto& poly : polygons)
    {
      if (arePolygonsSame(poly, target))
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

  void handleRmechoCommand(std::vector<Polygon>& polygons, const std::string& arg)
  {
    Polygon target = parsePolygonFromString(arg);
    size_t removed = 0;
    auto newEnd = std::unique(polygons.begin(), polygons.end(),
      [&target, &removed](const Polygon& a, const Polygon& b) {
        if (arePolygonsSame(a, target) && arePolygonsSame(b, target))
        {
          ++removed;
          return true;
        }
        return false;
      });

    polygons.erase(newEnd, polygons.end());
    printCount(removed);
  }

  void handleEchoCommand(std::vector<Polygon>& polygons, const std::string& arg)
  {
    Polygon target = parsePolygonFromString(arg);
    size_t added = 0;
    std::vector<Polygon> result;

    for (const auto& poly : polygons)
    {
      result.push_back(poly);
      if (arePolygonsSame(poly, target))
      {
        result.push_back(poly);
        ++added;
      }
    }

    polygons = result;
    printCount(added);
  }

  void handleLessAreaCommand(const std::vector<Polygon>& polygons, const std::string& arg)
  {
    Polygon target = parsePolygonFromString(arg);
    double targetArea = computeArea(target);

    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [targetArea](const Polygon& poly) { return computeArea(poly) < targetArea; });

    printCount(count);
  }

  void handleInFrameCommand(const std::vector<Polygon>& polygons, const std::string& arg)
  {
    Polygon target = parsePolygonFromString(arg);
    Polygon bbox = getBoundingBox(polygons);

    bool allInside = std::all_of(target.points.begin(), target.points.end(),
      [&bbox](const Point& point) { return isPointInPolygon(point, bbox); });

    printBool(allInside);
  }

  void handleIntersectionsCommand(const std::vector<Polygon>& polygons, const std::string& arg)
  {
    Polygon target = parsePolygonFromString(arg);

    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [&target](const Polygon& poly) { return doPolygonsIntersect(poly, target); });

    printCount(count);
  }

  void handleSameCommand(const std::vector<Polygon>& polygons, const std::string& arg)
  {
    Polygon target = parsePolygonFromString(arg);

    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [&target](const Polygon& poly) { return arePolygonsSame(poly, target); });

    printCount(count);
  }

  void handleRectsCommand(const std::vector<Polygon>& polygons)
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly) { return isPolygonRectangle(poly); });

    printCount(count);
  }

  void handleRightShapesCommand(const std::vector<Polygon>& polygons)
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(),
      [](const Polygon& poly) { return hasRightAngle(poly); });

    printCount(count);
  }
}
