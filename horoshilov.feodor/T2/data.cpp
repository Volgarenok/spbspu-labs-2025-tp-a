#include "data.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <cmath>
#include <iomanip>
#include "guard.h"

namespace horoshilov
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = 0;
    if (in >> c && std::tolower(c) != dest.exp)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> dest.ref >> DelimiterIO{ 'd' };
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleSciIO&& dest)
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

  std::istream& operator>>(std::istream& in, StringIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    return in >> DelimiterIO{ ':' };
  }

  std::istream& operator>>(std::istream& input, KeyIO&& dest)
  {
    std::istream::sentry sentry(input);
    if (!sentry)
    {
      return input;
    }
    input >> DelimiterIO{ 'k' } >> DelimiterIO{ 'e' } >> DelimiterIO{ 'y' };
    int key;
    input >> key;
    switch (key)
    {
    case 1:
      input >> DoubleIO{ dest.data.key1 } >> DelimiterIO{ ':' };
      break;
    case 2:
      input >> DoubleSciIO{ dest.data.key2 };
      break;
    case 3:
      input >> StringIO{ dest.data.key3 };
      break;
    default:
      input.setstate(std::ios::failbit);
    }
    return input;
  }

  std::istream& operator>>(std::istream& input, DataStruct& dest)
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

  std::ostream& operator<<(std::ostream& output, const DoubleIO&& dest)
  {
    return output << std::fixed << std::setprecision(1) << dest.ref << "d";
  }

  std::ostream& operator<<(std::ostream& out, const DoubleSciIO&& dest)
  {
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

  std::ostream& operator<<(std::ostream& output, const StringIO&& dest)
  {
    return output << "\"" << dest.ref << "\"";
  }

  std::ostream& operator<<(std::ostream& output, const DataStruct& dest)
  {
    std::ostream::sentry sentry(output);
    if (!sentry)
    {
      return output;
    }

    double dbl = dest.key1;
    double dblsc = dest.key2;
    std::string str = dest.key3;

    Guard guard(output);
    output << "(:key1 " << DoubleIO{ dbl };
    output << ":key2 " << DoubleSciIO{ dblsc };
    output << ":key3 " << StringIO{ str } << ":)";

    return output;
  }

  bool compareData(const DataStruct& a, const DataStruct& b)
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
}
