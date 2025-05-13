#include "data-struct.hpp"
#include <iomanip>
#include <iostream>
#include "io-guard.hpp"

namespace firstry {
  struct Label;
  struct String;
  struct ScientificDouble;
  struct Rational;

  std::ostream& operator<<(std::ostream&, const Label&);
  std::ostream& operator<<(std::ostream&, const String&);
  std::ostream& operator<<(std::ostream&, const ScientificDouble&);
  std::ostream& operator<<(std::ostream&, const Rational&);
}

struct firstry::Label
{
  const std::string& val;
};

struct firstry::String
{
  const std::string& val;
};

struct firstry::ScientificDouble
{
  const double& val;
};

struct firstry::Rational
{
  const DataStruct::Rational& val;
};

std::ostream& firstry::operator<<(std::ostream& out, const Label& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  return out << ':' << data.val << ' ';
}

std::ostream& firstry::operator<<(std::ostream& out, const Rational& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  out << "(";
  out << ":N " << data.val.numerator;
  out << ":D " << data.val.denominator;
  return out << ":)";
}

std::ostream& firstry::operator<<(std::ostream& out, const ScientificDouble& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IoGuard guard(out);
  out << std::scientific << std::setprecision(6);
  double val = data.val;
  if (val == 0.0) {
    return out << "0.0e+00";
  }
  out << val;
  
  std::string numStr = std::to_string(val);
  size_t ePos = numStr.find('E');
  if (ePos != std::string::npos) {
    numStr[ePos] = 'e';
  }
  
  return out;
}

std::ostream& firstry::operator<<(std::ostream& out, const String& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  return out << '"' << data.val << '"';
}

std::ostream& firstry::operator<<(std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IoGuard guard(out);
  out << '(';
  out << Label{ "key1" } << ScientificDouble{ data.key1 };
  out << Label{ "key2" } << Rational{ data.key2 };
  out << Label{ "key3" } << String{ data.key3 };
  return out << ":)";
}
