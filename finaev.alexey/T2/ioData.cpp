#include "ioData.hpp"
#include <vector>
#include <complex>
#include <iomanip>
#include <algorithm>
#include "streamGuard.hpp"

namespace
{
  struct delimiterIO
  {
    char obj;
  };

  struct signedLongLongIo
  {
    long long& obj;
  };

  struct complexIO
  {
    std::complex< double >& obj;
  };

  struct stringIO
  {
    std::string& obj;
  };

  std::istream& operator>>(std::istream& in, delimiterIO&& rhs)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    char c = 0;
    in >> c;
    if (in && (c != rhs.obj))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, signedLongLongIo&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    in >> rhs.obj >> delimiterIO{'l'} >> delimiterIO{'l'};
    return in;
  }

  std::istream& operator>>(std::istream& in, complexIO&& rhs)
  {
    std::istream::sentry s(in);
    if (!s)
    {
      return in;
    }
    double real = 0.0;
    double imag = 0.0;
    in >> delimiterIO{ '#' } >> delimiterIO{ 'c' };
    in >> delimiterIO{ '(' } >> real;
    in >> imag >> delimiterIO{ ')' };
    std::complex< double > cmp { real, imag };
    rhs.obj = cmp;
    return in;
  }

  std::istream& operator>>(std::istream& in, stringIO&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    return std::getline(in >> delimiterIO{ '"' }, rhs.obj, '"');
  }
}

std::istream& finaev::operator>>(std::istream& in, DataStruct& rhs)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using sep = delimiterIO;
    using ll = signedLongLongIo;
    using cmpLsp = complexIO;
    using str = stringIO;
    in >> sep{ '(' } >> sep{ ':' };
    std::vector< std::string > usedKeys;
    std::string key = "";
    while (in >> key && key != ")")
    {
      if (std::find(usedKeys.begin(), usedKeys.end(), key) != usedKeys.end())
      {
        in.setstate(std::ios::failbit);
        break;
      }
      usedKeys.push_back(key);
      if (key == "key1")
      {
        if (!(in >> ll{ input.key1 } >> sep{':'}))
        {
          in.setstate(std::ios::failbit);
          break;
        }
      }
      else if (key == "key2")
      {
        if (!(in >> cmpLsp{ input.key2 } >> sep{':'}))
        {
          in.setstate(std::ios::failbit);
          break;
        }
      }
      else if (key == "key3")
      {
        if (!(in >> str{ input.key3 } >> sep{':'}))
        {
          in.setstate(std::ios::failbit);
          break;
        }
      }
      else
      {
        in.setstate(std::ios::failbit);
        break;
      }
    }
  }
  if (in)
  {
    rhs = input;
  }
  return in;
}

std::ostream& finaev::operator<<(std::ostream& out, const DataStruct& rhs)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  StreamGuard outGuard(out);
  out << "(:key1 ";
  out << rhs.key1;
  out << "ll:";
  out << "key2 #c(";
  out << std::fixed << std::setprecision(1) << rhs.key2.real();
  out << " ";
  out << rhs.key2.imag();
  out << ")";
  out << ":key3 \"";
  out << rhs.key3;
  out << "\":)";
  return out;
}
