#include "DataStruct.hpp"
#include <iomanip>
#include "StreamGuard.hpp"
#include "IO-utilities.hpp"
namespace nikonov
{
  std::istream& operator>>(std::istream& in, DataStruct& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    DataStruct input;
    {
      using sep = detail::DelimiterIO;
      using label = detail::LabelIO;
      using dbl = detail::DoubleIO;
      using ull = detail::UnsignedLongLong;
      using str = detail::StringIO;
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
          in >> dbl{ input.key1_ } >> sep{ ':' };
          hasKey1 = true;
        }
        else if (keyName == "key2")
        {
          in >> ull{ input.key2_ } >> sep{ ':' };
          hasKey2 = true;
        }
        else if (keyName == "key3")
        {
          in >> str{ input.key3_ } >> sep{ ':' };
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
      dest.key1_ = input.key1_;
      dest.key2_ = input.key2_;
      dest.key3_ = input.key3_;
    }
    return in;
  }
  std::ostream& operator<<(std::ostream& out, const DataStruct& toOut)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard outGuard(out);
    out << "(";
    out << ":key1 " << detail::DoubleIO{ const_cast< double & >(toOut.key1_) };
    out << ":key2 " << detail::UnsignedLongLong{ const_cast< unsigned long long& >(toOut.key2_) };
    out << ":key3 " << detail::StringIO{ const_cast< std::string & >(toOut.key3_) };
    out << ":)";
    return out;
  }
}
