#include "functional.hpp"

#include <iterator>
#include <limits>
#include <numeric>
#include <cmath>
#include <functional>
#include "polygon.hpp"

namespace fedorov
{
  double calcPolygonArea(const Polygon &poly)
  {
    using namespace std::placeholders;

    if (poly.points.size() < 3)
    {
      return 0.0;
    }

    double sum = 0.0;
    const Point &first = poly.points.front();
    const Point &last = poly.points.back();

    if (poly.points.size() > 1)
    {
      auto begin = poly.points.begin();
      auto end = poly.points.end();

      auto multXYFirst = std::bind(std::multiplies< double >{}, std::bind(&Point::x, _1), std::bind(&Point::y, _2));
      auto multXYSecond = std::bind(std::multiplies< double >{}, std::bind(&Point::x, _2), std::bind(&Point::y, _1));
      auto crossProduct = std::bind(std::minus< double >{}, multXYFirst, multXYSecond);

      sum = std::inner_product(begin, end - 1, begin + 1, 0.0, std::plus<>{}, crossProduct);
    }

    sum += (last.x * first.y - first.x * last.y);
    return std::abs(sum) / 2.0;
  }

  bool validatePolygon(const Polygon &poly)
  {
    return poly.points.size() >= 3;
  }

  void readPolygons(std::istream &in, std::vector< Polygon > &polygons)
  {
    using inputIt = std::istream_iterator< Polygon >;
    while (!in.eof())
    {
      if (in.fail())
      {
        in.clear(in.rdstate() ^ std::ios::failbit);
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
      std::copy(inputIt(in), inputIt(), std::back_inserter(polygons));
    }
  }
}
