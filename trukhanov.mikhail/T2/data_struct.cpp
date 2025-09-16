#include "data_struct.hpp"
#include <iomanip>
#include <stream_guard.hpp>
#include <data_input.hpp>

bool trukhanov::operator<(const DataStruct& lhs, const DataStruct& rhs)
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

std::istream& trukhanov::operator>>(std::istream& in, DataStruct& dest)
{
  DataStruct temp{};
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;

  StreamGuard guard(in);

  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

  while (in)
  {
    if (in.peek() == ')')
    {
      break;
    }

    std::string field;
    in >> LabelIO{ "key" } >> field;

    if (field == "1")
    {
      in >> OctalULLIO{ temp.key1 };
      hasKey1 = true;
    }
    else if (field == "2")
    {
      in >> RationalIO{ temp.key2 };
      hasKey2 = true;
    }
    else if (field == "3")
    {
      in >> StringIO{ temp.key3 };
      hasKey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    if (in.peek() == ':')
    {
      in >> DelimiterIO{ ':' };
    }
    else if (in.peek() == ')')
    {
      break;
    }
    else
    {
      in.setstate(std::ios::failbit);
      return in;
    }
  }

  in >> DelimiterIO{ ')' };

  if (hasKey1 && hasKey2 && hasKey3)
  {
    dest = temp;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream& trukhanov::operator<<(std::ostream& out, const DataStruct& src)
{
  StreamGuard guard(out);
  out << "(:key1 0" << std::oct << src.key1 << std::dec;
  out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
  out << ":key3 " << '"' << src.key3 << '"' << ":)";
  return out;
}
