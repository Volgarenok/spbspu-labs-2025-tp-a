#include "data.h"
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "iofmtguard.h"

std::istream& ohantsev::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, MultDelimiterIO&& dest)
{
  for (auto chr: dest.exp)
  {
    in >> DelimiterIO{ chr };
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, TypenameIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string s = "";
  std::getline(in, s, ':');
  if (in && std::find(dest.exp.begin(), dest.exp.end(), s) == dest.exp.end())
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, DoubleI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string numberStr;
  std::getline(in, numberStr, ':');
  bool hasExponent = false;
  for (auto chr: numberStr)
  {
    if (chr == 'e' || chr == 'E')
    {
      hasExponent = true;
      break;
    }
  }
  if (!hasExponent) {
    in.setstate(std::ios::failbit);
    return in;
  }
  try {
    dest.ref = std::stod(numberStr);
  }
  catch (...) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, UllI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref >> TypenameIO{ { "ull", "ULL" } };
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, StringI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  return in >> DelimiterIO{ ':' };
}

std::istream& ohantsev::operator>>(std::istream& in, LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> MultDelimiterIO{ "key" };
  int LabelID = 0;
  in >> LabelID;
  if (in) {
    auto iter = std::find(dest.notFilled.cbegin(), dest.notFilled.cend(), LabelID);
    if (iter == dest.notFilled.cend())
    {
      in.setstate(std::ios::failbit);
    }
    else
    {
      dest.labelID = *iter;
      dest.notFilled.erase(iter);
    }
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, KeyIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  int ID;
  in >> LabelIO{ ID, dest.notFilled };
  switch (ID)
  {
  case 1:
    in >> DoubleI{ dest.data.key1 };
    break;
  case 2:
    in >> UllI{ dest.data.key2 };
    break;
  case 3:
    in >> StringI{ dest.data.key3 };
    break;
  }
  return in;
}

std::istream& ohantsev::operator>>(std::istream& in, Data& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  iofmtguard guard(in);
  std::vector<int> labels{ 1,2,3 };
  in >> MultDelimiterIO{ "(:" };
  in >> KeyIO{ dest, labels };
  in >> KeyIO{ dest, labels };
  in >> KeyIO{ dest, labels };
  in >> MultDelimiterIO{ ")" };
  return in;
}

std::ostream& ohantsev::operator<<(std::ostream& out, DoubleO&& dest)
{
  double mant = dest.ref;
  int order = 0;
  while (std::abs(mant) < 1 && mant != 0)
  {
    mant *= 10;
    order--;
  }
  while (std::abs(mant) >= 9.95)
  {
    mant /= 10;
    order++;
  }
  mant = std::round(mant * 10) / 10.0;
  out << std::fixed << std::setprecision(1) << mant;
  out << 'e' << ((order < 0) ? '-': '+') << std::abs(order);
  return out;
}

std::ostream& ohantsev::operator<<(std::ostream& out, UllO&& dest)
{
  return (out << dest.ref << "ull");
}

std::ostream& ohantsev::operator<<(std::ostream& out, StringO&& dest)
{
  return (out << '"' << dest.ref << '"');
}

std::ostream& ohantsev::operator<<(std::ostream& out, const Data& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  iofmtguard fmtguard(out);
  out << "(:key1 " << DoubleO{ dest.key1 } << ':';
  out << "key2 " << UllO{ dest.key2 } << ':';
  out << "key3 " << StringO{ dest.key3 } << ":)";
  return out;
}
