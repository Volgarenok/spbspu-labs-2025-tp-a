#include "structures.hpp"
#include <iomanip>
#include <iostream>
#include "ios.hpp"

namespace firstime {
  struct Label;
  struct String;
  struct Double;
  struct Rational;

  std::ostream& operator<<(std::ostream&, const Label&);
  std::ostream& operator<<(std::ostream&, const String&);
  std::ostream& operator<<(std::ostream&, const Double&);
  std::ostream& operator<<(std::ostream&, const Rational&);
}

struct firstime::Label { const std::string& val; };
struct firstime::String { const std::string& val; };
struct firstime::Double { const double& val; };
struct firstime::Rational { const DataStruct::Rational& val; };

std::ostream& firstime::operator<<(std::ostream& out, const Label& data) {
  std::ostream::sentry sentry(out);
  if (!sentry) return out;
  return out << ':' << data.val << ' ';
}

std::ostream& firstime::operator<<(std::ostream& out, const Double& data) {
  std::ostream::sentry sentry(out);
  if (!sentry) return out;
  StreamGuard guard(out);
  out << std::scientific << std::setprecision(1) << data.val;
  return out;
}

std::ostream& firstime::operator<<(std::ostream& out, const Rational& data) {
  std::ostream::sentry sentry(out);
  if (!sentry) return out;
  out << "(:N " << data.val.numerator << ":D " << data.val.denominator << ":)";
  return out;
}

std::ostream& firstime::operator<<(std::ostream& out, const String& data) {
  std::ostream::sentry sentry(out);
  if (!sentry) return out;
  return out << '"' << data.val << '"';
}

std::ostream& firstime::operator<<(std::ostream& out, const DataStruct& data) {
  std::ostream::sentry sentry(out);
  if (!sentry) return out;
  StreamGuard guard(out);
  out << "(:";
  out << Label{ "key1" } << Double{ data.key1 } << ':';
  out << Label{ "key2" } << Rational{ data.key2 } << ':';
  out << Label{ "key3" } << String{ data.key3 } << ":)";
  return out;
}
