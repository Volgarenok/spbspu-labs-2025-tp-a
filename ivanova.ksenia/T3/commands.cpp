#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <utility>

#include "commands.hpp"
#include "polygon.hpp"
#include "streamGuard.hpp"

namespace ivanova
{
  static bool isDigit(char c)
  {
    return std::isdigit(c);
  }

  static double areaEvenAcc(double acc, const Polygon& p)
  {
    return acc + (p.size() % 2 == 0 ? p.area() : 0.0);
  }

  static double areaOddAcc(double acc, const Polygon& p)
  {
    return acc + (p.size() % 2 != 0 ? p.area() : 0.0);
  }

  static double areaAcc(double acc, const Polygon& p)
  {
    return acc + p.area();
  }

  static double areaWithVerexes(const Polygon& p, std::size_t vertexes)
  {
    return (p.size() == vertexes) ? p.area() : 0.0;
  }

  static bool areaLess(const Polygon& a, const Polygon& b)
  {
    return a.area() < b.area();
  }

  static bool vertexesLess(const Polygon& a, const Polygon& b)
  {
    return a.size() < b.size();
  }

  static std::size_t sizeMod(const Polygon& p, std::size_t mod)
  {
    return std::modulus<std::size_t>()(p.size(), mod);
  }

  static std::vector< Polygon > echoAccumulate(std::vector< Polygon > acc, const Polygon& p, const Polygon& target)
  {
    acc.push_back(p);
    if (p == target)
    {
      acc.push_back(p);
    }
    return acc;
  }

  static std::vector< Point > edgeVectors(const Polygon& p)
  {
    std::vector<Point> rotated = p.points;
    std::rotate(rotated.begin(), rotated.begin() + 1, rotated.end());

    std::vector<Point> edges;
    edges.resize(p.size());
    std::transform(
      p.points.begin(), p.points.end(),
      rotated.begin(),
      edges.begin(),
      std::minus<Point>()
    );
    return edges;
  }

  static bool isOverlayable(const Polygon& a, const Polygon& b)
  {
    if (a.size() != b.size())
    {
      return false;
    }
    if (a.size() <= 1)
    {
      return true;
    }

    auto edgesA = edgeVectors(a);
    auto edgesB = edgeVectors(b);
    std::vector<Point> doubled;
    doubled.reserve(edgesA.size() * 2);
    doubled.insert(doubled.end(), edgesA.begin(), edgesA.end());
    doubled.insert(doubled.end(), edgesA.begin(), edgesA.end());

    if (std::search(doubled.begin(), doubled.end(), edgesB.begin(), edgesB.end()) != doubled.end())
    {
      return true;
    }
    std::transform(doubled.begin(), doubled.end(), doubled.begin(), std::negate<Point>());
    std::reverse(doubled.begin(), doubled.end());
    return std::search(doubled.begin(), doubled.end(), edgesB.begin(), edgesB.end()) != doubled.end();
  }

  void areaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
  {
    std::string param;
    in >> param;

    double answer = 0.0;
    if (param == "EVEN" || param == "ODD" || param == "MEAN")
    {
      answer = area(data, param);
    }
    else
    {
      if (!std::all_of(param.begin(), param.end(), isDigit))
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      std::size_t vertices = std::stoull(param);
      if (vertices < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      answer = area(data, vertices);
    }
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << answer << "\n";
  }

  void maxCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& src)
  {
    std::string param;
    in >> param;
    if (param == "AREA")
    {
      StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << maxArea(src) << "\n";
    }
    else if (param == "VERTEXES")
    {
      out << maxVertexes(src) << "\n";
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void minCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& src)
  {
    std::string param;
    in >> param;
    if (param == "AREA")
    {
      StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << minArea(src) << "\n";
    }
    else if (param == "VERTEXES")
    {
      out << minVertexes(src) << "\n";
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void countCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& src)
  {
    std::string param;
    in >> param;
  
    std::size_t answer = 0;
    if (param == "EVEN" || param == "ODD")
    {
      answer = count(src, param);
    }
    else
    {
      if (!std::all_of(param.begin(), param.end(), isDigit))
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      std::size_t vertices = std::stoull(param);
      if (vertices < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      answer = count(src, vertices);
    }
    out << answer << "\n";
  }

  void echoCommand(std::istream& in, std::ostream& out, std::vector< Polygon >& src)
  {
    Polygon target;
    if (!(in >> target) || target.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    out << echo(src, target) << "\n";
  }

  void sameCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& src)
  {
    Polygon target;
    if (!(in >> target) || target.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    out << same(src, target) << "\n";
  }

  void printCommand(std::ostream& out, const std::vector< Polygon>& src)
  {
    std::copy(src.begin(), src.end(), std::ostream_iterator< Polygon >(out, "\n"));
  }
}
