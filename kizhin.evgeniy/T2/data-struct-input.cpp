#include "data-struct.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
#include <interim-input-utils.hpp>
#include <stream-guard.hpp>

namespace kizhin {
  struct Label;
  struct String;
  struct Double;
  struct Nominator;
  struct Denominator;
  struct Rational;

  std::istream& operator>>(std::istream&, Label&&);
  std::istream& operator>>(std::istream&, String&&);
  std::istream& operator>>(std::istream&, Double&&);
  std::istream& operator>>(std::istream&, Nominator&&);
  std::istream& operator>>(std::istream&, Denominator&&);
  std::istream& operator>>(std::istream&, Rational&&);

  std::istream& inputKey(std::istream&, const std::string&, DataStruct&);
}

struct kizhin::Double
{
  double& val;
};

struct kizhin::Label
{
  const std::string& val;
};

struct kizhin::String
{
  std::string& val;
};

struct kizhin::Nominator
{
  long long& val;
};

struct kizhin::Denominator
{
  unsigned long long& val;
};

struct kizhin::Rational
{
  DataStruct::Rational& val;
};

std::istream& kizhin::operator>>(std::istream& in, Nominator&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> Delimiter{ 'N' } >> dest.val;
}

std::istream& kizhin::operator>>(std::istream& in, Denominator&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> Delimiter{ 'D' } >> dest.val;
}

std::istream& kizhin::operator>>(std::istream& in, Rational&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> Delimiter{ '(' } >> Delimiter{ ':' };
  in >> Nominator{ dest.val.numerator };
  in >> Delimiter{ ':' };
  in >> Denominator{ dest.val.denominator };
  in >> Delimiter{ ':' } >> Delimiter{ ')' };
  return in;
}

std::istream& kizhin::operator>>(std::istream& in, Double&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> dest.val >> OneOfDelimiters{ "dD" };
}

std::istream& kizhin::operator>>(std::istream& in, String&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> Delimiter{ '"' }, dest.val, '"');
}

std::istream& kizhin::operator>>(std::istream& in, Label&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string data;
  if (std::getline(in, data, ' ') && (data != dest.val)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kizhin::inputKey(std::istream& in, const std::string& key, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  if (key == "key1") {
    return in >> Double{ dest.key1 };
  } else if (key == "key2") {
    return in >> Rational{ dest.key2 };
  } else if (key == "key3") {
    return in >> String{ dest.key3 };
  }
  in.setstate(std::ios::failbit);
  return in;
}

std::istream& kizhin::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  StreamGuard guard(in);
  in >> std::skipws;
  in >> Delimiter{ '(' } >> Delimiter{ ':' };
  std::vector< std::string > keys{ "key1", "key2", "key3" };
  DataStruct input;
  for (std::string key; !keys.empty() && std::getline(in, key, ' ');) {
    keys.erase(std::find(keys.begin(), keys.end(), key));
    inputKey(in, key, input);
    in >> Delimiter{ ':' };
  }
  if ((in >> Delimiter{ ')' }) && keys.empty()) {
    dest = input;
  }
  return in;
}

