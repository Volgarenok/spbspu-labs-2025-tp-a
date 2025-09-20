#include "commands.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <numeric>
#include <algorithm>

namespace
{
  void printArea(double a)
  {
    std::cout << std::fixed << std::setprecision(1) << a << "\n";
  }

  void printCount(size_t c)
  {
    std::cout << c << "\n";
  }
}

namespace cherkasov
{
  void processCommand(const std::vector<Polygon> &polys, const std::string &cmd)
  {
    std::istringstream iss(cmd);
    std::string command;
    iss >> command;
    std::string arg;
    std::getline(iss, arg);
    if (!arg.empty() && arg[0] == ' ')
    {
      arg.erase(0, 1);
    }

    try
    {
      if (command == "AREA")
      {
        handleArea(polys, arg);
      }
      else if (command == "MAX")
      {
        handleMax(polys, arg);
      }
      else if (command == "MIN")
      {
        handleMin(polys, arg);
      }
      else if (command == "COUNT")
      {
        handleCount(polys, arg);
      }
      else if (command == "INTERSECTIONS")
      {
        handleIntersections(polys, arg);
      }
      else if (command == "RIGHTSHAPES")
      {
        handleRightShapes(polys);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  void handleArea(const std::vector<Polygon> &polys, const std::string &arg)
  {
    if (arg == "MEAN")
    {
      if (polys.empty())
      {
        throw std::invalid_argument("no polys");
      }
      double sum = std::accumulate(polys.begin(), polys.end(), 0.0,
        [](double s, const Polygon &p)
        {
          return s + getArea(p);
        });
      printArea(sum / polys.size());
    }
    else if (arg == "EVEN")
    {
      double sum = std::accumulate(polys.begin(), polys.end(), 0.0,
        [](double s, const Polygon &p)
        {
          return (p.points.size() % 2 == 0) ? s + getArea(p) : s;
        });
      printArea(sum);
    }
    else if (arg == "ODD")
    {
      double sum = std::accumulate(polys.begin(), polys.end(), 0.0,
        [](double s, const Polygon &p)
        {
          return (p.points.size() % 2 == 1) ? s + getArea(p) : s;
        });
      printArea(sum);
    }
    else
    {
      size_t num = std::stoul(arg);
      if (num < 3)
      {
        throw std::invalid_argument("invalid vertex count");
      }
      double sum = std::accumulate(polys.begin(), polys.end(), 0.0,
        [num](double s, const Polygon &p)
        {
          return (p.points.size() == num) ? s + getArea(p) : s;
        });
      printArea(sum);
    }
  }

  void handleMax(const std::vector<Polygon> &polys, const std::string &arg)
  {
    if (polys.empty())
    {
      throw std::invalid_argument("no polys");
    }
    if (arg == "AREA")
    {
      auto it = std::max_element(polys.begin(), polys.end(),
        [](const Polygon &a, const Polygon &b)
        {
          return getArea(a) < getArea(b);
        });
      printArea(getArea(*it));
    }
    else if (arg == "VERTEXES")
    {
      auto it = std::max_element(polys.begin(), polys.end(),
        [](const Polygon &a, const Polygon &b)
        {
          return a.points.size() < b.points.size();
        });
      printCount(it->points.size());
    }
    else
    {
      throw std::invalid_argument("bad arg");
    }
  }

  void handleMin(const std::vector<Polygon> &polys, const std::string &arg)
  {
    if (polys.empty())
    {
      throw std::invalid_argument("no polys");
    }
    if (arg == "AREA")
    {
      auto it = std::min_element(polys.begin(), polys.end(),
        [](const Polygon &a, const Polygon &b)
        {
          return getArea(a) < getArea(b);
        });
      printArea(getArea(*it));
    }
    else if (arg == "VERTEXES")
    {
      auto it = std::min_element(polys.begin(), polys.end(),
        [](const Polygon &a, const Polygon &b)
        {
          return a.points.size() < b.points.size();
        });
      printCount(it->points.size());
    }
    else
    {
      throw std::invalid_argument("bad arg");
    }
  }

  void handleCount(const std::vector<Polygon> &polys, const std::string &arg)
  {
    if (arg == "EVEN")
    {
      size_t c = std::count_if(polys.begin(), polys.end(),
        [](const Polygon &p)
        {
          return p.points.size() % 2 == 0;
        });
      printCount(c);
    }
    else if (arg == "ODD")
    {
      size_t c = std::count_if(polys.begin(), polys.end(),
        [](const Polygon &p)
        {
          return p.points.size() % 2 == 1;
        });
      printCount(c);
    }
    else
    {
      size_t num = std::stoul(arg);
      if (num < 3)
      {
        throw std::invalid_argument("invalid vertex count");
      }
      size_t c = std::count_if(polys.begin(), polys.end(),
        [num](const Polygon &p)
        {
          return p.points.size() == num;
        });
      printCount(c);
    }
  }

  void handleIntersections(const std::vector<Polygon> &polys, const std::string &arg)
{
  Polygon q = parsePolygon(arg);
  size_t count = 0;
  for (const auto &p : polys)
  {
    if (polygonsIntersect(p, q))
    {
      count++;
    }
  }
  printCount(count);
}

  void handleRightShapes(const std::vector<Polygon> &polys)
  {
    size_t c = std::count_if(polys.begin(), polys.end(),
      [](const Polygon &p)
      {
        return hasRightAngle(p);
      });
    printCount(c);
  }
}
