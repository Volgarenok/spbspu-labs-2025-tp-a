#include "input_struct.hpp"
#include "format_guard.hpp"
#include "data_struct.hpp"

bool filonova::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 == rhs.key1)
  {
    if (std::abs(lhs.key2) == std::abs(rhs.key2))
    {
      return lhs.key3.length() < rhs.key3.length();
    }
    return std::abs(lhs.key2) < std::abs(rhs.key2);
  }
  return lhs.key1 < rhs.key1;
}

std::istream &filonova::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  using sep = DelimiterValue;
  using hex = HexValue;
  using cmp = ComplexValue;
  using str = StringValue;

  in >> sep{'('};

  size_t keysRead = 0;
  bool key1Read = false, key2Read = false, key3Read = false;

  while (keysRead < 3)
  {
    std::string key;
    in >> sep{':'} >> key;

    if (key == "key1")
    {
      if (key1Read)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> hex{input.key1};
      key1Read = true;
    }
    else if (key == "key2")
    {
      if (key2Read)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> cmp{input.key2};
      key2Read = true;
    }
    else if (key == "key3")
    {
      if (key3Read)
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      in >> str{input.key3};
      key3Read = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (!in)
    {
      return in;
    }
    ++keysRead;
  }

  in >> sep{':'} >> sep{')'};

  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream &filonova::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  filonova::FormatGuard guard(out);

  out << "(";
  out << ":key1 0x" << std::hex << std::uppercase << src.key1;
  out << std::fixed << std::setprecision(1);
  out << ":key2 #c(" << src.key2.real() << " " << src.key2.imag() << ")";
  out << ":key3 \"" << src.key3 << "\"";
  out << ":)";

  return out;
}

