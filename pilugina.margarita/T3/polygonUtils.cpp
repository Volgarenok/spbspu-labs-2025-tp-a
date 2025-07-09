#include "polygonUtils.hpp"
#include <cmath>
#include <numeric>
#include <functional>
#include "polygon.hpp"

double pilugina::getPolygonArea(const Polygon &poly)
{
  using namespace std::placeholders;

  if (poly.points.size() < 3) {
    return 0.0;
  }

  double sum = 0.0;
  const Point &first = poly.points.front();
  const Point &last = poly.points.back();

  if (poly.points.size() > 1) {
    auto begin = poly.points.begin();
    auto end = poly.points.end();

    auto multXYFirst = std::bind(std::multiplies<double>{}, std::bind(&Point::x, _1), std::bind(&Point::y, _2));
    auto multXYSecond = std::bind(std::multiplies<double>{}, std::bind(&Point::x, _2), std::bind(&Point::y, _1));
    auto crossProduct = std::bind(std::minus<double>{}, multXYFirst, multXYSecond);

    sum = std::inner_product(begin, end - 1, begin + 1, 0.0, std::plus<>{}, crossProduct);
  }

  sum += (last.x * first.y - first.x * last.y);
  return std::abs(sum) / 2.0;
}

double pilugina::areaSumOperator(double init, const Polygon &poly)
{
  return init + getPolygonArea(poly);
}

bool pilugina::isEvenVertexNum(const Polygon &poly)
{
  return (poly.points.size() % 2) == 0;
}

bool pilugina::isOddVertexNum(const Polygon &poly)
{
  return (poly.points.size() % 2) == 1;
}

bool pilugina::isEqualVertexNum(std::size_t n, const Polygon &poly)
{
  return n == poly.points.size();
}
