#include "iomanipulators.hpp"
#include <iomanip>
#include <string>
#include <delimiter.hpp>
#include <scopeGuard.hpp>

using check = shapkov::DelimiterIO;
std::istream& shapkov::operator>>(std::istream& in, DoubleScientificIO&& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  std::string value;
  std::getline(in, value, ':');
  size_t posOfExp = 0;
  posOfExp = value.find_first_of("eE");
  if (posOfExp == std::string::npos)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (posOfExp == value.size() - 1)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (value[posOfExp + 1] != '-' && value[posOfExp + 1] != '+')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  rhs.key = std::stod(value);
  return in;
}

std::istream& shapkov::operator>>(std::istream& in, RatioIO&& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  long long llTemp = 0;
  unsigned long long ullTemp = 0;
  in >> check{ '(' } >> check{ ':' } >> check{ 'N' };
  in >> llTemp;
  in >> check{ ':' } >> check{ 'D' };
  in >> ullTemp;
  in >> check{ ':' } >> check{ ')' };
  if (in)
  {
    rhs.key = { llTemp, ullTemp };
  }
  return in;
}

std::istream& shapkov::operator>>(std::istream& in, StringIO&& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  in >> check{ '"' };
  return std::getline(in, rhs.key, '"');
}

std::istream& shapkov::operator>>(std::istream& in, LabelIO&& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  std::string data = "";
  if (std::getline(in, data, ' ') && (data != rhs.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const DoubleScientificIO& rhs)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  ScopeGuard scopeGuard(out);
  double value = rhs.key;
  char sign = '-';
  int degree = 0;
  if (value > 1)
  {
    sign = '+';
    while (value >= 10)
    {
      degree += 1;
      value /= 10;
    }
  }
  else
  {
    while (value < 1)
    {
      degree += 1;
      value *= 10;
    }
  }
  return out << std::fixed << std::setprecision(1) << value << "e" << sign << degree;
}
