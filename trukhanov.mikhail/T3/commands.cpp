#include "commands.hpp"
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <iomanip>
#include <stream_guard.hpp>
#include "functors.hpp"
#include "polygon.hpp"
#include "data_input.hpp"

void trukhanov::area(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::showpoint << std::setprecision(1);

  if (subcommand == "EVEN")
  {
    std::vector<Polygon> filtered;
    std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), isEven);

    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);

    double total = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << total << '\n';
  }
  else if (subcommand == "ODD")
  {
    std::vector<Polygon> filtered;
    std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), isOdd);

    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);

    double total = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << total << '\n';
  }
  else if (subcommand == "MEAN")
  {
    if (src.empty())
    {
      throw std::invalid_argument("Unknown subcommand");
    }
    else
    {
      std::vector< double > areas(src.size());
      std::transform(src.begin(), src.end(), areas.begin(), getArea);

      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      out << (total / static_cast< double >(areas.size())) << '\n';
    }
  }
  else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    size_t size = std::stoull(subcommand);
    if (size < 3)
    {
      throw std::invalid_argument("Unknown subcommand");
    }
    else
    {
      std::vector< Polygon > filtered;
      std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), isSize{ size });

      std::vector< double > areas(filtered.size());
      std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);

      double total = std::accumulate(areas.begin(), areas.end(), 0.0);
      out << total << '\n';
    }
  }
  else
  {
    throw std::invalid_argument("Unknown subcommand");
  }
}

void trukhanov::max(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::showpoint << std::setprecision(1);

  if (subcommand.empty() || subcommand == "AREA")
  {
    if (src.empty())
    {
      throw std::invalid_argument("Unknown subcommand");
    }
    out << getArea(*std::max_element(src.begin(), src.end(), CompareByArea())) << '\n';
  }
  else if (subcommand == "VERTEXES")
  {
    if (src.empty())
    {
      throw std::invalid_argument("Unknown subcommand");
    }
    out << std::max_element(src.begin(), src.end(), CompareByVertexes())->points.size() << '\n';
  }
  else
  {
    throw std::invalid_argument("Unknown subcommand");
  }
}

void trukhanov::min(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::showpoint << std::setprecision(1);

  if (subcommand.empty() || subcommand == "AREA")
  {
    if (src.empty())
    {
      throw std::invalid_argument("Unknown subcommand");
    }
    out << getArea(*std::min_element(src.begin(), src.end(), CompareByArea())) << '\n';
  }
  else if (subcommand == "VERTEXES")
  {
    if (src.empty())
    {
      throw std::invalid_argument("Unknown subcommand");
    }
    out << std::min_element(src.begin(), src.end(), CompareByVertexes())->points.size() << '\n';
  }
  else
  {
    throw std::invalid_argument("Unknown subcommand");
  }
}

void trukhanov::count(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  std::string subcommand;
  in >> subcommand;

  std::vector< Polygon > filtered;
  std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), PolygonHasMinSize{});

  if (subcommand == "EVEN")
  {
    out << std::count_if(filtered.begin(), filtered.end(), isEven) << '\n';
  }
  else if (subcommand == "ODD")
  {
    out << std::count_if(filtered.begin(), filtered.end(), isOdd) << '\n';
  }
  else if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    size_t size = std::stoull(subcommand);
    if (size < 3)
    {
      throw std::invalid_argument("Unknown subcommand");
    }
    else
    {
      out << std::count_if(filtered.begin(), filtered.end(), isSize{ size }) << '\n';
    }
  }
  else
  {
    throw std::invalid_argument("Unknown subcommand");
  }
}

void trukhanov::lessArea(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  Polygon polygon;
  in >> polygon;

  HasDuplicates checkDuplicates;
  if (!in || polygon.points.size() < 3 || !checkDuplicates(polygon))
  {
    throw std::invalid_argument("Unknown subcommand");
  }

  double refArea = getArea(polygon);
  size_t count = std::count_if(src.begin(), src.end(), LessArea{ refArea });
  out << count << '\n';
}

void trukhanov::right(std::istream&, std::ostream& out, const std::vector < Polygon >& src)
{
  out << std::count_if(src.begin(), src.end(), isRight) << '\n';
}
