#include "data_struct.hpp"

#include <iomanip>

#include <input_delimiter.hpp>
#include <format_guard.hpp>
#include "input_parsers.hpp"

std::ostream &fedorov::operator<<(std::ostream &out, const DataStruct &value)
{
  std::ostream::sentry guard(out);
  if (!guard)
  {
    return out;
  }

  FormatGuard fmt_guard(out);
  out << std::setprecision(1) << std::fixed;
  out << "(:key1 " << value.key1 << "d";
  out << ":key2 #c(" << value.key2.real() << " " << value.key2.imag() << "):";
  out << "key3 \"" << value.key3 << "\":)";
  return out;
}

bool fedorov::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  else if (std::abs(lhs.key2) != std::abs(rhs.key2))
  {
    return std::abs(lhs.key2) < std::abs(rhs.key2);
  }
  else
  {
    return (lhs.key3.length() < rhs.key3.length());
  }
}

std::istream &fedorov::operator>>(std::istream &in, DataStruct &value)
{
  std::istream::sentry guard(in);
  if (!guard)
  {
    return in;
  }

  using del = DelimiterInput;
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;
  std::string numKey = " ";

  in >> del{'('};
  while (in && !(hasKey1 && hasKey2 && hasKey3))
  {
    in >> del{':'} >> numKey;
    if (numKey == "key1")
    {
      if (hasKey1)
      {
        in.setstate(std::ios::failbit);
        break;
      }
      in >> DoubleKey{value.key1};
      hasKey1 = true;
    }
    else if (numKey == "key2")
    {
      if (hasKey2)
      {
        in.setstate(std::ios::failbit);
        break;
      }
      in >> ComplexKey{value.key2};
      hasKey2 = true;
    }
    else if (numKey == "key3")
    {
      if (hasKey3)
      {
        in.setstate(std::ios::failbit);
        break;
      }
      in >> StringKey{value.key3};
      hasKey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      break;
    }
  }

  if (!hasKey1 || !hasKey2 || !hasKey3)
  {
    in.setstate(std::ios::failbit);
  }

  if (in)
  {
    in >> del{':'} >> del{')'};
  }
  return in;
}
