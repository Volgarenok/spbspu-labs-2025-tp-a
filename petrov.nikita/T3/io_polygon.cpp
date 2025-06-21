#include "io_polygon.hpp"

petrov::StreamGuard::StreamGuard(std::basic_ios< char > & s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

petrov::StreamGuard::~StreamGuard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

std::istream & petrov::operator>>(std::istream & in, DelimeterIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char data = '0';
  if (!(in >> data && data == dest.exp))
  {
    in.setstate(std::ios_base::failbit);
  }
  return in;
}

std::istream & petrov::operator>>(std::istream & in, Polygon & polygon)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard in_strm_grd(in);
  Polygon input;
  {
    std::noskipws(in);
    using sep = DelimeterIO;
    size_t number_of_points = 0;
    if (in >> number_of_points && number_of_points >= 3)
    {
      int x = 0;
      int y = 0;
      for (size_t i = 0; i < number_of_points; i++)
      {
        in >> sep{ ' ' };
        in >> sep{ '(' };
        if (in >> x && in >> sep{ ';' } && in >> y)
        {
          input.points.push_back({ x, y });
        }
        in >> sep{ ')' };
      }
      in >> sep{ '\n' };
    }
    else
    {
      in.setstate(std::ios_base::failbit);
    }
  }
  if (in)
  {
    polygon = input;
  }
  return in;
}

std::ostream & petrov::operator<<(std::ostream & out, Polygon & polygon)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << polygon.points.size();
  for (size_t i = 0; i < polygon.points.size(); i++)
  {
    out << " (";
    out << polygon.points[i].x;
    out << ';';
    out << polygon.points[i].y;
    out << ')';
  }
  return out;
}
