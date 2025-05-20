#include "data_struct.hpp"
#include <cmath>
#include <io_fmt_guard.hpp>

namespace {
  struct DelimiterIn
  {
    char exp;
  };

  struct LabelIn
  {
    std::string exp;
  };

  struct DoubleIn
  {
    double& ref;
  };

  struct DoubleOut
  {
    const double& ref;
  };

  struct UllIn
  {
    unsigned long long& ref;
  };

  struct StringIn
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIn&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, LabelIn& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string data = "";
    if (!std::getline(in, data, ' ')) {
      in.setstate(std::ios::failbit);
    }
    dest.exp = data;
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleIn&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string n = "";
    std::getline(in, n, ':');
    if (n.find("e") == n.npos && n.find("E") == n.npos) {
      in.setstate(std::ios::failbit);
    }
    try {
      dest.ref = std::stod(n);
    } catch (const std::exception& e) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const DoubleOut& dest)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    maslevtsov::IOFmtGuard fmtguard(out);
    if (dest.ref == 0.0) {
      out << "0.0e0";
    } else {
      int exponent = static_cast< int >(std::floor(std::log10(std::abs(dest.ref))));
      double mantissa = dest.ref / std::pow(10.0, exponent);
      if (mantissa >= 10.0) {
        mantissa /= 10.0;
        exponent += 1;
      } else if (mantissa < 1.0) {
        mantissa *= 10.0;
        exponent -= 1;
      }
      out << std::fixed << std::setprecision(1);
      if (exponent > 0) {
        out << mantissa << 'e' << '+' << exponent;
      } else {
        out << mantissa << 'e' << exponent;
      }
    }
    return out;
  }

  std::istream& operator>>(std::istream& in, UllIn&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    std::string n = "";
    std::getline(in, n, ':');
    if (n.find("ull") == n.npos && n.find("ULL") == n.npos) {
      in.setstate(std::ios::failbit);
    }
    try {
      dest.ref = std::stoull(n);
    } catch (const std::exception& e) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIn&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return std::getline(in >> DelimiterIn{'"'}, dest.ref, '"');
  }
}

std::istream& maslevtsov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  DataStruct result;
  in >> DelimiterIn{'('} >> DelimiterIn{':'};
  for (std::size_t i = 0; i < 3ull; ++i) {
    LabelIn label{""};
    in >> label;
    if (label.exp == "key1") {
      in >> DoubleIn{result.key1};
    } else if (label.exp == "key2") {
      in >> UllIn{result.key2};
    } else if (label.exp == "key3") {
      in >> StringIn{result.key3} >> DelimiterIn{':'};
    }
  }
  in >> DelimiterIn{')'};
  if (in) {
    dest = result;
  }
  return in;
}

std::ostream& maslevtsov::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOFmtGuard fmtguard(out);
  out << "(:key1 " << DoubleOut{dest.key1};
  out << ":key2 " << dest.key2 << "ull:";
  out << "key3 \"" << dest.key3 << "\":)";
  return out;
}
