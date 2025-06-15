#include "data.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <Guard.h>
#include <Delimiter.h>

std::istream& horoshilov::operator>>(std::istream& in, DoubleI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref >> DelimiterIO{ 'd' };
  return in;
}

std::istream& horoshilov::operator>>(std::istream& in, DoubleSciI&& dest)
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
  if (!hasExponent)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  try
  {
    dest.ref = std::stod(numberStr);
  }
  catch (...)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& horoshilov::operator>>(std::istream& in, StringI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  return in >> DelimiterIO{ ':' };
}

std::istream& horoshilov::operator>>(std::istream& input, KeyIO&& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  input >> DelimiterIO{ 'k' } >> DelimiterIO{ 'e' } >> DelimiterIO{ 'y' };
  int key;
  input >> key;
  if (key < 1 || key > 3 || dest.usedKeys[key])
  {
    input.setstate(std::ios::failbit);
  }
  dest.usedKeys[key] = true;
  switch (key)
  {
  case 1:
    input >> DoubleI{ dest.data.key1 } >> DelimiterIO{ ':' };
    break;
  case 2:
    input >> DoubleSciI{ dest.data.key2 };
    break;
  case 3:
    input >> StringI{ dest.data.key3 };
    break;
  default:
    input.setstate(std::ios::failbit);
  }
  return input;
}

std::istream& horoshilov::operator>>(std::istream& input, DataStruct& dest)
{
  std::istream::sentry sentry(input);
  if (!sentry)
  {
    return input;
  }
  DataStruct temp;
  Guard guardian(input);
  {
    input >> DelimiterIO{ '(' } >> DelimiterIO{':'};
    input >> KeyIO{ temp };
    input >> KeyIO{ temp };
    input >> KeyIO{ temp };
    input >> DelimiterIO{ ')' };
  }
  if (input)
  {
    dest = temp;
  }
  return input;
}

std::ostream& horoshilov::operator<<(std::ostream& output, const DoubleO& dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }
  Guard scope(output);
  return output << std::fixed << std::setprecision(1) << dest.ref << "d";
}

std::ostream& horoshilov::operator<<(std::ostream& out, const DoubleSciO& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  Guard scope(out);
  double mant = dest.ref;
  int order = 0;
  if (mant != 0)
  {
    while (std::abs(mant) < 1)
    {
      mant *= 10;
      order--;
    }
    while (std::abs(mant) >= 9.995)
    {
      mant /= 10;
      order++;
    }
    mant = std::round(mant * 100) / 100.0;
  }
  out << std::fixed << std::setprecision(1) << mant;
  out << 'e' << std::showpos << order;
  return out;
}

std::ostream& horoshilov::operator<<(std::ostream& output, const StringO& dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }
  return output << "\"" << dest.ref << "\"";
}

std::ostream& horoshilov::operator<<(std::ostream& output, const DataStruct& dest)
{
  std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }
  Guard guard(output);

  output << "(:key1 " << DoubleO{ dest.key1 };
  output << ":key2 " << DoubleSciO{ dest.key2 };
  output << ":key3 " << StringO{ dest.key3 } << ":)";

  return output;
}

bool horoshilov::compareData(const DataStruct& a, const DataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }
  return a.key3.size() < b.key3.size();
}


