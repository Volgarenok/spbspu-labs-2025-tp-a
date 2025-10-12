#include "data.hpp"

#include <iomanip>
#include <sstream>

#include <streamGuard.hpp>
#include <delimiter.hpp>

namespace dribas
{
  bool compare(const DataStruct& lhs, const DataStruct& rhs) noexcept
  {
    if (lhs.key1 != rhs.key1) {
      return lhs.key1 < rhs.key1;
    }
    if (lhs.key2 != rhs.key2) {
      return lhs.key2 < rhs.key2;
    }
    return lhs.key3.length() < rhs.key3.length();
  }

  std::ostream& operator<<(std::ostream& out, const DoubleO&& dbl)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    StreamGuard guard(out);

    out << "key1 ";
    out << std::fixed << std::setprecision(1) << dbl.ref;
    out << "d";

    return out;
  }

  std::ostream& operator<<(std::ostream& out, const UllO&& ull)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    StreamGuard guard(out);

    out << "key2 0x";
    out << std::hex << std::uppercase << ull.ref;
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const StringO&& str)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }

    StreamGuard guard(out);
    out << "key3 ";
    out << '"';
    out << str.ref;
    out << '"';

    return out;
  }

  std::istream& operator>>(std::istream& in, DoubleI&& dbl)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);

    return in >> dbl.ref >> DelimiterI{ 'd' };
  }

  std::istream& operator>>(std::istream& in, UllI&& ull)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);

    return in >> DelimiterI{ '0' } >> DelimiterI{ 'x' } >> std::hex >> ull.ref;
  }

  std::istream& operator>>(std::istream& in, StringI&& str)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);

    return std::getline(in >> DelimiterI{ '"' }, str.ref, '"');
  }

  std::istream& operator>>(std::istream& in, KeyI&& key)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);
    in >> key.key;
    if (key.key != key.expected && key.wasVerified) {
      in.setstate(std::ios::failbit);
    }
    key.wasVerified = true;
    return in;
  }

  std::istream& operator>>(std::istream& in, DataStruct& data)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);

    DataStruct temp;
    in >> DelimiterI{ '(' } >> DelimiterI{ ':' };

    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

    for (int i = 0; i < 3; ++i) {
      std::string key;
      in >> KeyI{ key, "key1", hasKey1 } || in >> KeyI{ key, "key2", hasKey2 } || in >> KeyI{ key, "key3", hasKey3 };
      if (!in) {
        in.setstate(std::ios::failbit);
        return in;
      }
      if (key == "key1") {
        in >> DoubleI{ temp.key1 };
      } else if (key == "key2") {
        in >> UllI{ temp.key2 };
      } else if (key == "key3") {
        in >> StringI{ temp.key3 };
      }
      in >> DelimiterI{ ':' };
    }
    in >> DelimiterI{ ')' };
    if (in) {
      data = temp;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& data)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    StreamGuard guard(out);

    out <<  "(:";
    out << DoubleO{ data.key1 };
    out << ':';
    out << UllO{ data.key2 };
    out << ':';
    out << StringO{ data.key3 };
    out << ":)";

    return out;
  }
}
