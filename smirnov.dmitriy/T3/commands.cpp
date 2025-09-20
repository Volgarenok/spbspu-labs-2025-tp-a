#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "polygon.hpp"
#include "streamguard.hpp"

namespace smirnov
{
  using namespace std::placeholders;

  template < class UnaryPredicate >
  double getSumArea(const std::vector< Polygon >& polygons, UnaryPredicate P);
  double doAreaEven(const std::vector< Polygon >& polygons);
  double doAreaOdd(const std::vector< Polygon >& polygons);
  double doAreaMean(const std::vector< Polygon >& polygons);
  double doAreaNum(const std::vector< Polygon >& polygons, size_t n);

  void doAreaCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::string s;
    in >> s;
    double result = 0.0;
    ScopeGuard streamGuard(out);
    out << std::fixed << std::setprecision(1);
    std::map< std::string, std::function< double() > > subcommand;
    {
      using namespace std::placeholders;
      subcommand["EVEN"] = std::bind(doAreaEven, polygons);
      subcommand["ODD"] = std::bind(doAreaOdd, polygons);
      subcommand["MEAN"] = std::bind(doAreaMean, polygons);
    }
    try
    {
      size_t n = std::stoull(s);
      if (n < 3)
      {
        throw std::logic_error("FEW VERTEXES");
      }
    }
    catch (const std::invalid_argument&)
    {
      result = subcommand[s]();
    }
    std::vector< double > areas;
    out << result << "\n";
  }

  void doMinMaxCommand(
    const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out, const std::string& name
  )
  {
    std::string s;
    in >> s;
    if (polygons.empty())
    {
      throw std::logic_error("zero polygons");
    }

    ScopeGuard streamGuard(out);
    out << std::fixed << std::setprecision(1);

    if (s == "AREA")
    {
      if (name == "max")
      {
        out << getArea(*std::max_element(polygons.begin(), polygons.end(), minArea)) << "\n";
      }
      else
      {
        out << getArea(*std::min_element(polygons.begin(), polygons.end(), minArea)) << "\n";
      }
    }
    else if (s == "VERTEXES")
    {
      if (name == "max")
      {
        out << (*std::max_element(polygons.begin(), polygons.end(), minVertexes)).points.size() << "\n";
      }
      else
      {
        out << (*std::min_element(polygons.begin(), polygons.end(), minVertexes)).points.size() << "\n";
      }
    }
    else
    {
      throw std::logic_error("INVALID COMMAND");
    }
  }

  void doMaxCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    doMinMaxCommand(polygons, in, out, "max");
  }

  void doMinCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    doMinMaxCommand(polygons, in, out, "min");
  }

  void doCountCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::string s;
    in >> s;
    if (s == "EVEN")
    {
      out << std::count_if(polygons.begin(), polygons.end(), isEvenCountVertexes) << "\n";
    }
    else if (s == "ODD")
    {
      out << std::count_if(polygons.begin(), polygons.end(), isOddCountVertexes) << "\n";
    }
    else if (s == std::to_string(std::stoi(s)))
    {
      size_t n = std::stoi(s);
      if (n < 3)
      {
        throw std::logic_error("FEW VERTEXES");
      }
      else
      {
        out << std::count_if(polygons.begin(), polygons.end(), std::bind(isNCountVertexes, _1, n)) << "\n";
      }
    }
    else
    {
      throw std::logic_error("INVALID COMMAND");
    }
  }

  void doRectsCommand(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isRect) << "\n";
  }

  struct PolygonMaxSeq
  {
    size_t cur;
    size_t maxseq;
    bool operator()(const Polygon& polygon, const Polygon& data);
  };

  void doMaxseqCommand(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    smirnov::Polygon data;
    in >> data;
    if (data.points.size() < 3)
    {
      throw std::logic_error("INVALID COMMAND");
    }
    else
    {
      PolygonMaxSeq seq{ 0, 0 };
      size_t numberSeq = std::count_if(polygons.begin(), polygons.end(), std::bind(std::ref(seq), _1, data));
      if (numberSeq < 1)
      {
        throw std::logic_error("INVALID COMMAND");
      }
      else
      {
        out << seq.maxseq << "\n";
      }
    }
  }

  bool hasIntersection(const Polygon& p1, const Polygon& p2)
  {
    auto pair_p1 = std::minmax_element(p1.points.cbegin(), p1.points.cend());
    auto pair_p2 = std::minmax_element(p2.points.cbegin(), p2.points.cend());
    return !(*pair_p1.second < *pair_p2.first || *pair_p2.second < *pair_p1.first);
  }

  void doIntersections(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
  {
    Polygon polygon;
    in >> polygon;
    if (!in)
    {
      throw std::logic_error("Wrong argument");
    }

    out << std::count_if(data.begin(), data.end(), std::bind(hasIntersection, std::placeholders::_1, polygon)) << '\n';
  }

  bool PolygonMaxSeq::operator()(const Polygon& polygon, const Polygon& data)
  {
    if (polygon == data)
    {
      cur++;
      maxseq = std::max(maxseq, cur);
    }
    else
    {
      cur = 0;
    }
    return maxseq;
  }

  template < class UnaryPredicate >
  double getSumArea(const std::vector< Polygon >& polygons, UnaryPredicate P)
  {
    std::vector< Polygon > rightPolygons;
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(rightPolygons), P);
    std::vector< double > areas;
    std::transform(rightPolygons.cbegin(), rightPolygons.cend(), std::back_inserter(areas), getArea);
    double result = std::accumulate(areas.cbegin(), areas.cend(), 0.0);
    return result;
  }

  double doAreaEven(const std::vector< Polygon >& polygons)
  {
    return getSumArea(polygons, isEvenCountVertexes);
  }

  double doAreaOdd(const std::vector< Polygon >& polygons)
  {
    return getSumArea(polygons, isOddCountVertexes);
  }

  double doAreaMean(const std::vector< Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("NO POLYGONS FOR AREA MEAN COMMAND");
    }
    std::vector< double > areas;
    std::transform(polygons.cbegin(), polygons.cend(), std::back_inserter(areas), getArea);
    double result = std::accumulate(areas.cbegin(), areas.cend(), 0.0);
    result /= polygons.size();
    return result;
  }

  double doAreaNum(const std::vector< Polygon >& polygons, size_t n)
  {
    return getSumArea(polygons, std::bind(isNCountVertexes, _1, n));
  }
}
