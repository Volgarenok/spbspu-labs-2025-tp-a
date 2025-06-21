#include "comands.hpp"
#include <numeric>
#include <algorithm>
#include <map>
#include <string>
#include <functional>
#include <stdexcept>
#include <iomanip>
#include <scope_guard.hpp>

namespace
{
  bool isEven(const karnauhova::Polygon& pol)
  {
    return pol.points.size() % 2 == 0;
  }

  bool isOdd(const karnauhova::Polygon& pol)
  {
    return !isEven(pol);
  }

  bool allAreas(const karnauhova::Polygon&)
  {
    return true;
  }

  struct CountCompare
  {
    size_t count;
    bool operator()(const karnauhova::Polygon& pol) const
    {
      return count == pol.points.size();
    }
  };

  template< typename Compare >
  double sumAreas(const std::vector< karnauhova::Polygon >& polygons, Compare C)
  {
    std::vector< karnauhova::Polygon > it;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(it), C);
    std::vector< double > areas;
    std::transform(it.begin(), it.end(), std::back_inserter(areas), karnauhova::getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }
  bool compArea(const karnauhova::Polygon& poll, const karnauhova::Polygon& polr)
  {
    return karnauhova::getArea(poll) < karnauhova::getArea(polr);
  }
  bool compVert(const karnauhova::Polygon& poll, const karnauhova::Polygon& polr)
  {
    return poll.points.size() < polr.points.size();
  }

  template< typename Compare >
  size_t sumPol(const std::vector< karnauhova::Polygon >& polygons, Compare C)
  {
    std::vector< karnauhova::Polygon > it;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(it), C);
    return it.size();
  }
}

void karnauhova::areaComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::map< std::string, std::function< double() > > comands;
  comands["EVEN"] = std::bind(evenArea, std::cref(polygons));
  comands["ODD"] = std::bind(oddArea, std::cref(polygons));
  comands["MEAN"] = std::bind(meanArea, std::cref(polygons));
  std::string comand;
  in >> comand;
  double sum = 0;
  try
  {
    sum = comands.at(comand)();
  }
  catch(...)
  {
    size_t count = std::stoull(comand);
    if (count < 3)
    {
      throw std::logic_error("Error: few vertices");
    }
    sum = countArea(polygons, count);
  }
  ScopeGuard scope(out);
  out << std::fixed << std::setprecision(1) << sum << "\n";
}

double karnauhova::evenArea(const std::vector< Polygon >& polygons)
{
  return sumAreas(polygons, isEven);
}

double karnauhova::oddArea(const std::vector< Polygon >& polygons)
{
  return sumAreas(polygons, isOdd);
}

double karnauhova::meanArea(const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("Not enough polygons");
  }
  return sumAreas(polygons, allAreas) / polygons.size();
}

double karnauhova::countArea(const std::vector< Polygon >& polygons, size_t count)
{
  return sumAreas(polygons, CountCompare{ count });
}

void karnauhova::maxComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("Not enough polygons");
  }
  std::map< std::string, std::function< void() > > comands;
  comands["AREA"] = std::bind(maxArea, std::cref(polygons), std::ref(out));
  comands["VERTEXES"] = std::bind(maxVert, std::cref(polygons), std::ref(out));
  std::string comand;
  in >> comand;
  try
  {
    comands.at(comand)();
  }
  catch(...)
  {
    throw std::logic_error("Error in comand");
  }
}

void karnauhova::maxArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  auto max = (*std::max_element(polygons.begin(), polygons.end(), compArea));
  ScopeGuard scope(out);
  out << std::fixed << std::setprecision(1) << karnauhova::getArea(max) << "\n";
}

void karnauhova::maxVert(const std::vector< Polygon >& polygons, std::ostream& out)
{
  auto max = (*std::max_element(polygons.begin(), polygons.end(), compVert));
  ScopeGuard scope(out);
  out << std::fixed << std::setprecision(1) << max.points.size() << "\n";
}

void karnauhova::minComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("Not enough polygons");
  }
  std::map< std::string, std::function< void() > > comands;
  comands["AREA"] = std::bind(minArea, std::cref(polygons), std::ref(out));
  comands["VERTEXES"] = std::bind(minVert, std::cref(polygons), std::ref(out));
  std::string comand;
  in >> comand;
  try
  {
    comands.at(comand)();
  }
  catch(...)
  {
    throw std::logic_error("Error in comand");
  }
}

void karnauhova::minArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  auto min = (*std::min_element(polygons.begin(), polygons.end(), compArea));
  ScopeGuard scope(out);
  out << std::fixed << std::setprecision(1) << karnauhova::getArea(min) << "\n";
}

void karnauhova::minVert(const std::vector< Polygon >& polygons, std::ostream& out)
{
  auto min = (*std::min_element(polygons.begin(), polygons.end(), compVert));
  ScopeGuard scope(out);
  out << std::fixed << std::setprecision(1) << min.points.size() << "\n";
}

void karnauhova::countComands(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::map< std::string, std::function< size_t() > > comands;
  comands["EVEN"] = std::bind(evenCount, std::cref(polygons));
  comands["ODD"] = std::bind(oddCount, std::cref(polygons));
  std::string comand;
  in >> comand;
  size_t size = 0;
  try
  {
    size = comands.at(comand)();
  }
  catch(...)
  {
    size_t count = std::stoull(comand);
    if (count < 3)
    {
      throw std::logic_error("Error: few vertices");
    }
    size = countPol(polygons, count);
  }
  ScopeGuard scope(out);
  out << std::fixed << std::setprecision(1) << size << "\n";
}

size_t karnauhova::evenCount(const std::vector< Polygon >& polygons)
{
  return sumPol(polygons, isEven);
}

size_t karnauhova::oddCount(const std::vector< Polygon >& polygons)
{
  return sumPol(polygons, isOdd);
}

double karnauhova::countPol(const std::vector< Polygon >& polygons, size_t count)
{
  return sumPol(polygons, CountCompare{ count });
}

void karnauhova::lessareaComand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon it;
  in >> it;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t count = std::count_if(polygons.begin(), polygons.end(), std::bind(compArea, std::placeholders::_1, it));
  out << count << "\n";
}

void karnauhova::echoComand(std::istream& in, std::ostream& out, std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("Empty polygons");
  }
  Polygon pol;
  in >> pol;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::vector< Polygon > new_polygons;
  new_polygons.reserve(polygons.size());
  new_polygons = polygons;
  size_t count = 0;
  for (auto it = polygons.begin(); it != polygons.end(); it++)
  {
    if (*it == pol)
    {
      new_polygons.insert(std::find(new_polygons.begin(), new_polygons.end(), *it) + 1, *it);
      count++;
    }
  }
  polygons = std::move(new_polygons);
  out << count << '\n';
}
