#include "processing_structures.hpp"
#include <iostream>
#include <stack>
#include <stream_guardian.hpp>

bool zakirov::Data::operator<(const Data & other)
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }
  else if (key2 != other.key2)
  {
    return key2 < other.key2;
  }

  return key3.size() < key3.size();
}

std::istream & zakirov::operator>>(std::istream & in, Data & data)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Data received;
  in >> MinorSymbol{'('};
  in >> MinorSymbol{':'};
  std::string key;
  bool key_flag1 = false;
  bool key_flag2 = false;
  bool key_flag3 = false;
  for (size_t i = 0; i < 3; ++i)
  {
    in >> key;
    if (key == "key1" && !key_flag1)
    {
      in >> UllOctIO{received.key1};
      key_flag1 = true;
    }
    else if (key == "key2" && !key_flag2)
    {
      in >> UllHexIO{received.key2};
      key_flag2 = true;
    }
    else if (key == "key3" && !key_flag3)
    {
      in >> StringIO{received.key3};
      key_flag3 = true;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }

  in >> MinorSymbol{')'};
  if (in)
  {
    data = received;
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, MinorSymbol && sym)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char symbol;
  if (!(in >> symbol) || symbol != sym.symbol)
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, MinorLetter && sym)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  char symbol;
  if (!(in >> symbol) || (symbol != std::tolower(sym.letter) && symbol != std::toupper(sym.letter)))
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, UllOctIO && num)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  unsigned long long oct_num = 0;
  in >> std::oct >> oct_num;
  in >> MinorSymbol{':'};
  num.ref = oct_num;
  return in;
}

std::istream & zakirov::operator>>(std::istream & in, UllHexIO && num)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  in >> MinorSymbol{'0'} >> MinorLetter{'x'};
  unsigned long long hex_num = 0;
  in >> std::hex >> hex_num;
  in >> MinorSymbol{':'};
  num.ref = hex_num;
  return in;
}

std::istream & zakirov::operator>>(std::istream & in, StringIO && str)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  Guardian guardian(in);

  in >> MinorSymbol{'"'};
  std::string string_r = "";
  char next_symbol;
  in >> std::noskipws >> next_symbol;
  while (next_symbol != '"')
  {
    string_r += next_symbol;
    in >> next_symbol;
  }

  in >> MinorSymbol{':'};
  str.exp = string_r;
  return in;
}

std::ostream & zakirov::operator<<(std::ostream & out, const Data & src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  Guardian guardian(out);
  out << "(:key1 0";
  out << std::oct << src.key1;
  out << ":key2 0x";
  out << std::hex << std::uppercase << src.key2;
  out << ":key3 \"" << src.key3;
  out << "\":)";
  return out;
}
