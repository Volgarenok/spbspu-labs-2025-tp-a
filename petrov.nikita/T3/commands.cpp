#include "commands.hpp"
#include "io_polygon.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <map>
#include <string>
#include <cctype>
#include <stdexcept>

namespace
{
  bool isOdd(const petrov::Polygon & polygon)
  {
    return polygon.points.size() % 2 == 1;
  }

  bool isEven(const petrov::Polygon & polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isDigit(std::string & string)
  {
    bool is_digit = true;
    for (size_t i = 0; i < string.size() && is_digit; i++)
    {
      if (!bool(isdigit(string[i])))
      {
        is_digit = false;
      }
    }
    return is_digit;
  }
}

double petrov::calculateTrapezeArea(const Point & point_1, const Point & point_2)
{
  return 0.5 * (point_1.y + point_2.y) * (point_2.x - point_1.x);
}

double petrov::calculateArea(const Polygon & polygon)
{
  std::vector< double > trapezes_areas(polygon.points.size() - 1);
  std::transform(polygon.points.cbegin(), polygon.points.cend(), polygon.points.cbegin() + 1, trapezes_areas.begin(), calculateTrapezeArea);
  trapezes_areas.push_back(calculateTrapezeArea(polygon.points[polygon.points.size() - 1], polygon.points[0]));
  double result = std::accumulate(trapezes_areas.cbegin(), trapezes_areas.cend(), 0.0);
  return std::abs(result);
}

void petrov::addIfEven(const std::vector< Polygon > & polygons, std::ostream & out)
{
  using namespace std::placeholders;
  std::vector< Polygon > even_polygons;
  std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(even_polygons), std::bind(isEven, _1));
  std::vector< double > sum_area(even_polygons.size());
  std::transform(even_polygons.cbegin(), even_polygons.cend(), sum_area.begin(), calculateArea);
  double result = std::accumulate(sum_area.cbegin(), sum_area.cend(), 0.0);
  out << result;
}

void petrov::addIfOdd(const std::vector< Polygon > & polygons, std::ostream & out)
{
  using namespace std::placeholders;
  std::vector< Polygon > odd_polygons;
  std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(odd_polygons), std::bind(isOdd, _1));
  std::vector< double > sum_area(odd_polygons.size());
  std::transform(odd_polygons.cbegin(), odd_polygons.cend(), sum_area.begin(), calculateArea);
  double result = std::accumulate(sum_area.cbegin(), sum_area.cend(), 0.0);
  out << result;
}

void petrov::mean(const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::vector< double > sum_area(polygons.size());
  std::transform(polygons.cbegin(), polygons.cend(), sum_area.begin(), calculateArea);
  double result = std::accumulate(sum_area.cbegin(), sum_area.cend(), 0.0);
  out << result / polygons.size();
}

void petrov::sum(size_t & num_of_vertexes, const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (num_of_vertexes < 3)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  double result = 0.0;
  for (size_t i = 0; i < polygons.size(); i++)
  {
    if (polygons[i].points.size() == num_of_vertexes)
    {
      result += calculateArea(polygons[i]);
    }
  }
  out << result;
}

void petrov::area(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::map< std::string, std::function< void() > > subcmds;
  size_t num_of_vertexes = 0;
  StreamGuard outguard(out);
  out << std::fixed << std::setprecision(1);
  subcmds["EVEN"] = std::bind(addIfEven, std::cref(polygons), std::ref(out));
  subcmds["ODD"] = std::bind(addIfOdd, std::cref(polygons), std::ref(out));
  subcmds["MEAN"] = std::bind(mean, std::cref(polygons), std::ref(out));
  subcmds["SUM"] = std::bind(sum, std::ref(num_of_vertexes), std::cref(polygons), std::ref(out));
  std::string subcommand;
  if (in >> subcommand)
  {
    if (isDigit(subcommand))
    {
      char * p_end;
      num_of_vertexes = strtoull(subcommand.c_str(), &p_end, subcommand.size() - 1);
      subcmds["SUM"]();
    }
    else
    {
      try
      {
        subcmds.at(subcommand)();
      }
      catch (...)
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
    }
  }
}

void petrov::max_area(const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  double result = calculateArea(polygons[0]);
  StreamGuard outguard(out);
  out << std::fixed << std::setprecision(1);
  for (size_t i = 1; i < polygons.size(); i++)
  {
    result = std::max(calculateArea(polygons[i]), result);
  }
  out << result;
}

void petrov::max_vertexes(const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t max_vertexes = polygons[0].points.size();
  for (size_t i = 1; i < polygons.size(); i++)
  {
    max_vertexes = std::max(polygons[i].points.size(), max_vertexes);
  }
  out << max_vertexes;
}

void petrov::max(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(max_area, std::cref(polygons), std::ref(out));
  subcmds["VERTEXES"] = std::bind(max_vertexes, std::cref(polygons), std::ref(out));
  std::string subcommand;
  if (in >> subcommand)
  {
    try
    {
      subcmds.at(subcommand)();
    }
    catch (...)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
}

void petrov::min_area(const std::vector< Polygon > & polygons, std::ostream & out)
{
  double result = calculateArea(polygons[0]);
  StreamGuard outguard(out);
  out << std::fixed << std::setprecision(1);
  for (size_t i = 1; i < polygons.size(); i++)
  {
    result = std::min(calculateArea(polygons[i]), result);
  }
  out << result;
}

void petrov::min_vertexes(const std::vector< Polygon > & polygons, std::ostream & out)
{
  size_t min_vertexes = polygons[0].points.size();
  for (size_t i = 1; i < polygons.size(); i++)
  {
    min_vertexes = std::min(polygons[i].points.size(), min_vertexes);
  }
  out << min_vertexes;
}

void petrov::min(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(min_area, std::cref(polygons), std::ref(out));
  subcmds["VERTEXES"] = std::bind(min_vertexes, std::cref(polygons), std::ref(out));
  std::string subcommand;
  if (in >> subcommand)
  {
    try
    {
      subcmds.at(subcommand)();
    }
    catch (...)
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
  }
}

void petrov::countIfEven(const std::vector< Polygon > & polygons, std::ostream & out)
{
  size_t count = 0;
  for (size_t i = 0; i < polygons.size(); i++)
  {
    if (polygons[i].points.size() % 2 == 0)
    {
      count++;
    }
  }
  out << count;
}

void petrov::countIfOdd(const std::vector< Polygon > & polygons, std::ostream & out)
{
  size_t count = 0;
  for (size_t i = 0; i < polygons.size(); i++)
  {
    if (polygons[i].points.size() % 2 == 1)
    {
      count++;
    }
  }
  out << count;
}

void petrov::countIfThisNumber(size_t & num_of_vertexes, const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (num_of_vertexes < 3)
  {
    throw std::logic_error("<INVALID COMMAND");
  }
  size_t count = 0;
  for (size_t i = 0; i < polygons.size(); i++)
  {
    if (polygons[i].points.size() == num_of_vertexes)
    {
      count++;
    }
  }
  out << count;
}

void petrov::count(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  std::map< std::string, std::function< void() > > subcmds;
  size_t num_of_vertexes = 0;
  StreamGuard outguard(out);
  out << std::fixed << std::setprecision(1);
  subcmds["EVEN"] = std::bind(countIfEven, std::cref(polygons), std::ref(out));
  subcmds["ODD"] = std::bind(countIfOdd, std::cref(polygons), std::ref(out));
  subcmds["NUM"] = std::bind(countIfThisNumber, std::ref(num_of_vertexes), std::cref(polygons), std::ref(out));
  std::string subcommand;
  if (in >> subcommand)
  {
    if (isDigit(subcommand))
    {
      char * p_end;
      num_of_vertexes = strtoull(subcommand.c_str(), &p_end, subcommand.size() - 1);
      subcmds["NUM"]();
    }
    else
    {
      try
      {
        subcmds.at(subcommand)();
      }
      catch (...)
      {
        throw std::logic_error("<INVALID COMMAND>");
      }
    }
  }
}

void petrov::rmecho(std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  size_t current_count = 0;
  size_t deleted_count = 0;
  Polygon polygon;
  if (in >> polygon)
  {
    for (size_t i = 0; i < polygons.size(); i++)
    {
      bool isEqual = true;
      for (size_t j = 0; j < polygons[i].points.size() && isEqual; j++)
      {
        if (!(polygon.points[j].x == polygons[i].points[j].x && polygon.points[j].y == polygons[i].points[j].y))
        {
          isEqual = false;
        }
      }
      if (isEqual == true)
      {
        current_count++;
        if (current_count > 1)
        {
          polygons.erase(polygons.begin() + i);
          deleted_count++;
        }
      }
      else
      {
        current_count = 0;
      }
    }
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  out << deleted_count;
}

void petrov::maxseq(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  size_t current_count = 0;
  size_t max_count = 0;
  Polygon polygon;
  if (in >> polygon)
  {
    for (size_t i = 0; i < polygons.size(); i++)
    {
      bool isEqual = true;
      for (size_t j = 0; j < polygons[i].points.size() && isEqual; j++)
      {
        if (!(polygon.points[j].x == polygons[i].points[j].x && polygon.points[j].y == polygons[i].points[j].y))
        {
          isEqual = false;
        }
      }
      if (isEqual == true)
      {
        current_count++;
      }
      else
      {
        if (max_count < current_count)
        {
          max_count = current_count;
        }
        current_count = 0;
      }
    }
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  out << max_count;
}

