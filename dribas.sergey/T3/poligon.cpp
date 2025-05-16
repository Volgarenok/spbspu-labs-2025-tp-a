#include "poligon.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <string>
#include <iostream>
#include <streamGuard.hpp>
#include <delimiter.hpp>

namespace
{
  struct Gauss
  {
    const dribas::Point& firstPt;
    dribas::Point currentPt;
    double operator()(double res, const dribas::Point& nextPt) {
      double a = (currentPt.x - firstPt.x) * (nextPt.y - firstPt.y);
      double b = (nextPt.x - firstPt.x) * (currentPt.y - firstPt.y);
      res += std::abs(a - b);
      currentPt = nextPt;
      return res;
    }
  };
}

namespace dribas
{
  bool operator==(const Point& r, const Point& l)
  {
    return r.x == l.x && r.y == l.y;
  }

  std::istream& operator>>(std::istream& in, Point& point)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);

    in >> DelimiterI{ '(' } >> point.x;
    in >> DelimiterI{ ';' } >> point.y;
    in >> DelimiterI{ ')' };

    return in;
  }

  std::istream& operator>>(std::istream& in, Poligon& plg)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);

    size_t size = 0;
    if (!(in >> size) || size < 3) {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::string line;
    std::getline(std::cin, line);

    std::vector< Point > pnts(size);
    std::copy_n(std::istream_iterator< Point >(in), size, pnts.begin());
    if (in)
    {
      plg.points = std::move(pnts);
    }
    return in;
  }

  double getPoligonArea(const Poligon& poligon)
  {
    Gauss areaCalc{ poligon.points[0], poligon.points[1] };
    double area = std::accumulate(poligon.points.begin() + 2, poligon.points.end(), 0.0, std::ref(areaCalc));
    return area / 2.0;
  }
}
