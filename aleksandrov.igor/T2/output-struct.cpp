#include "output-struct.hpp"
#include "stream-guard.hpp"

namespace aleksandrov
{
  std::ostream& operator<<(std::ostream& out, const DoubleO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard guard(out);
    return out << std::fixed << std::setprecision(1) << dest.ref << 'd';
  }

  std::ostream& operator<<(std::ostream& out, const RationalO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard guard(out);
    out << "(:N " << dest.ref.first;
    out << ":D " << dest.ref.second;
    return out << ":)";
  }

  std::ostream& operator<<(std::ostream& out, const StringO& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard guard(out);
    return out << '"' << dest.ref << '"';
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    StreamGuard guard(out);
    out << "(:";
    out << "key1 " << DoubleO{ dest.key1 } << ':';
    out << "key2 " << RationalO{ dest.key2 } << ':';
    out << "key3 " << StringO{ dest.key3 };
    return out << ":)";
  }
}

