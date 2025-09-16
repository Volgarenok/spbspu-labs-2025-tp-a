#include "target.hpp"

namespace
{
  bool is_equal_type(const std::string& rhs)
  {
    return (rhs == "UAV" || rhs == "MISSILE" || rhs == "DRONE" || rhs == "FIGHTER");
  }
  bool is_equal_unique_code(const std::string& rhs)
  {
    return rhs.size() == 9;
  }
}

std::istream& lanovenko::operator>>(std::istream& in, Target& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  if (!(in >> rhs.type) || !is_equal_type(rhs.type))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> rhs.distance))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> rhs.height))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> rhs.speed))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> rhs.unique_code) || !is_equal_unique_code(rhs.unique_code))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

std::ostream& lanovenko::operator<<(std::ostream& out, const Target& rhs)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << "TYPE: " << rhs.type << "\n";
  out << "DISTANCE: " << rhs.distance << "\n";
  out << "HEIGHT: " << rhs.height << "\n";
  out << "SPEED: " << rhs.speed << "\n";
  out << "UNIQUE CODE: " << rhs.unique_code << "\n";
  return out;
}
