#include "DataStruct.hpp"
#include <iomanip>
#include <DelimetrIO.hpp>
#include "StreamGuard.hpp"
#include "IOUtilities.hpp"
std::istream& nikonov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using sep = DelimiterIO;
    using label = LabelI;
    using dbl = DoubleI;
    using ull = UnsignedLongLongI;
    using str = StringI;
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;
    in >> sep{ '(' } >> sep{ ':' };
    for (size_t i = 0; i < 3; ++i)
    {
      std::string keyName;
      in >> label{ keyName };
      if (keyName == "key1")
      {
        if (hasKey1)
        {
          in.setstate(std::ios::failbit);
          break;
        }
        in >> dbl{ input.key1 } >> sep{ ':' };
        hasKey1 = true;
      }
      else if (keyName == "key2")
      {
        if (hasKey2)
        {
          in.setstate(std::ios::failbit);
          break;
        }
        in >> ull{ input.key2 } >> sep{ ':' };
        hasKey2 = true;
      }
      else if (keyName == "key3")
      {
        if (hasKey3)
        {
          in.setstate(std::ios::failbit);
          break;
        }
        in >> str{ input.key3 } >> sep{ ':' };
        hasKey3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
      }
    }
    in >> sep{ ')' };
    if (!(hasKey1 && hasKey2 && hasKey3))
    {
      in.setstate(std::ios::failbit);
    }
  }
  if (in)
  {
    dest = input;
  }
  return in;
}
std::ostream& nikonov::operator<<(std::ostream& out, const DataStruct& toOut)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  StreamGuard outGuard(out);
  out << "(";
  out << ":key1 " << DoubleO{ toOut.key1 };
  out << ":key2 " << UnsignedLongLongO{ toOut.key2 };
  out << ":key3 " << StringO{ toOut.key3 };
  out << ":)";
  return out;
}
