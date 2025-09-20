#include "data_struct.hpp"
#include <string>
#include <exception>
#include <iomanip>
#include <scope_guard.hpp>
#include <delimiter.hpp>

std::istream& karnauhova::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> dest.ref >> DelimiterIO{'d'};
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, UllIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> dest.ref >> DelimiterIO{'u'} >> DelimiterIO{'l'} >> DelimiterIO{'l'};
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, KeyIO& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string input = "";
  in >> input;
  if (input.substr(0, 3) != "key")
  {
    in.setstate(std::ios::failbit);
  }
  dest.num = input.back();
  return in;
}

std::istream& karnauhova::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using key = KeyIO;
    using symb = DelimiterIO;
    using dbl = DoubleIO;
    using ull = UllIO;
    using str = StringIO;
    bool key1 = false;
    bool key2 = false;
    bool key3 = false;
    in >> symb{'('} >> symb{':'};
    for (size_t i = 0; i < 3; i++)
    {
      key key_num{ 0 };
      in >> key_num;
      if (key_num.num == '1' && !key1)
      {
        in >> dbl{input.key1} >> symb{':'};
        key1 = true;
      }
      else if (key_num.num == '2' && !key2)
      {
        in >> ull{input.key2} >> symb{':'};
        key2 = true;
      }
      else if (key_num.num == '3' && !key3)
      {
        in >> str{input.key3} >> symb{':'};
        key3 = true;
      }
      else
      {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    in >> symb{')'};
  }
  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream& karnauhova::operator<<(std::ostream& out, const DoubleIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  ScopeGuard fmtguard(out);
  out << std::fixed << std::setprecision(1) << dest.ref << "d";
  return out;
}

std::ostream& karnauhova::operator<<(std::ostream& out, const UllIO&& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << dest.ref << "ull";
  return out;
}

std::ostream& karnauhova::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  double dbl = src.key1;
  unsigned long long ull = src.key2;
  ScopeGuard fmtguard(out);
  out << "(:key1 " << DoubleIO{dbl};
  out << ":key2 " << UllIO{ull};
  out << ":key3 \"" << src.key3;
  out << "\":)";
  return out;
}

bool karnauhova::compare(const DataStruct& l, const DataStruct& r)
{
  if (l.key1 != r.key1)
  {
    return l.key1 < r.key1;
  }
  if (l.key2 != r.key2)
  {
    return l.key2 < r.key2;
  }
  return l.key3.size() < r.key3.size();
}
