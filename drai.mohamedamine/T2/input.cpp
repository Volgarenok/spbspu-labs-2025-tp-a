#include "structures.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
#include "ios.hpp"

namespace firstime {
  struct Delimiter;
  struct OneOfDelimiters;
  struct Label;
  struct String;
  struct Double;
  struct Nominator;
  struct Denominator;
  struct Rational;

  std::istream& operator>>(std::istream&, Delimiter&&);
  std::istream& operator>>(std::istream&, OneOfDelimiters&&);
  std::istream& operator>>(std::istream&, Label&&);
  std::istream& operator>>(std::istream&, String&&);
  std::istream& operator>>(std::istream&, Double&&);
  std::istream& operator>>(std::istream&, Nominator&&);
  std::istream& operator>>(std::istream&, Denominator&&);
  std::istream& operator>>(std::istream&, Rational&&);

  std::istream& inputKey(std::istream&, const std::string&, DataStruct&);
}

struct firstime::Delimiter { char val; };
struct firstime::OneOfDelimiters { const std::string& val; };
struct firstime::Double { double& val; };
struct firstime::Label { const std::string& val; };
struct firstime::String { std::string& val; };
struct firstime::Nominator { long long& val; };
struct firstime::Denominator { unsigned long long& val; };
struct firstime::Rational { DataStruct::Rational& val; };

std::istream& firstime::operator>>(std::istream& in, Delimiter&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  char c;
  if ((in >> c) && (c != dest.val)) in.setstate(std::ios::failbit);
  return in;
}

std::istream& firstime::operator>>(std::istream& in, OneOfDelimiters&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  char c;
  if ((in >> c) && (dest.val.find(c) == std::string::npos)) in.setstate(std::ios::failbit);
  return in;
}

std::istream& firstime::operator>>(std::istream& in, Double&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  in >> dest.val;
  if (in.peek() == 'd' || in.peek() == 'D') in.ignore(1);
  return in;
}

std::istream& firstime::operator>>(std::istream& in, String&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  return std::getline(in >> Delimiter{ '"' }, dest.val, '"');
}

std::istream& firstime::operator>>(std::istream& in, Label&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  std::string data;
  if (std::getline(in, data, ' ') && (data != dest.val)) in.setstate(std::ios::failbit);
  return in;
}

std::istream& firstime::operator>>(std::istream& in, Nominator&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  return in >> Delimiter{ 'N' } >> dest.val;
}

std::istream& firstime::operator>>(std::istream& in, Denominator&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  return in >> Delimiter{ 'D' } >> dest.val;
}

std::istream& firstime::operator>>(std::istream& in, Rational&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  in >> Delimiter{ '(' } >> Delimiter{ ':' };
  in >> Nominator{ dest.val.numerator } >> Delimiter{ ':' };
  in >> Denominator{ dest.val.denominator } >> Delimiter{ ':' } >> Delimiter{ ')' };
  return in;
}

std::istream& firstime::inputKey(std::istream& in, const std::string& key, DataStruct& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  if (key == "key1") return in >> Double{ dest.key1 };
  else if (key == "key2") return in >> Rational{ dest.key2 };
  else if (key == "key3") return in >> String{ dest.key3 };
  in.setstate(std::ios::failbit);
  return in;
}

std::istream& firstime::operator>>(std::istream& in, DataStruct& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) return in;
  StreamGuard guard(in);
  in >> std::skipws >> Delimiter{ '(' } >> Delimiter{ ':' };
  std::vector<std::string> keys{ "key1", "key2", "key3" };
  DataStruct input;
  for (std::string key; !keys.empty() && std::getline(in, key, ' ');) {
    keys.erase(std::find(keys.begin(), keys.end(), key));
    inputKey(in, key, input);
    in >> Delimiter{ ':' };
  }
  if ((in >> Delimiter{ ')' }) && keys.empty()) dest = input;
  return in;
}