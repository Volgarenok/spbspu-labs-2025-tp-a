#include "geometry.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <functional>
#include <delimiter.hpp>

namespace
{
  using namespace demehin::io;

  struct AreaCalc
  {
    const demehin::Point& pt0;
    demehin::Point pt1;

    double operator()(double area, const demehin::Point& pt2)
    {
      area += 0.5 * std::abs((pt1.x - pt0.x) * (pt2.y - pt0.y) - (pt2.x - pt0.x) * (pt1.y - pt0.y));
      pt1 = pt2;
      return area;
    }
  };
}

bool demehin::operator==(const Point& pt1, const Point& pt2)
{
  return pt1.x == pt2.x && pt1.y == pt2.y;
}

std::istream& demehin::operator>>(std::istream& in, Point& pt)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  {
    using sep = DelimiterIO;

    return in >> sep{ '(' } >> pt.x >> sep{ ';' } >> pt.y >> sep{ ')' };
  }
}

std::istream& demehin::operator>>(std::istream& in, Polygon& plg)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  size_t vrtx_cnt;
  if (!(in >> vrtx_cnt) || vrtx_cnt < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector< Point > pts(vrtx_cnt);
  std::copy_n(std::istream_iterator< Point >(in), vrtx_cnt, pts.begin());

  if (!in)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  plg.points = pts;
  return in;
}

double demehin::getPlgArea(const Polygon& plg)
{
  AreaCalc area{ plg.points[0], plg.points[1] };
  return std::accumulate(plg.points.begin() + 2, plg.points.end(), 0.0, std::ref(area));
}
