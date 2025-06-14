#include "polygon-commands.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <limits>
#include <cmath>
#include "io-utils.hpp"

namespace aleksandrov
{
  namespace detail
  {
    struct EvenVertexesChecker
    {
      bool operator()(const Polygon& polygon) const
      {
        return polygon.points.size() % 2 == 0;
      }
    };

    struct OddVertexesChecker
    {
      bool operator()(const Polygon& polygon) const
      {
        return polygon.points.size() % 2 != 0;
      }
    };

    struct NVertexesChecker
    {
      bool operator()(const Polygon& polygon) const
      {
        return polygon.points.size() == numOfVertexes;
      }
      size_t numOfVertexes;
    };

    double multPoints(const Point& a, const Point& b)
    {
      return a.x * b.y - a.y * b.x;
    }

    double calcArea(const Polygon& polygon)
    {
      const auto& p = polygon.points;
      double sum = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, multPoints);
      sum += multPoints(p.back(), p.front());
      return std::abs(sum) / 2.0;
    }

    struct AreaComparator
    {
      bool operator()(const Polygon& a, const Polygon& b) const
      {
        return detail::calcArea(a) < detail::calcArea(b);
      }
    };

    struct VertexesComparator
    {
      bool operator()(const Polygon& a, const Polygon& b) const
      {
        return a.points.size() < b.points.size();
      }
    };
  }

  void getPolygons(std::istream& in, std::vector< Polygon >& polygons)
  {
    while (!in.eof())
    {
      std::copy(std::istream_iterator< Polygon >(in), std::istream_iterator< Polygon >(), std::back_inserter(polygons));
      if (!in)
      {
        in.clear();
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }

  double areaEven(const std::vector< Polygon >& polygons)
  {
    struct EvenAreaAccumulator
    {
      double operator()(double sum, const Polygon& polygon) const
      {
        if (isEven(polygon))
        {
          return sum + detail::calcArea(polygon);
        }
        return sum;
      }
      detail::EvenVertexesChecker isEven;
    };

    return std::accumulate(polygons.begin(), polygons.end(), 0.0, EvenAreaAccumulator{});
  }

  double areaOdd(const std::vector< Polygon >& polygons)
  {
    struct OddAreaAccumulator
    {
      double operator()(double sum, const Polygon& polygon) const
      {
        if (isOdd(polygon))
        {
          return sum + detail::calcArea(polygon);
        }
        return sum;
      }
      detail::OddVertexesChecker isOdd;
    };

    return std::accumulate(polygons.begin(), polygons.end(), 0.0, OddAreaAccumulator{});
  }

  double areaMean(const std::vector< Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("There are no polygons!");
    }
    struct AreaAccumulator
    {
      double operator()(double sum, const Polygon& polygon) const
      {
        return sum + detail::calcArea(polygon);
      }
    };
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, AreaAccumulator{}) / polygons.size();
  }

  double areaN(const std::vector< Polygon >& polygons, size_t numOfVertexes)
  {
    struct NAreaAccumulator
    {
      double operator()(double sum, const Polygon& polygon) const
      {
        if (isN(polygon))
        {
          return sum + detail::calcArea(polygon);
        }
        return sum;
      }
      detail::NVertexesChecker isN;
    };
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, NAreaAccumulator{ numOfVertexes });
  }

  void area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::string subcommand;
    in >> subcommand;
    out << std::fixed << std::setprecision(1);
    if (subcommand == "EVEN")
    {
      out << areaEven(polygons);
    }
    else if (subcommand == "ODD")
    {
      out << areaOdd(polygons);
    }
    else if (subcommand == "MEAN")
    {
      out << areaMean(polygons);
    }
    else
    {
      std::istringstream iss(subcommand);
      size_t numOfVertexes = 0;
      if (!(iss >> numOfVertexes) || subcommand[0] == '-')
      {
        throw std::logic_error("Unknown subcommand!");
      }
      if (numOfVertexes < 3)
      {
        throw std::logic_error("Incorrect number of vertexes!");
      }
      out << areaN(polygons, numOfVertexes);
    }
  }

  double maxArea(const std::vector< Polygon >& polygons)
  {
    return detail::calcArea(*std::max_element(polygons.begin(), polygons.end(), detail::AreaComparator{}));
  }

  size_t maxVertexes(const std::vector< Polygon >& polygons)
  {
    return (*std::max_element(polygons.begin(), polygons.end(), detail::VertexesComparator{})).points.size();
  }

  void max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("There are no polygons!");
    }
    std::string subcommand;
    in >> subcommand;
    out << std::fixed << std::setprecision(1);
    if (subcommand == "AREA")
    {
      out << maxArea(polygons);
    }
    else if (subcommand == "VERTEXES")
    {
      out << maxVertexes(polygons);
    }
    else
    {
      throw std::logic_error("Unknown subcommand!");
    }
  }

  double minArea(const std::vector< Polygon >& polygons)
  {
    return detail::calcArea(*std::min_element(polygons.begin(), polygons.end(), detail::AreaComparator{}));
  }

  size_t minVertexes(const std::vector< Polygon >& polygons)
  {
    return (*std::min_element(polygons.begin(), polygons.end(), detail::VertexesComparator{})).points.size();
  }

  void min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("There are no polygons!");
    }
    std::string subcommand;
    in >> subcommand;
    out << std::fixed << std::setprecision(1);
    if (subcommand == "AREA")
    {
      out << minArea(polygons);
    }
    else if (subcommand == "VERTEXES")
    {
      out << minVertexes(polygons);
    }
    else
    {
      throw std::logic_error("Unknown subcommand!");
    }
  }

  size_t countEven(const std::vector< Polygon >& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), detail::EvenVertexesChecker{});
  }

  size_t countOdd(const std::vector< Polygon >& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), detail::OddVertexesChecker{});
  }

  size_t countN(const std::vector< Polygon >& polygons, size_t numOfVertexes)
  {
    return std::count_if(polygons.begin(), polygons.end(), detail::NVertexesChecker{ numOfVertexes });
  }

  void count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    std::string subcommand;
    in >> subcommand;
    if (subcommand == "EVEN")
    {
      out << countEven(polygons);
    }
    else if (subcommand == "ODD")
    {
      out << countOdd(polygons);
    }
    else
    {
      std::istringstream iss(subcommand);
      size_t numOfVertexes = 0;
      if (!(iss >> numOfVertexes) || subcommand[0] == '-')
      {
        throw std::logic_error("Unknown subcommand!");
      }
      if (numOfVertexes < 3)
      {
        throw std::logic_error("Incorrect number of vertexes!");
      }
      out << countN(polygons, numOfVertexes);
    }
  }

  struct FrameRectFinder
  {
    FrameRect operator()(const FrameRect& rect, const Point& point) const
    {
      Point newMin{ std::min(rect.first.x, point.x), std::min(rect.first.y, point.y) };
      Point newMax{ std::max(rect.second.x, point.x), std::max(rect.second.y, point.y) };
      return { newMin, newMax };
    }
  };

  FrameRect findFrameRect(const Polygon& polygon)
  {
    const auto& pts = polygon.points;
    if (pts.empty())
    {
      return {{0, 0}, {0, 0}};
    }
    FrameRect init{ pts.front(), pts.front() };
    return std::accumulate(pts.begin(), pts.end(), init, FrameRectFinder{});
  }

  struct FrameRectsMerger
  {
    FrameRect operator()(const FrameRect& a, const FrameRect& b) const
    {
      Point newMin{ std::min(a.first.x, b.first.x), std::min(a.first.y, b.first.y) };
      Point newMax{ std::max(a.second.x, b.second.x), std::max(a.second.y, b.second.y) };
      return { newMin, newMax };
    }
  };

  FrameRect findGlobalFrameRect(const std::vector< Polygon >& polygons)
  {
    if (polygons.empty())
    {
      return {{0, 0}, {0, 0}};
    }
    std::vector< FrameRect > rects;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(rects), findFrameRect);
    return std::accumulate(rects.begin(), rects.end(), rects.front(), FrameRectsMerger{});
  }

  void inframe(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
  {
    Polygon polygon;
    if (!(in >> polygon))
    {
      throw std::logic_error("Incorrect polygon!");
    }
    while (in.peek() != '\n' && std::isspace(in.peek()))
    {
      in.ignore(1);
    }
    if (in.peek() != '\n' && in.peek() != EOF)
    {
      throw std::logic_error("Incorrect polygon!");
    }

    FrameRect globalRect = findGlobalFrameRect(polygons);
    FrameRect inputRect = findFrameRect(polygon);

    const bool isMinInside = globalRect.first.x <= inputRect.first.x && globalRect.first.y <= inputRect.first.y;
    const bool isMaxInside = globalRect.second.x >= inputRect.second.x && globalRect.second.y >= inputRect.second.y;
    out << ((isMinInside && isMaxInside) ? "<TRUE>" : "<FALSE>");
  }

  struct VectorCoordsCalculator
  {
    Vector operator()(const Point& a, const Point& b) const
    {
      return Vector{ b.x - a.x, b.y - a.y };
    }
  };

  struct EdgeVectorsCalculator
  {
    std::vector< Vector > operator()(const Polygon& polygon) const
    {
      std::vector< Vector > vectors;
      const auto& pts = polygon.points;
      VectorCoordsCalculator calculator;
      std::transform(pts.begin(), pts.end() - 1, pts.begin() + 1, std::back_inserter(vectors), calculator);
      vectors.emplace_back(calculator(pts.back(), pts.front()));
      return vectors;
    }
  };

  struct RightAngleChecker
  {
    bool operator()(const std::vector< Vector >& vectors) const
    {
      struct PerpendicularChecker
      {
        bool operator()(const Vector& a, const Vector& b) const
        {
          return !(a.first * b.first + a.second * b.second);
        }
      };
      std::vector< Vector > rotated(vectors);
      std::rotate(rotated.begin(), rotated.begin() + 1, rotated.end());
      return std::inner_product(vectors.begin(), vectors.end(), rotated.begin(), false, std::logical_or< bool >{}, PerpendicularChecker{});
    }
  };

  void rightshapes(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::vector< std::vector< Vector > > allEdges;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(allEdges), EdgeVectorsCalculator{});
    size_t count = std::count_if(allEdges.begin(), allEdges.end(), RightAngleChecker{});
    out << count;
  }
}

