#include "vertex_processing.hpp"
#include <algorithm>
#include <numeric>
#include <functional>
#include <exception>
#include "utilities.hpp"

void zakirov::count(const std::list< Polygon > & plgns, std::istream & in, std::ostream & out)
{
  std::string subcommand;
  in >> subcommand;
  if (subcommand == "EVEN")
  {
    out << std::count_if(plgns.begin(), plgns.end(), even_polygon_pred);
  }
  else if (subcommand == "ODD")
  {
    out << std::count_if(plgns.begin(), plgns.end(), odd_polygon_pred);
  }
  else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    size_t a = std::stoull(subcommand);
    if (a < 3)
    {
      throw std::logic_error("Division by zero");
    }

    out << std::count_if(plgns.begin(), plgns.end(), std::bind(equal_vertexes_pred, std::placeholders::_1, a));
  }
}

void zakirov::echo(std::list< Polygon > & plgns, std::istream & in, std::ostream & out)
{
  std::string subcommand;
  in >> subcommand;
  if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    size_t count_vertexes = std::stoull(subcommand);
    if (count_vertexes < 3)
    {
      throw std::invalid_argument("Invalid figure");
    }

    Point pnt;
    Polygon plgn;
    while (in.peek() != '\n' && in >> pnt)
    {
      plgn.points_.push_back(pnt);
    }

    if (in.fail() || plgn.points_.size() != count_vertexes)
    {
      throw std::invalid_argument("Invalid figure");
    }

    size_t counter = 0;
    for (auto it = plgns.begin(); it != plgns.end(); ++it)
    {
      if (std::equal((*it).points_.begin(), (*it).points_.end(), plgn.points_.begin()))
      {
        plgns.insert(it, plgn);
        ++counter;
      }
    }

    out << counter;
  }
}
