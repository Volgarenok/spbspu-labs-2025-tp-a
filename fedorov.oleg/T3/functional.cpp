#include "functional.hpp"

#include <numeric>
#include <iterator>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <string>

#include "polygon.hpp"

namespace fedorov
{
  double calcPolygonArea(const Polygon &poly)
  {
    if (poly.points.size() < 3)
    {
      return 0.0;
    }

    struct AreaAccumulator
    {
      double operator()(const Point &p1, const Point &p2) const
      {
        return p1.x * p2.y - p2.x * p1.y;
      }
    };

    double sum = 0.0;
    const Point &first = poly.points.front();
    const Point &last = poly.points.back();

    if (poly.points.size() > 1)
    {
      sum = std::inner_product(poly.points.begin(), poly.points.end() - 1, poly.points.begin() + 1, 0.0, std::plus<>(),
                               AreaAccumulator());
    }

    sum += (last.x * first.y - first.x * last.y);

    return std::abs(sum) / 2.0;
  }

  bool LineValidator::operator()(const Line &line) const
  {
    if (line.data.empty())
    {
      return false;
    }

    size_t start = line.data.find_first_not_of(" \t");
    if (start == std::string::npos)
    {
      return false;
    }

    size_t end = line.data.find_first_of(" \t", start);
    std::string token = (end == std::string::npos) ? line.data.substr(start) : line.data.substr(start, end - start);

    for (char c : token)
    {
      if (!std::isdigit(static_cast< unsigned char >(c)))
      {
        return false;
      }
    }
    return true;
  }

  Polygon LineToPolygonConverter::operator()(const Line &line) const
  {
    std::vector< std::string > tokens;
    size_t start = 0;
    size_t end = 0;
    const std::string &data = line.data;

    while (start < data.length())
    {
      start = data.find_first_not_of(" \t", end);
      if (start == std::string::npos)
      {
        break;
      }

      end = data.find_first_of(" \t", start);
      if (end == std::string::npos)
      {
        tokens.push_back(data.substr(start));
        break;
      }
      tokens.push_back(data.substr(start, end - start));
      start = end + 1;
    }

    if (tokens.empty())
    {
      return Polygon();
    }

    size_t numVertices;
    try
    {
      numVertices = std::stoul(tokens[0]);
    }
    catch (...)
    {
      return Polygon();
    }

    if (numVertices < 3 || tokens.size() != numVertices + 1)
    {
      return Polygon();
    }

    Polygon poly;
    for (size_t i = 1; i < tokens.size(); ++i)
    {
      const std::string &token = tokens[i];
      if (token.size() < 5 || token.front() != '(' || token.back() != ')')
      {
        return Polygon();
      }

      std::string content = token.substr(1, token.size() - 2);
      size_t sepPos = content.find(';');
      if (sepPos == std::string::npos)
      {
        return Polygon();
      }

      std::string xStr = content.substr(0, sepPos);
      std::string yStr = content.substr(sepPos + 1);

      try
      {
        int x = std::stoi(xStr);
        int y = std::stoi(yStr);
        poly.points.push_back({x, y});
      }
      catch (...)
      {
        return Polygon();
      }
    }
    return poly;
  }

  bool PolygonValidator::operator()(const Polygon &poly) const
  {
    return poly.points.size() >= 3;
  }

  void readPolygons(std::istream &in, std::vector< Polygon > &polygons)
  {
    polygons.clear();

    std::vector< Line > lines;
    std::copy(std::istream_iterator< Line >(in), std::istream_iterator< Line >(), std::back_inserter(lines));

    std::vector< Line > validLines;
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(validLines), LineValidator());

    std::vector< Polygon > allPolygons;
    std::transform(validLines.begin(), validLines.end(), std::back_inserter(allPolygons), LineToPolygonConverter());

    std::copy_if(allPolygons.begin(), allPolygons.end(), std::back_inserter(polygons), PolygonValidator());
  }

  bool AreaComparator::operator()(const Polygon &p1, const Polygon &p2) const
  {
    return calcPolygonArea(p1) < calcPolygonArea(p2);
  }

  bool VerticesComparator::operator()(const Polygon &p1, const Polygon &p2) const
  {
    return p1.points.size() < p2.points.size();
  }

  size_t countIf(const std::vector< Polygon > &polys, const LessAreaPredicate &pred)
  {
    return std::count_if(polys.begin(), polys.end(), pred);
  }

  EchoHandler::EchoHandler(const Polygon &ref):
    reference(ref),
    count(0)
  {}

  void EchoHandler::operator()(std::vector< Polygon > &result, const Polygon &poly) const
  {
    result.push_back(poly);
    if (poly == reference)
    {
      result.push_back(poly);
      ++count;
    }
  }

  void processEcho(std::vector< Polygon > &polys, const Polygon &ref)
  {
    std::vector< Polygon > temp;
    temp.reserve(polys.size() * 2);
    EchoHandler handler(ref);
    std::for_each(polys.begin(), polys.end(),
                  std::bind(&EchoHandler::operator(), &handler, std::ref(temp), std::placeholders::_1));
    polys.swap(temp);
  }

  AreaAccumulator::AreaAccumulator(const std::string &param):
    param(param)
  {}

  double AreaAccumulator::operator()(double acc, const Polygon &poly) const
  {
    const size_t vertices = poly.points.size();
    if (param == "ODD" && vertices % 2 != 0)
    {
      return acc + calcPolygonArea(poly);
    }
    if (param == "EVEN" && vertices % 2 == 0)
    {
      return acc + calcPolygonArea(poly);
    }
    if (param == "MEAN")
    {
      return acc + calcPolygonArea(poly);
    }

    try
    {
      size_t num = std::stoul(param);
      if (num == vertices)
      {
        return acc + calcPolygonArea(poly);
      }
    }
    catch (...)
    {}

    return acc;
  }

  CountAccumulator::CountAccumulator(const std::string &param):
    param(param)
  {}

  size_t CountAccumulator::operator()(size_t cnt, const Polygon &poly) const
  {
    const size_t vertices = poly.points.size();
    if (param == "ODD" && vertices % 2 != 0)
    {
      return cnt + 1;
    }
    if (param == "EVEN" && vertices % 2 == 0)
    {
      return cnt + 1;
    }

    try
    {
      size_t num = std::stoul(param);
      if (num == vertices)
      {
        return cnt + 1;
      }
    }
    catch (...)
    {}

    return cnt;
  }

  LessAreaPredicate::LessAreaPredicate(double threshold):
    threshold(threshold)
  {}

  bool LessAreaPredicate::operator()(const Polygon &poly) const
  {
    return calcPolygonArea(poly) < threshold;
  }
}
