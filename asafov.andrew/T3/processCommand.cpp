#include "functions.h"
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iostream>

void asafov::processCommand(const std::vector< Polygon >& polygons, const std::string& cmd)
{
  auto getPolygon = [](const std::string& s) ->Polygon
  {
    size_t space = s.find(' ');
    if (space == std::string::npos) throw std::invalid_argument("Invalid command");

    size_t num_vertices = std::stoul(s.substr(0, space));
    std::string data = s.substr(space + 1);

    std::vector< Point > points;
    size_t pos = 0;
    for (size_t i = 0; i < num_vertices; ++i)
    {
      size_t p1 = data.find('(', pos);
      size_t p2 = data.find(';', p1);
      size_t p3 = data.find(')', p2);
      if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos)
        throw std::invalid_argument("Invalid point format");

      points.push_back({
        std::stoi(data.substr(p1 + 1, p2 - p1 - 1)),
        std::stoi(data.substr(p2 + 1, p3 - p2 - 1))
      });
      pos = p3 + 1;
    }
    return {points};
  };

  auto area_even = [](double a, const Polygon& p)
  {
    return a + (p.points.size() % 2 == 0 ? computeArea(p) : 0);
  };
  auto area_odd = [](double a, const Polygon& p)
  {
    return a + (p.points.size() % 2 != 0 ? computeArea(p) : 0);
  };
  auto area_all = [](double a, const Polygon& p)
  {
    return a + computeArea(p);
  };
  auto area_num = [](size_t num)
  {
    return [num](double a, const Polygon& p)
    {
      return a + (p.points.size() == num ? computeArea(p) : 0);
    };
  };

  auto count_even = [](const Polygon& p)
  {
    return p.points.size() % 2 == 0;
  };
  auto count_odd = [](const Polygon& p)
  {
    return p.points.size() % 2 != 0;
  };
  auto count_num = [](size_t num)
  {
    return [num](const Polygon& p)
    {
      return p.points.size() == num;
    };
  };

  try
  {
    if (cmd.find("AREA ") == 0)
    {
      std::string arg = cmd.substr(5);
      if (arg == "EVEN")
      {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, area_even);
        std::cout << sum << '\n';
      }
      else if (arg == "ODD")
      {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, area_odd);
        std::cout << sum << '\n';
      }
      else if (arg == "MEAN")
      {
        if (polygons.empty()) throw std::invalid_argument("No polygons");
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, area_all);
        std::cout << (sum / polygons.size()) << '\n';
      }
      else
      {
        size_t num = std::stoul(arg);
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, area_num(num));
        std::cout << sum << '\n';
      }
    }
    else if (cmd.find("MAX ") == 0)
    {
      std::string arg = cmd.substr(4);
      if (polygons.empty()) throw std::invalid_argument("No polygons");
      if (arg == "AREA")
      {
        auto it = std::max_element(polygons.begin(), polygons.end(),
                                   [](const Polygon& a, const Polygon& b)
                                   {
                                     return computeArea(a) < computeArea(b);
                                   });
        std::cout << computeArea(*it) << '\n';
      }
      else if (arg == "VERTEXES")
      {
        auto it = std::max_element(polygons.begin(), polygons.end(),
                                   [](const Polygon& a, const Polygon& b)
                                   {
                                     return a.points.size() < b.points.size();
                                   });
        std::cout << it->points.size() << '\n';
      }
      else throw std::invalid_argument("Invalid MAX argument");
    }
    else if (cmd.find("MIN ") == 0)
    {
      std::string arg = cmd.substr(4);
      if (polygons.empty()) throw std::invalid_argument("No polygons");
      if (arg == "AREA")
      {
        auto it = std::min_element(polygons.begin(), polygons.end(),
                                   [](const Polygon& a, const Polygon& b)
                                   {
                                     return computeArea(a) < computeArea(b);
                                   });
        std::cout << computeArea(*it) << '\n';
      }
      else if (arg == "VERTEXES")
      {
        auto it = std::min_element(polygons.begin(), polygons.end(),
                                   [](const Polygon& a, const Polygon& b)
                                   {
                                     return a.points.size() < b.points.size();
                                   });
        std::cout << it->points.size() << '\n';
      }
      else throw std::invalid_argument("Invalid MIN argument");
    }
    else if (cmd.find("COUNT ") == 0)
    {
      std::string arg = cmd.substr(6);
      if (arg == "EVEN")
      {
        auto count = std::count_if(polygons.begin(), polygons.end(), count_even);
        std::cout << count << '\n';
      }
      else if (arg == "ODD")
      {
        auto count = std::count_if(polygons.begin(), polygons.end(), count_odd);
        std::cout << count << '\n';
      }
      else
      {
        size_t num = std::stoul(arg);
        auto count = std::count_if(polygons.begin(), polygons.end(), count_num(num));
        std::cout << count << '\n';
      }
    }
    else if (cmd.find("INTERSECTIONS ") == 0)
    {
      Polygon target = getPolygon(cmd.substr(14));
      auto count = std::count_if(polygons.begin(), polygons.end(),
                                 [&target](const Polygon& p)
                                 {
                                   return doPolygonsIntersect(p, target);
                                 });
      std::cout << count << '\n';
    }
    else if (cmd.find("SAME ") == 0)
    {
      Polygon target = getPolygon(cmd.substr(5));
      auto count = std::count_if(polygons.begin(), polygons.end(),
                                 [&target](const Polygon& p)
                                 {
                                   return arePolygonsSame(p, target);
                                 });
      std::cout << count << '\n';
    }
    else throw std::invalid_argument("Unknown command");
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}
