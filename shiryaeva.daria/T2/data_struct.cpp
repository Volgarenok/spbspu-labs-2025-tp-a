#include "data_struct.hpp"
#include "input_struct.hpp"
#include "format_guard.hpp"

bool shiryaeva::operator<(const DataStruct &lhs, const DataStruct &rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.length() < rhs.key3.length();
}

std::istream &shiryaeva::operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  DataStruct input;
  using sep = DelimiterIO;
  using hex = HexUllIO;
  using chr = CharIO;
  using str = StringIO;

  in >> sep{'('};

  for (size_t i = 0; i < 3; ++i)
  {
    std::string key;
    in >> sep{':'} >> key;

    if (key == "key1")
    {
      in >> hex{input.key1};
    }
    else if (key == "key2")
    {
      in >> chr{input.key2};
    }
    else if (key == "key3")
    {
      in >> str{input.key3};
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> sep{':'} >> sep{')'};

  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream &shiryaeva::operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  shiryaeva::FormatGuard guard(out);

  out << "(:key1 0x" << std::hex << std::uppercase << src.key1;
  out << ":key2 '" << src.key2 << "'";
  out << ":key3 \"" << src.key3 << "\":)";

  return out;
}
