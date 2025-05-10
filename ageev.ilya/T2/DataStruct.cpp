#include "DataStruct.hpp"
#include <stdexcept>
#include <cmath>
#include <string>
#include <iomanip>


namespace ageev
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
  {
    // все перегрузки операторов ввода/вывода должны начинаться с проверки экземпляра класса sentry
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

  std::istream& operator>>(std::istream& in, DoubleIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return in >> dest.ref >> DelimiterIO{ 'd' };
  }

  std::istream& operator>>(std::istream& in, DoubleSciIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    char c = '0';
    std::string str;
    try
    {
      while (in >> c && (c != 'e' or c != 'E'))
      {
        str += c;
      }
      double number = std::stod(str);
      str.clear();
      while (in >> c && c != ':')
      {
        str += c;
      }
      int degree = std::stoi(str);
      dest.ref = number * std::pow(10, degree);
    }
    catch (const std::exception& e)
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
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO&& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    std::string data = "";
    if ((in >> StringIO{ data }) && (data != dest.exp))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
  // { "key1": 1.0d, "key2": "Let madness release you" }

  std::istream& operator>>(std::istream& in, Data& dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    Data input;
    {
      using sep = DelimiterIO;
      using label = LabelIO;
      using dbl = DoubleIO;
      using dblsci = DoubleSciIO;
      using str = StringIO;
      in >> sep{ '(' } >> sep ( ':' );
      in >> label{ "key1" } >> dbl{ input.key1 };
      in >> sep{ ':' };
      in >> label{ "key2" } >> dblsci{ input.key2 };
      in >> sep{ ':' };
      in >> label{ "key3" } >> str{ input.key3 };
      in >> sep{ ')' };
    }
    if (in)
    {
      dest = input;
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Data& src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
      return out;
    }
    iofmtguard fmtguard(out);
    out << "{ ";
    out << "\"key1\": " << std::fixed << std::setprecision(1) << src.key1 << "d, ";
    out << "\"key2\": " << src.key2;
    out << " }";
    return out;
  }

  iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
  {}

  iofmtguard::~iofmtguard()
  {
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
  }
}
