#include "pantsir.hpp"

std::istream& lanovenko::operator>>(std::istream& in, Pantsir& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  if (!(in >> rhs.missiles) || rhs.missiles > 12)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> rhs.ammunition) || rhs.ammunition > 1400)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> rhs.barell_wear) || rhs.barell_wear >= 100)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> rhs.opening_hours) || rhs.opening_hours > 2000)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!(in >> rhs.targets_destroyed))
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

std::ostream& lanovenko::operator<<(std::ostream& out, const Pantsir& rhs)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << "MISILLES: " << rhs.missiles << "\n";
  out << "AMMUNITION: " << rhs.ammunition << "\n";
  out << "BARELL WEAR: " << rhs.barell_wear << "%\n";
  out << "OPENING HOURS: " << rhs.opening_hours << "\n";
  out << "TARGETS DESTROYED: " << rhs.targets_destroyed << "\n";
  return out;
}
