#include "commands.hpp"
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <data_input.hpp>
#include <stream_guard.hpp>
#include "polygon_utils.hpp"
#include "polygon.hpp"

void areaEven(std::ostream& out, const std::vector < trukhanov::Polygon >& src)
{
  std::vector< trukhanov::Polygon > filtered;
  std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), trukhanov::isEven);

  std::vector< double > areas(filtered.size());
  std::transform(filtered.begin(), filtered.end(), areas.begin(), trukhanov::getArea);

  double total = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << total << '\n';
}

void areaOdd(std::ostream& out, const std::vector < trukhanov::Polygon >& src)
{
  std::vector< trukhanov::Polygon > filtered;
  std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), trukhanov::isOdd);

  std::vector< double > areas(filtered.size());
  std::transform(filtered.begin(), filtered.end(), areas.begin(), trukhanov::getArea);

  double total = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << total << '\n';
}

void areaMean(std::ostream& out, const std::vector < trukhanov::Polygon >& src)
{
  if (src.empty())
  {
    throw std::invalid_argument("Unknown subcommand");
  }
  else
  {
    std::vector< double > areas(src.size());
    std::transform(src.begin(), src.end(), areas.begin(), trukhanov::getArea);

    double total = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << (total / static_cast<double>(areas.size())) << '\n';
  }
}

struct AreaVertexes
{
  const std::vector< trukhanov::Polygon >& src;
  std::ostream& out;
  size_t n;

  void operator()() const
  {
    if (n < 3)
    {
      throw std::invalid_argument("Unknown subcommand");
    }

    std::vector< trukhanov::Polygon > filtered;
    std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), trukhanov::isSize{ n });

    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), trukhanov::getArea);

    double total = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << total << '\n';
  }
};

void maxArea(std::ostream& out, const std::vector < trukhanov::Polygon >& src)
{
  if (src.empty())
  {
    throw std::invalid_argument("Unknown subcommand");
  }
  out << getArea(*std::max_element(src.begin(), src.end(), trukhanov::CompareByArea)) << '\n';
}

void maxVertexes(std::ostream& out, const std::vector < trukhanov::Polygon >& src)
{
  if (src.empty())
  {
    throw std::invalid_argument("Unknown subcommand");
  }
  out << std::max_element(src.begin(), src.end(), trukhanov::compareByVertexes)->points.size() << '\n';
}

void minArea(std::ostream& out, const std::vector < trukhanov::Polygon >& src)
{
  if (src.empty())
  {
    throw std::invalid_argument("Unknown subcommand");
  }
  out << getArea(*std::min_element(src.begin(), src.end(), trukhanov::CompareByArea)) << '\n';
}

void minVertexes(std::ostream& out, const std::vector < trukhanov::Polygon >& src)
{
  if (src.empty())
  {
    throw std::invalid_argument("Unknown subcommand");
  }
  out << std::min_element(src.begin(), src.end(), trukhanov::compareByVertexes)->points.size() << '\n';
}

void countEven(std::ostream& out, const std::vector < trukhanov::Polygon >& src)
{
  std::vector< trukhanov::Polygon > filtered;
  std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), trukhanov::PolygonHasMinSize);
  out << std::count_if(filtered.begin(), filtered.end(), trukhanov::isEven) << '\n';
}

void countOdd(std::ostream& out, const std::vector < trukhanov::Polygon >& src)
{
  std::vector< trukhanov::Polygon > filtered;
  std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), trukhanov::PolygonHasMinSize);
  out << std::count_if(filtered.begin(), filtered.end(), trukhanov::isOdd) << '\n';
}

struct ShowCommonPoint
{
  const std::vector< trukhanov::Polygon >& polygons;
  std::ostream& out;

  void operator()() const
  {
    bool result = trukhanov::hasCommonPoint(polygons);
    out << (result ? "YES\n" : "NO\n");
  }
};

struct ShowCommonSide
{
  const std::vector< trukhanov::Polygon >& polygons;
  std::ostream& out;

  void operator()() const
  {
    if (polygons.size() < 2)
    {
      out << "NO\n";
      return;
    }
    bool result = trukhanov::CheckAllSides(polygons)(0);
    out << (result ? "YES\n" : "NO\n");
  }
};

struct CountVertexes
{
  const std::vector< trukhanov::Polygon >& src;
  std::ostream& out;
  size_t n;

  void operator()() const
  {
    if (n < 3)
    {
      throw std::invalid_argument("Unknown subcommand");
    }
    else
    {
      std::vector< trukhanov::Polygon > filtered;
      std::copy_if(src.begin(), src.end(), std::back_inserter(filtered), trukhanov::PolygonHasMinSize);
      out << std::count_if(filtered.begin(), filtered.end(), trukhanov::isSize{ n }) << '\n';
    }
  }
};

void trukhanov::area(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::showpoint << std::setprecision(1);

  std::map< std::string, std::function< void() > > subcommands;
  subcommands["EVEN"] = std::bind(areaEven, std::ref(out), std::cref(src));
  subcommands["ODD"] = std::bind(areaOdd, std::ref(out), std::cref(src));
  subcommands["MEAN"] = std::bind(areaMean, std::ref(out), std::cref(src));
  if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    size_t n = std::stoull(subcommand);
    AreaVertexes{ src, out, n }();
  }
  else
  {
    subcommands.at(subcommand)();
  }
}

void trukhanov::max(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::showpoint << std::setprecision(1);

  std::map< std::string, std::function<void() > > subcommands;
  subcommands["AREA"] = std::bind(maxArea, std::ref(out), std::cref(src));
  subcommands["VERTEXES"] = std::bind(maxVertexes, std::ref(out), std::cref(src));
  subcommands.at(subcommand)();
}

void trukhanov::min(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  StreamGuard guard(out);
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::showpoint << std::setprecision(1);

  std::map< std::string, std::function< void() > > subcommands;
  subcommands["AREA"] = std::bind(minArea, std::ref(out), std::cref(src));
  subcommands["VERTEXES"] = std::bind(minVertexes, std::ref(out), std::cref(src));
  subcommands.at(subcommand)();
}

void trukhanov::count(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  std::string subcommand;
  in >> subcommand;

  std::map< std::string, std::function< void() > > subcommands;
  subcommands["EVEN"] = std::bind(countEven, std::ref(out), std::cref(src));
  subcommands["ODD"] = std::bind(countOdd, std::ref(out), std::cref(src));
  if (std::all_of(subcommand.begin(), subcommand.end(), ::isdigit))
  {
    size_t n = std::stoull(subcommand);
    CountVertexes{ src, out, n }();
  }
  else
  {
    subcommands.at(subcommand)();
  }
}

void trukhanov::commonSide(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  std::string subcommand;
  in >> subcommand;

  out << std::fixed << std::showpoint << std::setprecision(1);

  std::map< std::string, std::function< void() > > subcommands;
  subcommands["SIDE"] = ShowCommonSide{ src, out };
  subcommands["VERTEXES"] = ShowCommonPoint{ src, out };
  subcommands.at(subcommand)();
}

void trukhanov::lessArea(std::istream& in, std::ostream& out, const std::vector < Polygon >& src)
{
  Polygon polygon;
  in >> polygon;

  if (!in || polygon.points.size() < 3 || !HasDuplicates(polygon))
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
