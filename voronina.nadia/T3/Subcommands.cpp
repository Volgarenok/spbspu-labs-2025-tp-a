#include "Subcommands.h"

#include <IOFmtguard.h>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>

namespace
{
  struct TriangleGenerator
  {
    const std::vector< voronina::Point >& points;
    size_t i;

    TriangleGenerator(const std::vector< voronina::Point >& pts);
    voronina::Polygon operator()();
  };

  struct SideTriangleGenerator
  {
    const std::vector< voronina::Point >& points;
    size_t i;

    SideTriangleGenerator(const std::vector< voronina::Point >& pts);
    voronina::Polygon operator()();
  };

  double getAreaOfPolygon(const voronina::Polygon& shape);
  double getAreaOfTriangle(const voronina::Polygon& p);
  bool areaComporator(const voronina::Polygon& left, const voronina::Polygon& right);
  bool vertexesComporator(const voronina::Polygon& left, const voronina::Polygon& right);
  bool isEven(const voronina::Polygon& shape);
  bool isOdd(const voronina::Polygon& shape);
  bool hasGivenAmountOfVertexes(const voronina::Polygon& shape, int vertexes);
  bool isRightAngle(const voronina::Polygon& p);

  TriangleGenerator::TriangleGenerator(const std::vector< voronina::Point >& pts):
    points(pts), i(1)
  {}

  voronina::Polygon TriangleGenerator::operator()()
  {
    voronina::Polygon triangle;
    triangle.points = { points[0], points[i], points[i + 1] };
    ++i;
    return triangle;
  }

  SideTriangleGenerator::SideTriangleGenerator(const std::vector< voronina::Point >& pts):
    points(pts), i(1)
  {}

  voronina::Polygon SideTriangleGenerator::operator()()
  {
    voronina::Polygon triangle;
    triangle.points = { points[i - 1], points[i], points[i + 1] };
    ++i;
    return triangle;
  }

  double getAreaOfPolygon(const voronina::Polygon& shape)
  {
    using namespace std::placeholders;
    const size_t n = shape.points.size();
    if (n < 3)
      return 0.0;

    std::vector< voronina::Polygon > triangles(n - 2);
    std::generate(triangles.begin(), triangles.end(), TriangleGenerator(shape.points));

    std::vector< double > areas(triangles.size());
    std::transform(triangles.begin(), triangles.end(), areas.begin(), getAreaOfTriangle);

    return std::accumulate(areas.cbegin(), areas.cend(), 0.0);
  }

  double getAreaOfTriangle(const voronina::Polygon& p)
  {
    const voronina::Point& p1 = p.points[0];
    const voronina::Point& p2 = p.points[1];
    const voronina::Point& p3 = p.points[2];
    return 0.5 * std::abs((p1.x - p3.x) * (p2.y - p1.y) - (p1.x - p2.x) * (p3.y - p1.y));
  }

  bool areaComporator(const voronina::Polygon& left, const voronina::Polygon& right)
  {
    return getAreaOfPolygon(left) < getAreaOfPolygon(right);
  }

  bool vertexesComporator(const voronina::Polygon& left, const voronina::Polygon& right)
  {
    return left.points.size() < right.points.size();
  }

  bool isEven(const voronina::Polygon& shape)
  {
    return shape.points.size() % 2 == 0;
  }

  bool isOdd(const voronina::Polygon& shape)
  {
    return shape.points.size() % 2 != 0;
  }

  bool hasGivenAmountOfVertexes(const voronina::Polygon& shape, int vertexes)
  {
    return shape.points.size() == static_cast< size_t >(vertexes);
  }

  bool isRightAngle(const voronina::Polygon& p)
  {
    auto side1 = p.points[1] - p.points[0];
    auto side2 = p.points[2] - p.points[1];
    return (side1.x * side2.x + side1.y * side2.y) == 0;
  }

  bool isRegular(const voronina::Polygon& shape)
  {
    using namespace std::placeholders;
    auto shiftedPoints = std::vector< voronina::Point >(std::next(shape.points.cbegin()), shape.points.cend());
    shiftedPoints.push_back(shape.points.front());
    std::vector< double > distances(shiftedPoints.size());
    auto pointsBegin = shape.points.cbegin();
    auto pointsEnd = shape.points.cend();
    std::transform(pointsBegin, pointsEnd, shiftedPoints.cbegin(), distances.begin(), voronina::distance);
    return std::all_of(distances.begin(), distances.end(), std::bind(std::equal_to<>(), _1, distances.front()));
  }
} // namespace

bool voronina::isThereRightAngleInPolygon(const voronina::Polygon& shape)
{
  std::vector< Polygon > triangles(shape.points.size() - 2);
  std::generate(triangles.begin(), triangles.end(), SideTriangleGenerator{ shape.points });

  const auto n = shape.points.size();
  auto firstPolygon = Polygon{ { shape.points[n - 1], shape.points[0], shape.points[1] } };
  auto lastPolygon = Polygon{ { shape.points[n - 2], shape.points[n - 1], shape.points[0] } };
  triangles.push_back(std::move(firstPolygon));
  triangles.push_back(std::move(lastPolygon));

  return std::any_of(triangles.begin(), triangles.end(), isRightAngle);
}

void voronina::getEven(const std::vector< Polygon >& shapes, std::ostream& out)
{
  std::vector< Polygon > evenPolygons;
  std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(evenPolygons), isEven);
  std::vector< double > areas;
  std::transform(evenPolygons.cbegin(), evenPolygons.cend(), std::back_inserter(areas), getAreaOfPolygon);
  out << std::accumulate(areas.cbegin(), areas.cend(), 0.0);
}

void voronina::getOdd(const std::vector< Polygon >& shapes, std::ostream& out)
{
  std::vector< Polygon > oddPolygons;
  std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(oddPolygons), isOdd);
  std::vector< double > areas;
  std::transform(oddPolygons.cbegin(), oddPolygons.cend(), std::back_inserter(areas), getAreaOfPolygon);
  out << std::accumulate(areas.cbegin(), areas.cend(), 0.0);
}

void voronina::getMean(const std::vector< Polygon >& shapes, std::ostream& out)
{
  if (shapes.size() == 0)
  {
    throw std::invalid_argument("ERROR: No shapes");
  }
  std::vector< double > areas(shapes.size());
  std::transform(shapes.begin(), shapes.end(), areas.begin(), getAreaOfPolygon);
  out << std::accumulate(areas.cbegin(), areas.cend(), 0.0) / shapes.size();
}

void voronina::getVertexes(const std::vector< Polygon >& shapes, std::ostream& out, int vertexes)
{
  const int MIN_AMOUNT_OF_VERTEXES = 3;
  if (vertexes >= MIN_AMOUNT_OF_VERTEXES)
  {
    using namespace std::placeholders;
    std::vector< Polygon > filtered;
    std::copy_if(shapes.cbegin(), shapes.cend(), std::back_inserter(filtered),
                 std::bind(hasGivenAmountOfVertexes, _1, vertexes));
    std::vector< double > areas;
    std::transform(filtered.cbegin(), filtered.cend(), std::back_inserter(areas), getAreaOfPolygon);
    out << std::accumulate(areas.cbegin(), areas.cend(), 0.0);
  }
  else
  {
    throw std::invalid_argument("ERROR: Invalid amount of vertexes");
  }
}

void voronina::getAreaMax(const std::vector< Polygon >& shapes, std::ostream& out)
{
  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  out << getAreaOfPolygon(*std::max_element(shapes.cbegin(), shapes.cend(), areaComporator));
}

void voronina::getVertexesMax(const std::vector< Polygon >& shapes, std::ostream& out)
{
  out << (*std::max_element(shapes.cbegin(), shapes.cend(), vertexesComporator)).points.size();
}

void voronina::getAreaMin(const std::vector< Polygon >& shapes, std::ostream& out)
{
  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  out << getAreaOfPolygon(*std::min_element(shapes.cbegin(), shapes.cend(), areaComporator));
}

void voronina::getVertexesMin(const std::vector< Polygon >& shapes, std::ostream& out)
{
  out << (*std::min_element(shapes.cbegin(), shapes.cend(), vertexesComporator)).points.size();
}

void voronina::getEvenCount(const std::vector< Polygon >& shapes, std::ostream& out)
{
  out << std::count_if(shapes.cbegin(), shapes.cend(), isEven);
}

void voronina::getOddCount(const std::vector< Polygon >& shapes, std::ostream& out)
{
  out << std::count_if(shapes.cbegin(), shapes.cend(), isOdd);
}

void voronina::getVertexesCount(const std::vector< Polygon >& shapes, std::ostream& out, int vertexes)
{
  using namespace std::placeholders;
  const int MIN_AMOUNT_OF_VERTEXES = 3;
  if (vertexes >= MIN_AMOUNT_OF_VERTEXES)
  {
    out << std::count_if(shapes.cbegin(), shapes.cend(), std::bind(hasGivenAmountOfVertexes, _1, vertexes));
  }
  else
  {
    throw std::invalid_argument("ERROR: Invalid amount of vertexes");
  }
}

void voronina::getRegularEvenCount(const std::vector< Polygon >& shapes, std::ostream& out)
{
  using namespace std::placeholders;
  auto isRegularAndEven = std::bind(std::logical_and<>{}, std::bind(isRegular, _1), std::bind(isEven, _1));
  auto count = std::count_if(shapes.cbegin(), shapes.cend(), isRegularAndEven);
  out << count;
}

void voronina::getRegularOddCount(const std::vector< Polygon >& shapes, std::ostream& out)
{
  using namespace std::placeholders;
  auto isRegularAndOdd = std::bind(std::logical_and<>{}, std::bind(isRegular, _1), std::bind(isOdd, _1));
  auto count = std::count_if(shapes.cbegin(), shapes.cend(), isRegularAndOdd);
  out << count;
}

void voronina::getRegularVertexesCount(const std::vector< Polygon >& shapes, std::ostream& out, int vertexes)
{
  using namespace std::placeholders;
  auto isRegularPolygon = std::bind(isRegular, _1);
  auto hasGivenVertexes = std::bind(hasGivenAmountOfVertexes, _1, vertexes);
  auto isRegularAndHasGivenVertexes = std::bind(std::logical_and<>{}, isRegularPolygon, hasGivenVertexes);
  auto count = std::count_if(shapes.cbegin(), shapes.cend(), isRegularAndHasGivenVertexes);
  out << count;
}

int voronina::maxSeqFolder(int left, int right)
{
  if (left == 0 || right == 0)
  {
    return left;
  }
  return left + right;
}
