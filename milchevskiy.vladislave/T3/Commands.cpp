#include "Commands.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "FormatGuard.hpp"

namespace
{
  double calculateArea(const milchevskiy::Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      return 0.0;
    }

    std::vector< double > products1(polygon.points.size());
    std::vector< double > products2(polygon.points.size());

    std::transform(
      polygon.points.begin(),
      std::prev(polygon.points.end()),
      std::next(polygon.points.begin()),
      products1.begin(),
      std::bind(
        std::multiplies< double >{},
        std::bind(&milchevskiy::Point::x, std::placeholders::_1),
        std::bind(&milchevskiy::Point::y, std::placeholders::_2)
      )
    );
    products1.back() = polygon.points.back().x * polygon.points.front().y;

    std::transform(
      polygon.points.begin(),
      std::prev(polygon.points.end()),
      std::next(polygon.points.begin()),
      products2.begin(),
      std::bind(
        std::multiplies< double >{},
        std::bind(&milchevskiy::Point::y, std::placeholders::_1),
        std::bind(&milchevskiy::Point::x, std::placeholders::_2)
      )
    );
    products2.back() = polygon.points.front().y * polygon.points.back().x;

    double sum1 = std::accumulate(products1.begin(), products1.end(), 0.0);
    double sum2 = std::accumulate(products2.begin(), products2.end(), 0.0);

    return std::abs(sum1 - sum2) / 2.0;
  }

  bool isEvenVertexCount(const milchevskiy::Polygon& polygon)
  {
    return polygon.points.size() % 2 == 0;
  }

  bool isOddVertexCount(const milchevskiy::Polygon& polygon)
  {
    return !isEvenVertexCount(polygon);
  }

  double getPolygonArea(const milchevskiy::Polygon& polygon)
  {
    return calculateArea(polygon);
  }

  size_t getPolygonVertexCount(const milchevskiy::Polygon& polygon)
  {
    return polygon.points.size();
  }

  bool compareArea(const milchevskiy::Polygon& left, const milchevskiy::Polygon& right)
  {
    return getPolygonArea(left) < getPolygonArea(right);
  }

  bool compareVertexCount(const milchevskiy::Polygon& left, const milchevskiy::Polygon& right)
  {
    return getPolygonVertexCount(left) < getPolygonVertexCount(right);
  }

  double accumulateEvenArea(double sum, const milchevskiy::Polygon& polygon)
  {
    return isEvenVertexCount(polygon) ? sum + getPolygonArea(polygon) : sum;
  }

  double accumulateOddArea(double sum, const milchevskiy::Polygon& polygon)
  {
    return isOddVertexCount(polygon) ? sum + getPolygonArea(polygon) : sum;
  }

  double accumulateAllArea(double sum, const milchevskiy::Polygon& polygon)
  {
    return sum + getPolygonArea(polygon);
  }

  bool predicateEven(const milchevskiy::Polygon& polygon)
  {
    return isEvenVertexCount(polygon);
  }

  bool predicateOdd(const milchevskiy::Polygon& polygon)
  {
    return isOddVertexCount(polygon);
  }

  bool predicateVertexCount(const milchevskiy::Polygon& polygon, const size_t vertexCount)
  {
    return getPolygonVertexCount(polygon) == vertexCount;
  }

  bool predicateLessArea(const milchevskiy::Polygon& polygon, const double compareAreaValue)
  {
    return getPolygonArea(polygon) < compareAreaValue;
  }

  struct SequenceState
  {
    size_t currentSequence;
    size_t maxSequence;
  };

  SequenceState accumulateSequence(SequenceState state, const milchevskiy::Polygon& polygon, const milchevskiy::Polygon& target)
  {
    if (polygon == target)
    {
      state.currentSequence++;
      state.maxSequence = std::max(state.maxSequence, state.currentSequence);
    }
    else
    {
      state.currentSequence = 0;
    }
    return state;
  }

  double evenArea(const std::vector< milchevskiy::Polygon >& polygons)
  {
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulateEvenArea);
  }

  double oddArea(const std::vector< milchevskiy::Polygon >& polygons)
  {
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulateOddArea);
  }

  double meanArea(const std::vector< milchevskiy::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("Not enough polygons");
    }
    double totalArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulateAllArea);
    return totalArea / polygons.size();
  }

  struct VertexAreaAccumulator
  {
    std::function< bool(const milchevskiy::Polygon&) > predicate;

    double operator()(double sum, const milchevskiy::Polygon& polygon) const
    {
      return predicate(polygon) ? sum + getPolygonArea(polygon) : sum;
    }
  };

  double vertexArea(const std::vector< milchevskiy::Polygon >& polygons, const size_t vertexCount)
  {
    auto predicate = std::bind(predicateVertexCount, std::placeholders::_1, vertexCount);
    VertexAreaAccumulator accumulator{ predicate };
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulator);
  }

  double maxAreaValue(const std::vector< milchevskiy::Polygon >& polygons)
  {
    auto maxElement = std::max_element(polygons.begin(), polygons.end(), compareArea);
    return getPolygonArea(*maxElement);
  }

  size_t maxVertexCount(const std::vector< milchevskiy::Polygon >& polygons)
  {
    auto maxElement = std::max_element(polygons.begin(), polygons.end(), compareVertexCount);
    return getPolygonVertexCount(*maxElement);
  }

  double minAreaValue(const std::vector< milchevskiy::Polygon >& polygons)
  {
    auto minElement = std::min_element(polygons.begin(), polygons.end(), compareArea);
    return getPolygonArea(*minElement);
  }

  size_t minVertexCount(const std::vector< milchevskiy::Polygon >& polygons)
  {
    auto minElement = std::min_element(polygons.begin(), polygons.end(), compareVertexCount);
    return getPolygonVertexCount(*minElement);
  }

  size_t countEven(const std::vector< milchevskiy::Polygon >& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), predicateEven);
  }

  size_t countOdd(const std::vector< milchevskiy::Polygon >& polygons)
  {
    return std::count_if(polygons.begin(), polygons.end(), predicateOdd);
  }

  size_t countVertex(const std::vector< milchevskiy::Polygon >& polygons, const size_t vertexCount)
  {
    auto predicate = std::bind(predicateVertexCount, std::placeholders::_1, vertexCount);
    return std::count_if(polygons.begin(), polygons.end(), predicate);
  }

  size_t countLessArea(const std::vector< milchevskiy::Polygon >& polygons, const milchevskiy::Polygon& comparePolygon)
  {
    double areaValue = getPolygonArea(comparePolygon);
    auto predicate = std::bind(predicateLessArea, std::placeholders::_1, areaValue);
    return std::count_if(polygons.begin(), polygons.end(), predicate);
  }

  size_t findMaxSequence(const std::vector< milchevskiy::Polygon >& polygons, const milchevskiy::Polygon& target)
  {
    if (polygons.empty())
    {
      return 0;
    }

    SequenceState initialState{ 0, 0 };
    auto accumulator = std::bind(accumulateSequence, std::placeholders::_1, std::placeholders::_2, target);
    SequenceState finalState = std::accumulate(polygons.begin(), polygons.end(), initialState, accumulator);
    return finalState.maxSequence;
  }
}

namespace milchevskiy
{
  void areaPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string parameter;
    in >> parameter;

    milchevskiy::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    std::map< std::string, std::function< double() > > subcommands;
    subcommands["EVEN"] = std::bind(evenArea, std::cref(polygons));
    subcommands["ODD"] = std::bind(oddArea, std::cref(polygons));
    subcommands["MEAN"] = std::bind(meanArea, std::cref(polygons));

    auto iterator = subcommands.find(parameter);
    if (iterator != subcommands.end())
    {
      out << iterator->second();
    }
    else
    {
      try
      {
        size_t vertexCount = std::stoul(parameter);
        if (vertexCount < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        out << vertexArea(polygons, vertexCount);
      }
      catch (const std::exception&)
      {
        throw std::invalid_argument("Invalid parameter");
      }
    }
  }

  void maxPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string parameter;
    in >> parameter;

    if (polygons.empty())
    {
      throw std::invalid_argument("Not enough polygons");
    }

    milchevskiy::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    std::map< std::string, std::function< double() > > subcommands;
    subcommands["AREA"] = std::bind(maxAreaValue, std::cref(polygons));
    subcommands["VERTEXES"] = std::bind(minVertexCount, std::cref(polygons));

    auto iterator = subcommands.find(parameter);
    if (iterator != subcommands.end())
    {
      iterator->second();
    }
    else
    {
      throw std::invalid_argument("Invalid parameter");
    }
  }

  void minPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string parameter;
    in >> parameter;

    if (polygons.empty())
    {
      throw std::invalid_argument("Not enough polygons");
    }

    milchevskiy::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);

    std::map< std::string, std::function< double() > > subcommands;
    subcommands["AREA"] = std::bind(minAreaValue, std::cref(polygons));
    subcommands["VERTEXES"] = std::bind(minVertexCount, std::cref(polygons));

    auto iterator = subcommands.find(parameter);
    if (iterator != subcommands.end())
    {
      iterator->second();
    }
    else
    {
      throw std::invalid_argument("Invalid parameter");
    }
  }

  void countPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    std::string parameter;
    in >> parameter;

    std::map< std::string, std::function< size_t() > > subcommands;
    subcommands["EVEN"] = std::bind(countEven, std::cref(polygons));
    subcommands["ODD"] = std::bind(countOdd, std::cref(polygons));

    auto iterator = subcommands.find(parameter);
    if (iterator != subcommands.end())
    {
      out << iterator->second();
    }
    else
    {
      try
      {
        size_t vertexCount = std::stoul(parameter);
        if (vertexCount < 3)
        {
          throw std::invalid_argument("Invalid vertex count");
        }
        out << countVertex(polygons, vertexCount);
      }
      catch (const std::exception&)
      {
        throw std::invalid_argument("Invalid parameter");
      }
    }
  }

  void lessAreaPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    Polygon comparePolygon;
    if (!(in >> comparePolygon))
    {
      throw std::invalid_argument("Invalid polygon");
    }

    if (comparePolygon.points.size() < 3)
    {
      throw std::invalid_argument("Invalid polygon");
    }

    size_t count = countLessArea(polygons, comparePolygon);
    out << count;
  }

  void maxSeqPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
  {
    Polygon targetPolygon;
    if (!(in >> targetPolygon))
    {
      throw std::invalid_argument("Invalid polygon");
    }

    if (targetPolygon.points.size() < 3)
    {
      throw std::invalid_argument("Invalid polygon");
    }

    size_t maxSequence = findMaxSequence(polygons, targetPolygon);
    out << maxSequence;
  }
}
