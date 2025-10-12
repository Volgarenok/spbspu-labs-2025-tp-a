#include "dataStruct.hpp"
#include <delimiter.hpp>
#include "iomanipulators.hpp"

std::istream& shapkov::operator>>(std::istream& in, DataStruct& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  DataStruct input;
  using check = DelimiterIO;
  using dbl = DoubleScientificIO;
  using ratio = RatioIO;
  using str = StringIO;
  bool hasKey[3] = { false, false, false };
  std::string key = "";
  in >> check{ '(' } >> check{ ':' };
  for (int i = 0; i < 3; i++)
  {
    in >> key;
    if (key == "key1" && !hasKey[0])
    {
      hasKey[0] = true;
      in >> dbl{ input.key1 };
    }
    else if (key == "key2" && !hasKey[1])
    {
      hasKey[1] = true;
      in >> ratio{ input.key2 };
      in >> check{ ':' };
    }
    else if (key == "key3" && !hasKey[2])
    {
      hasKey[2] = true;
      in >> str{ input.key3 };
      in >> check{ ':' };
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  in >> check{ ')' };
  if (in)
  {
    rhs = input;
  }
  return in;
}

std::ostream& shapkov::operator<<(std::ostream& out, const DataStruct& rhs)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << "(:key1 ";
  out << DoubleScientificIO{ const_cast< double& >(rhs.key1) };
  out << ":key2 (:N ";
  out << rhs.key2.first;
  out << ":D ";
  out << rhs.key2.second;
  out << ":):key3 \"";
  out << rhs.key3 << "\":)";
  return out;
}

bool shapkov::DataStruct::operator<(const DataStruct& src) const noexcept
{
  if (key1 != src.key1)
  {
    return key1 < src.key1;
  }
  else if (key2 != src.key2)
  {
    return key2 < src.key2;
  }
  else
  {
    return key3.size() < src.key3.size();
  }
}
