#include <iomanip>

#include "dataStruct.hpp"
#include "delimiter.hpp"
#include "keys.hpp"
#include "scopeGuard.hpp"

std::string castDoubleToScientific(double val)
{
  int exp = 0;
  while (std::abs(val) < 1)
  {
    val *= 10;
    exp--;
  }
  while (std::abs(val) >= 10)
  {
    val /= 10;
    exp++;
  }
  int num = static_cast< int >(std::round(val * 10));
  std::string result = std::to_string(num);
  result.insert(1, 1, '.');
  result += 'e';
  if (exp >= 0)
  {
    result += '+';
  }
  return result + std::to_string(exp);
}

std::istream& shabalin::operator>>(std::istream& in, DataStruct& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct temp{ { 0, 0 }, 0.0, "" };
  const std::size_t KEYS_COUNT = 3;
  in >> DelimiterSym{ '(' };
  for (std::size_t i = 0; i < KEYS_COUNT && in; ++i)
  {
    char key = 0;
    in >> DelimiterStr{ ":key" } >> key;
    switch (key)
    {
    case '1':
      in >> ComplexKey{ temp.key1 };
      break;
    case '2':
      in >> DoubleKey{ temp.key2 };
      break;
    case '3':
      in >> StringKey{ temp.key3 };
      break;
    default:
      in.setstate(std::ios::failbit);
      break;
    }
  }
  in >> DelimiterStr{ ":)" };
  if (in)
  {
    value = temp;
  }
  return in;
}

std::ostream& shabalin::operator<<(std::ostream& out, const DataStruct& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  ScopeGuard guard(out);
  out << std::setprecision(1) << std::fixed;
  out << "(:";
  out << "key1 #c(" << value.key1.real() << ' ' << value.key1.imag() << "):";
  out << ":key2 " << castDoubleToScientific(value.key2) << ":";
  out << "key3 \"" << value.key3 << '\"';
  out << ":)";
  return out;
}

bool shabalin::DataStruct::operator<(const DataStruct& other) const
{
  if (key1 == other.key1)
  {
    if (key2 == other.key2)
    {
      return key3.size() < other.key3.size();
    }
    return key2 < other.key2;
  }
  return std::abs(key1) < std::abs(other.key1);
}

