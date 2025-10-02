#include "io_actions.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <common_io_actions.hpp>
#include <streamguard.hpp>
#include "data_struct.hpp"

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
      if (keys[0])
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      if (!keys[0] && in >> DoubleIO{temp.key1})
      {
        keys[0] = true;
      }
    }
    else  if (key == "key2")
    {
      if (keys[1])
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      if (in >> UllIO{temp.key2})
      {
        keys[1] = true;
      }
    }
    else if (key == "key3")
    {
      if (keys[2])
      {
        in.setstate(std::ios::failbit);
        return in;
      }
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
  char c = '\0';
  while (in.peek() != ':' && in.peek() != std::istream::traits_type::eof())
  {
    in >> c;
    str += c;
  }
  if (!in || str.empty())
  {
    in.setstate(std::ios::failbit);
    return in;
  }
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
    if (!inputKey(in, key, temp, keys))
    {
      return in;
    }
  }
  for (auto key: keys)
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

std::ostream& tkach::operator<<(std::ostream& out, const ConstUllIO& dest)
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

std::ostream& tkach::operator<<(std::ostream& out, const ConstDoubleIO& dest)
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

std::ostream& tkach::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  tkach::StreamGuard guard(out);
  out << "(";
  out << ":key1 " << ConstDoubleIO{dest.key1};
  out << ":key2 " << ConstUllIO{dest.key2};
  out << ":key3 \"" << dest.key3 << "\":)";
  return out;
}
