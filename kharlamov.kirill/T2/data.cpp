#include "data.h"
#include <cctype>
#include <istream>
#include <ostream>
#include <guard.h>

std::istream& kharlamov::operator>>(std::istream& in, kharlamov::SymbolIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c;
  in >> c;
  if (in && std::tolower(c) != std::tolower(dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kharlamov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::getline(in >> SymbolIO{ '"' }, dest.ref, '"');
  return in;
}

std::istream& kharlamov::operator>>(std::istream& in, kharlamov::LongLongIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref >> kharlamov::SymbolIO{ 'l' } >> kharlamov::SymbolIO{ 'l' };
  return in;
}

std::istream& kharlamov::operator>>(std::istream& in, kharlamov::UnsignedLongLongIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> dest.ref >> kharlamov::SymbolIO{ 'u' } >> kharlamov::SymbolIO{ 'l' } >> kharlamov::SymbolIO{ 'l' };
  return in;
}

std::istream& kharlamov::operator>>(std::istream& in, kharlamov::DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Guard scope{ in };
  DataStruct temp;
  using sep = kharlamov::SymbolIO;
  in >> sep{ '(' };
  bool haskey1 = false;
  bool haskey2 = false;
  bool haskey3 = false;
  for (int i = 0; i < 3; i++)
  {
    in >> sep{ ':' } >> sep{ 'k' } >> sep{ 'e' } >> sep{ 'y' };
    int a;
    in >> a;
    if (a == 1 && !haskey1)
    {
      in >> LongLongIO{ temp.key1 };
      haskey1 = true;
    }
    else if (a == 2 && !haskey2)
    {
      in >> UnsignedLongLongIO{ temp.key2 };
      haskey2 = true;
    }
    else if (a == 3 && !haskey3)
    {
      in >> StringIO{ temp.key3 };
      haskey3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  in >> sep{ ':' } >> sep{ ')' };
  if (in && haskey1 && haskey2 && haskey3)
  {
    dest = std::move(temp);
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

bool kharlamov::compare(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  else if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}

std::ostream& kharlamov::operator<<(std::ostream& out, const kharlamov::DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  Guard scope{ out };
  out << "(:key1 " << src.key1 << "ll";
  out << ":key2 " << src.key2 << "ull";
  out << ":key3 \"" << src.key3 << "\":)";
  return out;
}
