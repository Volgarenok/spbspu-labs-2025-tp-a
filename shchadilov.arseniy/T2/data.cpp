#include "data.hpp"
#include <ios>
#include <iomanip>
#include <streamGuard.hpp>
#include <delimiter.hpp>

shchadilov::KeyIO::KeyIO(DataStruct& data, std::array< bool, 3 > used):
  data(data),
  used(used)
{}

std::istream& shchadilov::operator>>(std::istream& in, DoubleI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref;
  char c = 0;
  in >> c;
  if (!in || (c != 'd' && c != 'D'))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& shchadilov::operator>>(std::istream& in, UllIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> std::oct >> dest.ref;
  return in;
}

std::istream& shchadilov::operator>>(std::istream& in, StringI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '\0';
  in >> c;
  if (!in || c != '"')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::getline(in, dest.ref, '"');
  return in;
}

std::istream& shchadilov::operator>>(std::istream& in, KeyIO& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> WordI{3, "key"};
  int key = 0;
  in >> key;
  if (key < 1 || key > 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (dest.used[key - 1])
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  dest.used[key - 1] = true;
  switch (key)
  {
  case 1:
    in >> DoubleI{ dest.data.key1 };
    break;
  case 2:
    in >> UllIO{ dest.data.key2 };
    break;
  case 3:
    in >> StringI{ dest.data.key3 };
    break;
  }
  if (in)
  {
    in >> DelimiterIO{ ':' };
  }
  return in;
}

std::istream& shchadilov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct temp{};
  KeyIO keyReader(temp);
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
  for (size_t i = 0; i < 3; ++i)
  {
    in >> keyReader;
    if (!in)
    {
      break;
    }
  }
  in >> DelimiterIO{ ')' };
  if (in)
  {
    dest = temp;
  }
  return in;
}

std::istream& shchadilov::operator>>(std::istream& in, WordI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char c = '0';
  for (int i = 0; i < dest.length; ++i)
  {
    in >> c;
    if (in && (dest.ref[i] != c)) {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::ostream& shchadilov::operator<<(std::ostream& out, const DoubleO& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  shchadilov::StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << dest.ref << 'd';
  return out;
}

std::ostream& shchadilov::operator<<(std::ostream& out, const UllO& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  shchadilov::StreamGuard guard(out);
  out << '0' << std::oct << dest.ref;
  return out;
}

std::ostream& shchadilov::operator<<(std::ostream& out, const StringO& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << '"' << dest.ref << '"';
  return out;
}

std::ostream& shchadilov::operator<<(std::ostream& out, const DataStruct& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  shchadilov::StreamGuard guard(out);
  out << "(:key1 " << DoubleO{ dest.key1 };
  out << ":key2 " << UllO{ dest.key2 };
  out << ":key3 " << StringO{ dest.key3 } << ":)";
  return out;
}

bool shchadilov::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}
