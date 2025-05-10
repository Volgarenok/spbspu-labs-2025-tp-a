#include "io_actions.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "data_struct.hpp"
#include "streamguard.hpp"

namespace
{
  std::istream& inputKey(std::istream& in, std::string& key, tkach::DataStruct& temp, std::vector< bool >& keys)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }
    using namespace tkach;
    if (key == "key1")
    {
      if (in >> DoubleIO{temp.key1})
      {
        keys[0] = true;
      }
    }
    else  if (key == "key2")
    {
      if (in >> UllIO{temp.key2})
      {
        keys[1] = true;
      }
    }
    else if (key == "key3")
    {
      if (in >> StringIO{temp.key3})
      {
        keys[2] = true;
      }
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }
}
std::istream& tkach::operator>>(std::istream& in, DelimiterIO&& dest)
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

std::istream& tkach::operator>>(std::istream& in, DoubleIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  if (!(in >> dest.ref)  || !(in >> c) || (c != 'd' && c != 'D'))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& tkach::operator>>(std::istream& in, UllIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::string str;
  if (!std::getline(in, str, ':') || str.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  in.putback(':');
  try
  {
    size_t right_symbols = 0;
    size_t val = std::stoull(str, &right_symbols, 16);
    if (str.length() != right_symbols)
    {
      in.setstate(std::ios::failbit);
    }
    else
    {
      dest.ref = val;
    }
  }
  catch (const std::exception&)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& tkach::operator>>(std::istream& in, StringIO&& dest)
{
  if (!std::getline(in >> tkach::DelimiterIO{ '"' }, dest.ref, '"'))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& tkach::operator>>(std::istream& in, LabelIO&& dest)
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

std::istream& tkach::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct temp;
  in >> DelimiterIO{'('};
  std::vector< bool > keys(3, false);
  for (size_t i = 0; i < 3; ++i)
  {
    in >> DelimiterIO{':'};
    std::string key;
    in >> key;
    inputKey(in, key, temp, keys);
  }
  for (auto key : keys)
  {
    if (key == false)
    {
      in.setstate(std::ios::failbit);
    }
  }
  if (in >> DelimiterIO{':'} >> DelimiterIO{')'})
  {
    dest = temp;
  }
  return in;
}

std::ostream& tkach::operator<<(std::ostream& out, const UllIO& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  tkach::StreamGuard guard(out);
  std::cout << "0x" << std::uppercase << std::hex << dest.ref;
  return out;
}

std::ostream& tkach::operator<<(std::ostream& out, const DoubleIO& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  tkach::StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << dest.ref << 'd';
  return out;
}

std::ostream& tkach::operator<<(std::ostream& out, const tkach::DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  tkach::StreamGuard guard(out);
  out << "(";
  out << ":key1 " << DoubleIO{const_cast< double & >(dest.key1)};
  out << ":key2 " << UllIO{const_cast< size_t & >(dest.key2)};
  out << ":key3 \"" << dest.key3 << "\":)";
  return out;
}
