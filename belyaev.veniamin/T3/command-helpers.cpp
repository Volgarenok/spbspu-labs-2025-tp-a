#include "command-helpers.hpp"

size_t belyaev::getVertices(const Polygon& src)
{
  return src.points.size();
}

bool belyaev::isPolygonEven(const Polygon& src)
{
  return getVertices(src) % 2 == 0;
}

bool belyaev::isPolygonOdd(const Polygon& src)
{
  return getVertices(src) % 2 != 0;
}

bool belyaev::isPolygonOfSize(const Polygon& src, const size_t& userSize)
{
  return getVertices(src) == userSize;
}

bool belyaev::isStringNumeric(const std::string& str)
{
  using namespace std::placeholders;
  auto isNumericBind = std::bind(isdigit, _1);
  return std::all_of(str.begin(), str.end(), isNumericBind);
}

double belyaev::accumulateTerm(double sum, int i, const std::vector<Point>& pnts, int n)
{
  const int next_i = (i + 1) % n;
  return sum + (pnts[i].y + pnts[next_i].y) * (pnts[i].x - pnts[next_i].x);
}

double belyaev::calcArea(const Polygon& src)
{
  const int amount = getVertices(src);
  std::vector<int> indices(amount);
  std::iota(indices.begin(), indices.end(), 0);

  using namespace std::placeholders;
  auto accumTermFunc = std::bind(accumulateTerm, _1, _2, std::cref(src.points), amount);
  double area = std::accumulate(indices.begin(), indices.end(), 0.0, accumTermFunc);
  return std::abs(area) / 2.0;
}

double belyaev::areaOddAccumulate(double value, const Polygon& src)
{
  if (isPolygonOdd(src))
  {
    return value + calcArea(src);
  }
  return value;
}

double belyaev::areaEvenAccumulate(double value, const Polygon& src)
{
  if (isPolygonEven(src))
  {
    return value + calcArea(src);
  }
  return value;
}

double belyaev::areaMeanAccumulate(double value, const Polygon& src, size_t size)
{
  return value + calcArea(src) / size;
}

double belyaev::areaVerticesAccumulate(double value, const Polygon& src, size_t vertices)
{
  if (getVertices(src) == vertices)
  {
    return value + calcArea(src);
  }
  return value;
}

void belyaev::areaOut(double result, std::ostream& out)
{
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << result << '\n';
}

bool belyaev::compareAreas(const Polygon& lhs, const Polygon& rhs)
{
  return calcArea(lhs) < calcArea(rhs);
}

bool belyaev::compareVertices(const Polygon& lhs, const Polygon& rhs)
{
  return getVertices(lhs) < getVertices(rhs);
}

belyaev::Polygon belyaev::minElement(const std::vector<Polygon>& data, comparatorFunction comparator)
{
  return *std::min_element(data.begin(), data.end(), comparator);
}

belyaev::Polygon belyaev::maxElement(const std::vector<Polygon>& data, comparatorFunction comparator)
{
  return *std::max_element(data.begin(), data.end(), comparator);
}

bool belyaev::rmEchoHelper(const Polygon& rmPolygon, const Polygon& lhs, const Polygon& rhs)
{
  return (rmPolygon == lhs) && (rmPolygon == rhs);
}

belyaev::Borders belyaev::getPointBorders(Borders box, const Point& pnt)
{
  return Borders{
    std::min(box.minX, pnt.x),
    std::min(box.minY, pnt.y),
    std::max(box.maxX, pnt.x),
    std::max(box.maxY, pnt.y)
  };
}

belyaev::Borders belyaev::getPolygonBorders(Borders box, const Polygon& src)
{
  using namespace std::placeholders;
  Borders polyBorders = std::accumulate(src.points.begin(), src.points.end(), Borders{}, getPointBorders);
  return Borders{
    std::min(box.minX, polyBorders.minX),
    std::min(box.minY, polyBorders.minY),
    std::max(box.maxX, polyBorders.maxX),
    std::max(box.maxY, polyBorders.maxY)
  };
}

bool belyaev::isPointInBorders(const belyaev::Point& pnt, const belyaev::Borders& box)
{
  return pnt.x >= box.minX && pnt.x <= box.maxX && pnt.y >= box.minY && pnt.y <= box.maxY;
}
