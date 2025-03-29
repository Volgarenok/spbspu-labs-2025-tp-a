#include "dataStruct.hpp"
#include <delimiter.hpp>

std::istream& shapkov::operator>>(std::istream& in, DataStruct& rhs)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  DataStruct input;
  using check = delimiterIO;
  using dbl = doubleScientificIO;
  using ratio = ratioIO;
  using str = stringIO;
  bool eachKey[3] = { false, false, false };
  std::string key = "";
  in >> check{ '(' } >> check{ ':' };
  in >> key;
  if (key == "key1")
  {
    eachKey[0] = true;
    in >> dbl{ input.key1 };
  }
  else if (key == "key2")
  {
    eachKey[1] = true;
    in >> ratio{ input.key2 };
    in >> check{ ':' };
  }
  else if (key == "key3")
  {
    eachKey[2] = true;
    in >> str{ input.key3 };
    in >> check{ ':' };
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  in >> key;
  if (key == "key1" && eachKey[0] == false)
  {
    eachKey[0] = true;
    in >> dbl{ input.key1 };
  }
  else if (key == "key2" && eachKey[1] == false)
  {
    eachKey[1] = true;
    in >> ratio{ input.key2 };
    in >> check{ ':' };
  }
  else if (key == "key3" && eachKey[2] == false)
  {
    eachKey[2] = true;
    in >> str{ input.key3 };
    in >> check{ ':' };
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  in >> key;
  if (key == "key1" && eachKey[0] == false)
  {
    eachKey[0] = true;
    in >> dbl{ input.key1 };
  }
  else if (key == "key2" && eachKey[1] == false)
  {
    eachKey[1] = true;
    in >> ratio{ input.key2 };
    in >> check{ ':' };
  }
  else if (key == "key3" && eachKey[2] == false)
  {
    eachKey[2] = true;
    in >> str{ input.key3 };
    in >> check{ ':' };
  }
  else
  {
    in.setstate(std::ios::failbit);
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
  out << doubleScientificIO{ const_cast<double&>(rhs.key1) };
  out << ":key2 (:N ";
  out << rhs.key2.first;
  out << ":D ";
  out << rhs.key2.second;
  out << ":):key3 \"";
  out << rhs.key3 << "\":)";
  return out;
}

bool shapkov::dataComparator(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  else if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  else
  {
    return lhs.key3.size() < rhs.key3.size();
  }
}
