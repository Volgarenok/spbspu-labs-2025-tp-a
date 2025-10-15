#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <limits>
#include <functional>
#include <numeric>
#include <stdexcept>
#include "format_guard.hpp"
#include "functional.hpp"
#include <string>

namespace nehvedovich
{
  double getArea(const Polygon &poly)
  {
    return calcPolygonArea(poly);
  }

  struct AreaFilter
  {
    bool operator()(const Polygon &poly, const std::string &param) const
    {
      if (param == "EVEN")
      {
        return poly.points.size() % 2 == 0;
      }
      if (param == "ODD")
      {
        return poly.points.size() % 2 != 0;
      }
      size_t num = std::stoul(param);
      return poly.points.size() == num;
    }
  };

  struct SumAllAreas
  {
    double operator()(double acc, const Polygon &poly) const
    {
      return acc + getArea(poly);
    }
  };

  struct SumAreasIfParam
  {
    explicit SumAreasIfParam(const std::string &p):
      param(p)
    {}

    double operator()(double acc, const Polygon &poly) const
    {
      AreaFilter filter;
      return filter(poly, param) ? (acc + getArea(poly)) : acc;
    }

    std::string param;
  };

  struct VertexCountFilter
  {
    bool operator()(const Polygon &poly, const std::string &param) const
    {
      if (param == "EVEN")
      {
        return poly.points.size() % 2 == 0;
      }
      if (param == "ODD")
      {
        return poly.points.size() % 2 != 0;
      }
      size_t num = std::stoul(param);
      return poly.points.size() == num;
    }
  };

  struct CountByParam
  {
    explicit CountByParam(const std::string &p):
      param(p)
    {}

    bool operator()(const Polygon &poly) const
    {
      VertexCountFilter filter;
      return filter(poly, param);
    }

    std::string param;
  };

  struct LessThanArea
  {
    explicit LessThanArea(double a):
      area(a)
    {}

    bool operator()(const Polygon &p) const
    {
      return getArea(p) < area;
    }

    double area;
  };

}

void nehvedovich::areaCommand(std::istream &in, std::ostream &out, const std::vector< nehvedovich::Polygon > &polygons)
{
  std::string param;
  in >> param;

  if (param == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::runtime_error("No polygons for MEAN calculation");
    }
    const double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, SumAllAreas());
    out << std::fixed << std::setprecision(1) << (total / polygons.size()) << '\n';
    return;
  }

  if (param != "EVEN" && param != "ODD")
  {
    std::size_t num = std::stoul(param);
    if (num < 3)
    {
      throw std::runtime_error("Invalid AREA arg (<3)");
    }
  }

  const double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, SumAreasIfParam(param));
  out << std::fixed << std::setprecision(1) << sum << '\n';
}

void nehvedovich::maxCommand(std::istream &in, std::ostream &out, const std::vector< nehvedovich::Polygon > &polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("No polygons for MAX calculation");
  }

  std::string param;
  in >> param;

  if (param == "AREA")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), AreaComparator());
    out << std::fixed << std::setprecision(1) << getArea(*it) << '\n';
  }
  else if (param == "VERTEXES")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), VerticesComparator());

    out << it->points.size() << '\n';
  }
  else
  {
    throw std::runtime_error("Invalid MAX parameter");
  }
}

void nehvedovich::minCommand(std::istream &in, std::ostream &out, const std::vector< nehvedovich::Polygon > &polygons)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }
  FormatGuard guard(in);
  out << std::fixed << std::setprecision(1);

  if (polygons.empty())
  {
    throw std::runtime_error("No polygons for MIN calculation");
  }

  std::string param;
  in >> param;

  if (param == "AREA")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), AreaComparator());
    out << std::fixed << std::setprecision(1) << getArea(*it) << '\n';
  }
  else if (param == "VERTEXES")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), VerticesComparator());
    out << it->points.size() << '\n';
  }
  else
  {
    throw std::runtime_error("Invalid MIN parameter");
  }
}

void nehvedovich::countCommand(std::istream &in, std::ostream &out, const std::vector< nehvedovich::Polygon > &polygons)
{
  std::string param;
  in >> param;

  if (param != "EVEN" && param != "ODD")
  {
    std::size_t num = std::stoul(param);
    if (num < 3)
    {
      throw std::runtime_error("Invalid COUNT arg (<3)");
    }
  }

  std::size_t count = std::count_if(polygons.begin(), polygons.end(), CountByParam(param));
  out << count << '\n';
}

void nehvedovich::lessAreaCommand(std::istream &in,
                                  std::ostream &out,
                                  const std::vector< nehvedovich::Polygon > &polygons)
{
  nehvedovich::Polygon poly;
  if (!(in >> poly))
  {
    throw std::runtime_error("Invalid polygon for LESSAREA");
  }

  std::string tail;
  std::getline(in, tail);
  if (tail.find_first_not_of(" \t\r") != std::string::npos)
  {
    throw std::runtime_error("Trailing tokens for LESSAREA");
  }

  const double area = getArea(poly);
  const std::size_t count =
      static_cast< std::size_t >(std::count_if(polygons.begin(), polygons.end(), LessThanArea(area)));

  out << count << '\n';
}

void nehvedovich::inFrameCommand(std::istream &in,
                                 std::ostream &out,
                                 const std::vector< nehvedovich::Polygon > &polygons)
{
  if (polygons.empty())
  {
    throw std::runtime_error("No polygons for frame calculation");
  }

  nehvedovich::Polygon poly;
  if (!(in >> poly))
  {
    throw std::runtime_error("Invalid polygon for INFRAME");
  }

  std::string tail;
  std::getline(in, tail);
  if (tail.find_first_not_of(" \t\r") != std::string::npos)
  {
    throw std::runtime_error("Trailing tokens for INFRAME");
  }

  BoundingBox box(polygons);
  out << (box.contains(poly) ? "<TRUE>\n" : "<FALSE>\n");
}

namespace
{
  using namespace nehvedovich;

  struct SubPoint
  {
    explicit SubPoint(const Point &base):
      base_(base)
    {}
    Point operator()(const Point &p) const
    {
      return Point {p.x - base_.x, p.y - base_.y};
    }
    Point base_;
  };

  struct NormalizePolygon
  {
    std::vector< Point > operator()(const Polygon &poly) const
    {
      if (poly.points.empty())
      {
        return std::vector< Point >();
      }
      std::vector< Point >::const_iterator minIt =
          std::min_element(poly.points.begin(), poly.points.end(), PointLess());
      std::vector< Point > rel(poly.points.size());
      std::transform(poly.points.begin(), poly.points.end(), rel.begin(), SubPoint(*minIt));
      std::sort(rel.begin(), rel.end(), PointLess());
      return rel;
    }
  };
}

void nehvedovich::sameCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return;
  }

  Polygon pattern;
  if (!(in >> pattern))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::string tail;
  std::getline(in, tail);
  if (tail.find_first_not_of(" \t\r") != std::string::npos)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const std::vector< Point > key = NormalizePolygon()(pattern);

  struct EqualByNorm
  {
    explicit EqualByNorm(const std::vector< Point > &k):
      key(k)
    {}
    bool operator()(const Polygon &p) const
    {
      if (p.points.size() != key.size())
      {
        return false;
      }
      const std::vector< Point > k2 = NormalizePolygon()(p);
      return std::equal(key.begin(), key.end(), k2.begin());
    }
    const std::vector< Point > &key;
  };

  const std::size_t cnt = static_cast< std::size_t >(std::count_if(polygons.begin(), polygons.end(), EqualByNorm(key)));

  out << cnt << "\n";
}
