#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <format_guard.hpp>

void puzikov::checkVerticesParameter(const std::string &param)
{
  try
  {
    std::size_t verticesCount = std::stoul(param);
    if (verticesCount < 3)
    {
      throw std::logic_error("Not enough vertices.");
    }
  }
  catch (const std::invalid_argument &)
  {
    return;
  }
  catch (const std::out_of_range &)
  {
    return;
  }
}

void puzikov::areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }
  FormatGuard guard(in);
  out << std::fixed << std::setprecision(1);

  std::string param;
  if (!(in >> param))
  {
    return;
  }

  double areaSum;
  checkVerticesParameter(param);
  areaSum = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaAccumulator(param));

  if (param == "MEAN")
  {
    if (polygons.size() != 0)
    {
      out << areaSum / polygons.size() << '\n';
    }
    else
    {
      throw std::logic_error("No mean area when no polygons.");
    }
  }
  else
  {
    out << areaSum << '\n';
  }
}

void puzikov::minmaxCommand(
    std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons, AreaAlgo areaAlgo, VertAlgo vertAlgo)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }
  FormatGuard guard(in);
  out << std::fixed << std::setprecision(1);

  std::string param;
  if (!(in >> param))
  {
    return;
  }

  if (param == "AREA")
  {
    if (polygons.empty())
    {
      throw std::logic_error("Can't count area if no polygons.");
    }
    auto it = areaAlgo(polygons.begin(), polygons.end(), AreaComparator);
    out << calcPolygonArea(*it) << '\n';
  }
  else if (param == "VERTEXES")
  {
    if (polygons.empty())
    {
      throw std::logic_error("Can't count vertices if no polygons.");
    }
    auto it = vertAlgo(polygons.begin(), polygons.end(), VerticesComparator);
    out << it->points.size() << '\n';
  }
  else
  {
    throw std::logic_error("Wrong min/max command");
  }
}

void puzikov::maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  minmaxCommand(in, out, polygons, maxAreaElement, maxVertElement);
}

void puzikov::minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  minmaxCommand(in, out, polygons, minAreaElement, minVertElement);
}

void puzikov::countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }

  std::string param;
  if (!(in >> param))
  {
    return;
  }

  checkVerticesParameter(param);
  unsigned counter = std::accumulate(polygons.begin(), polygons.end(), 0, ShapesAccumulator(param));

  out << counter << '\n';
}

void puzikov::rmEchoCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }

  Polygon reference;
  if (!(in >> reference))
  {
    throw std::logic_error("Invalid polygon.");
  }

  auto last = std::unique(polygons.begin(), polygons.end(), RmEchoPredicate{reference});
  unsigned counter = std::distance(last, polygons.end());
  polygons.erase(last, polygons.end());
  out << counter << '\n';
}

void puzikov::sameCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return;
  }

  Polygon reference;
  try
  {
    in >> reference;
  }
  catch (...)
  {
    throw std::runtime_error("WHOOPS");
  }
  if (!in || in.peek() != '\n')
  {
    throw std::logic_error("wrong polygon");
  }
  out << std::count_if(polygons.begin(), polygons.end(), IsTranslationCongruent(reference)) << '\n';
}
