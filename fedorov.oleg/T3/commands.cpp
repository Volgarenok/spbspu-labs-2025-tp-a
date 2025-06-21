#include "commands.hpp"

#include <iomanip>
#include <numeric>
#include <algorithm>

#include "functional.hpp"

namespace fedorov
{
  template < typename Comp, bool isMax >
  void processExtremum(std::ostream &out, const std::vector< Polygon > &polys, Comp comp, bool isArea)
  {
    if (polys.empty())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    auto it =
        isMax ? std::max_element(polys.begin(), polys.end(), comp) : std::min_element(polys.begin(), polys.end(), comp);

    if (isArea)
    {
      out << std::fixed << std::setprecision(1) << calcPolygonArea(*it) << '\n';
    }
    else
    {
      out << it->points.size() << '\n';
    }
  }

  void maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    std::string param;
    if (!(in >> param))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (param == "AREA")
    {
      processExtremum< AreaComparator, true >(out, polys, AreaComparator(), true);
    }
    else if (param == "VERTEXES")
    {
      processExtremum< VerticesComparator, true >(out, polys, VerticesComparator(), false);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    std::string param;
    if (!(in >> param))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (param == "AREA")
    {
      processExtremum< AreaComparator, false >(out, polys, AreaComparator(), true);
    }
    else if (param == "VERTEXES")
    {
      processExtremum< VerticesComparator, false >(out, polys, VerticesComparator(), false);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (param != "ODD" && param != "EVEN" && param != "MEAN")
    {
      try
      {
        size_t num = std::stoul(param);
        if (num < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
      }
      catch (...)
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
    }

    try
    {
      double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaAccumulator(param));

      if (param == "MEAN")
      {
        if (polygons.empty())
        {
          throw std::logic_error("No polygons");
        }
        total /= polygons.size();
      }

      out << std::fixed << std::setprecision(1) << total << '\n';
    }
    catch (...)
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void LessAreaCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons)
  {
    std::string line;
    std::getline(in >> std::ws, line);

    try
    {
      LineToPolygonConverter converter;
      Polygon target = converter(Line{line});
      if (target.points.size() < 3)
      {
        throw std::invalid_argument("Invalid polygon");
      }

      double threshold = calcPolygonArea(target);
      size_t count = countIf(polygons, LessAreaPredicate(threshold));
      out << count << '\n';
    }
    catch (...)
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void EchoCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons)
  {
    std::string line;
    std::getline(in >> std::ws, line);

    try
    {
      LineToPolygonConverter converter;
      Polygon target = converter(Line{line});
      if (target.points.size() < 3)
      {
        throw std::invalid_argument("Invalid polygon");
      }

      size_t initial = polygons.size();
      processEcho(polygons, target);
      out << (polygons.size() - initial) << '\n';
    }
    catch (...)
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (param != "ODD" && param != "EVEN")
    {
      try
      {
        size_t num = std::stoul(param);
        if (num < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
      }
      catch (...)
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
    }

    try
    {
      size_t count = std::accumulate(polygons.begin(), polygons.end(), 0u, CountAccumulator(param));
      out << count << '\n';
    }
    catch (...)
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}
