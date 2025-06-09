#include "Shapes.h"

#include <iomanip>

#include "Delimiter.h"
#include "Iofmtguard.h"

const int MIN_AMOUNT_OF_VERTEXES = 3;

std::istream &voronina::operator>>(std::istream &in, Point &point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Point input;
  in >> DelimiterIO{'('};
  in >> input.x;
  in >> DelimiterIO{';'};
  in >> input.y;
  in >> DelimiterIO{')'};

  if (!in.fail())
  {
    point = input;
  }
  return in;
}

std::istream &voronina::operator>>(std::istream &in, Polygon &polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  iofmtguard fmtguard(in);
  Polygon input;
  int vertexes = 0;
  in >> std::noskipws;
  in >> vertexes;
  if (vertexes < MIN_AMOUNT_OF_VERTEXES)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  Point point;
  for (int i = 0; i < vertexes; ++i)
  {
    if (in.peek() == '\n')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> DelimiterIO{' '};
    if (in.peek() == '\n')
    {
      in.setstate(std::ios::failbit);
      return in;
    }
    in >> point;
    input.points.push_back(point);

    if (in.fail() && !in.eof())
    {
      return in;
    }
  }
  if (in.peek() != EOF)
  {
    in >> DelimiterIO{'\n'};
  }
  if (!in.fail())
  {
    polygon = input;
  }
  return in;
}

bool voronina::Point::operator==(const Point &point) const
{
  return (x == point.x && y == point.y);
}

bool voronina::Polygon::operator==(const Polygon &polygon) const
{
  return (points == polygon.points);
}
