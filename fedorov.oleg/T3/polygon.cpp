#include "polygon.hpp"

#include <algorithm>
#include <iterator>

#include <input_delimiter.hpp>

namespace fedorov
{
  bool operator==(const Point &p1, const Point &p2)
  {
    return (p1.x == p2.x) && (p1.y == p2.y);
  }

  std::istream &operator>>(std::istream &in, Point &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    Point temp{0, 0};
    in >> DelimiterInput{'('} >> temp.x >> DelimiterInput{';'} >> temp.y >> DelimiterInput{')'};
    if (in)
    {
      dest = temp;
    }
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const Point &src)
  {
    return out << '(' << src.x << ';' << src.y << ')';
  }

  bool operator==(const Polygon &p1, const Polygon &p2)
  {
    return p1.points == p2.points;
  }

  bool operator!=(const Polygon &p1, const Polygon &p2)
  {
    return !(p1 == p2);
  }
  std::istream &operator>>(std::istream &in, Polygon &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    std::size_t vertex_num = 0;
    in >> vertex_num;
    if (!in || vertex_num < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    dest.points.clear();
    std::copy_n(std::istream_iterator< Point >{in}, vertex_num - 1, std::back_inserter(dest.points));
    if (in.peek() != '\n')
    {
      std::copy_n(std::istream_iterator< Point >{in}, 1, std::back_inserter(dest.points));
    }
    if (!in || dest.points.size() != vertex_num || in.peek() != '\n')
    {
      dest.points.clear();
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const Polygon &src)
  {
    out << src.points.size() << ' ';
    std::copy(src.points.begin(), src.points.end(), std::ostream_iterator< Point >(out, " "));
    return out;
  }
}
