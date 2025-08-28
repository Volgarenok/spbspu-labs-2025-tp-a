#include "commands.hpp"
#include "io_polygon.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <functional>
#include <iomanip>
#include <map>
#include <string>
#include <cctype>
#include <stdexcept>
#include <iterator>

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

  bool isRightNumber(const petrov::Polygon & polygon, size_t exp)
  {
    return polygon.points.size() == exp;
  }

  bool isEqPts(const petrov::Point & point_1, const petrov::Point & point_2)
  {
    return point_1.x == point_2.x && point_1.y == point_2.y;
  }

  bool isEqualPolygon(const petrov::Polygon & target, const petrov::Polygon & seq)
  {
    using namespace std::placeholders;
    if (target.points.size() == seq.points.size())
    {
      std::vector< bool > isSimilarPts(seq.points.size());
      std::transform(target.points.cbegin(), target.points.cend(), seq.points.cbegin(), isSimilarPts.begin(), isEqPts);
      return std::none_of(isSimilarPts.cbegin(), isSimilarPts.cend(), std::bind(std::equal_to< bool >{}, _1, false));
    }
    return false;
  }

  bool removeIfTrue(const std::vector< bool > & doRemove, size_t & index)
  {
    return doRemove[index++];
  }

  size_t countDuplicatesSequencesLength(bool doCount, size_t & counter)
  {
    if (doCount)
    {
      return ++counter;
    }
    else
    {
      counter = 0;
      return counter;
    }
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
  auto & pts = polygon.points;
  std::vector< double > trapezes_areas(polygon.points.size() - 1);
  std::transform(pts.cbegin(), pts.cend() - 1, pts.cbegin() + 1, trapezes_areas.begin(), calculateTrapezeArea);
  trapezes_areas.push_back(calculateTrapezeArea(polygon.points[polygon.points.size() - 1], polygon.points[0]));
  double result = std::accumulate(trapezes_areas.cbegin(), trapezes_areas.cend(), 0.0);
  return std::abs(result);
}

void petrov::addIfEven(const std::vector< Polygon > & polygons, std::ostream & out)
{
  using namespace std::placeholders;
  std::vector< Polygon > even_polygons;
  std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(even_polygons), isEven);
  std::vector< double > sum_area(polygons.size());
  std::transform(even_polygons.cbegin(), even_polygons.cend(), sum_area.begin(), calculateArea);
  double result = std::accumulate(sum_area.cbegin(), sum_area.cend(), 0.0);
  out << result;
}

void petrov::addIfOdd(const std::vector< Polygon > & polygons, std::ostream & out)
{
  using namespace std::placeholders;
  std::vector< Polygon > odd_polygons;
  std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(odd_polygons), isOdd);
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

void petrov::sum(size_t & vrtxs_num, const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (vrtxs_num < 3)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  using namespace std::placeholders;
  std::vector< Polygon > proper_polygons;
  auto is_required_num = std::bind(isRightNumber, _1, vrtxs_num);
  std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(proper_polygons), is_required_num);
  std::vector< double > sum_area(proper_polygons.size());
  std::transform(proper_polygons.cbegin(), proper_polygons.cend(), sum_area.begin(), calculateArea);
  double result = std::accumulate(sum_area.cbegin(), sum_area.cend(), 0.0);
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

size_t petrov::getVertexesNum(const Polygon & polygon)
{
  return polygon.points.size();
}

void petrov::max_area(const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard outguard(out);
  std::vector< double > sum_area(polygons.size());
  std::transform(polygons.cbegin(), polygons.cend(), sum_area.begin(), calculateArea);
  out << std::fixed << std::setprecision(1);
  out << *std::max_element(sum_area.cbegin(), sum_area.cend());
}

void petrov::max_vertexes(const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::vector< size_t > vertexes(polygons.size());
  std::transform(polygons.cbegin(), polygons.cend(), vertexes.begin(), getVertexesNum);
  out << *std::max_element(vertexes.cbegin(), vertexes.cend());
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
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard outguard(out);
  std::vector< double > sum_area(polygons.size());
  std::transform(polygons.cbegin(), polygons.cend(), sum_area.begin(), calculateArea);
  out << std::fixed << std::setprecision(1);
  out << *std::min_element(sum_area.cbegin(), sum_area.cend());
}

void petrov::min_vertexes(const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::vector< size_t > vertexes(polygons.size());
  std::transform(polygons.cbegin(), polygons.cend(), vertexes.begin(), getVertexesNum);
  out << *std::min_element(vertexes.cbegin(), vertexes.cend());
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
  out << std::count_if(polygons.cbegin(), polygons.cend(), isEven);
}

void petrov::countIfOdd(const std::vector< Polygon > & polygons, std::ostream & out)
{
  out << std::count_if(polygons.cbegin(), polygons.cend(), isOdd);
}

void petrov::countIfThisNumber(size_t & num_of_vertexes, const std::vector< Polygon > & polygons, std::ostream & out)
{
  if (num_of_vertexes < 3)
  {
    throw std::logic_error("<INVALID COMMAND");
  }
  using namespace std::placeholders;
  out << std::count_if(polygons.cbegin(), polygons.cend(), std::bind(isRightNumber, _1, num_of_vertexes));
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
  using namespace std::placeholders;
  Polygon polygon;
  if (in >> polygon)
  {
    std::vector< bool > do_remove(polygons.size());
    auto isSimilar = std::bind(isEqualPolygon, std::cref(polygon), _2);
    auto isAsPrev = std::bind(isEqualPolygon, _2, _1);
    auto andAnd = std::bind(std::logical_and< bool >{}, isSimilar, isAsPrev);
    std::transform(polygons.cbegin(), polygons.cend() - 1, polygons.cbegin() + 1, do_remove.begin(), andAnd);
    size_t bool_vector_it = 0;
    auto confirmRemove = std::bind(removeIfTrue, std::cref(do_remove), std::ref(bool_vector_it));
    const auto no_space_end = std::remove_if(polygons.begin(), polygons.end(), confirmRemove);
    polygons.erase(no_space_end, polygons.end());
    out << std::count(do_remove.cbegin(), do_remove.cend(), true);
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}

void petrov::maxseq(const std::vector< Polygon > & polygons, std::istream & in, std::ostream & out)
{
  using namespace std::placeholders;
  Polygon polygon;
  if (in >> polygon)
  {
    std::vector< bool > is_part_of_seq(polygons.size());
    auto isSimilar = std::bind(isEqualPolygon, std::cref(polygon), _2);
    auto isAsPrev = std::bind(isEqualPolygon, _2, _1);
    auto andAnd = std::bind(std::logical_and< bool >{}, isSimilar, isAsPrev);
    std::transform(polygons.cbegin(), polygons.cend() - 1, polygons.cbegin() + 1, is_part_of_seq.begin(), andAnd);
    std::vector< size_t > sequences_length(is_part_of_seq.size());
    size_t count = 0;
    auto counter = std::bind(countDuplicatesSequencesLength, _1, std::ref(count));
    std::transform(is_part_of_seq.cbegin(), is_part_of_seq.cend(), sequences_length.begin(), counter);
    size_t max_count = *std::max_element(sequences_length.cbegin(), sequences_length.cend());
    if (max_count)
    {
      out << ++max_count;
    }
    else if (std::count_if(polygons.cbegin(), polygons.cend(), std::bind(isEqualPolygon, std::cref(polygon), _1)))
    {
      out << 1;
    }
    else
    {
      out << 0;
    }
  }
  else
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
}
