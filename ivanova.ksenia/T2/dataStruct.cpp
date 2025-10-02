#include "dataStruct.hpp"
#include <iomanip>
#include <cmath>
#include <cstdio>
#include "streamGuard.hpp"

bool ivanova::dataStruct::operator<(const dataStruct& other)
{
  if (key1 != other.key1)
  {
    return key1 < other.key1;
  }
  if (key2 != other.key2)
  {
    return key2 < other.key2;
  }
  return key3.size() < other.key3.size();
}

std::ostream& ivanova::operator<<(std::ostream& out, const DoubleScienceT& x)
{
  char keyStr[32];
  std::snprintf(keyStr, sizeof(keyStr), "%.1e", x.key);
  std::string key(keyStr);
  size_t exp_pos = key.find('e');
  if (exp_pos != std::string::npos && (key[exp_pos + 2] == '0'))
  {
    key.erase(exp_pos + 2, 1);
  }
  out << key;
  return out;
}

std::ostream& ivanova::operator<<(std::ostream& out, const UllBinT& x)
{
  out << "0b";

  if (x.key == 0)
  {
    out << "0";
    return out;
  }

  for (size_t i = 0; i < x.prefix_zeroes; ++i)
  {
    out << '0';
  }

  unsigned long long temp = x.key;
  int bit_count = 0;
  while (temp > 0)
  {
    temp >>= 1;
    bit_count++;
  }

  temp = x.key;
  for (int i = bit_count - 1; i >= 0; --i)
  {
    out << ((temp >> i) & 1 ? '1' : '0');
  }

  return out;
}

std::ostream& ivanova::operator<<(std::ostream& out, const StringT& x)
{
  out << std::quoted(x.key);
  return out;
}

std::ostream& ivanova::operator<<(std::ostream& out, const dataStruct& ds)
{
  DoubleScienceT x { ds.key1 };
  UllBinT y { ds.key2, ds.key2_zeroes };
  StringT z { ds.key3 };

  StreamGuard guard(out);
  out << "(";
  out << ":key1 " << x;
  out << ":key2 " << y;
  out << ":key3 " << z;
  out << ":)";
  return out;
}

std::istream& ivanova::operator>>(std::istream& in, ExpectCharT&& x)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char ch;
  if (!(in >> ch) || ch != x.ch)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ivanova::operator>>(std::istream& in, DoubleScienceT& x)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> x.key;
  return in;
}

std::istream& ivanova::operator>>(std::istream& in, UllBinT& x)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  in >> ExpectCharT{'0'} >> ExpectCharT{'b'};

  x.key = 0;
  x.prefix_zeroes = 0;
  bool reading_prefix = true;
  char ch;
  bool has_bits = false;
  size_t bit_count = 0;
  const size_t max_bits = 64;

  while (in.get(ch) && (ch == '0' || ch == '1') && bit_count < max_bits)
  {
    has_bits = true;
    bit_count++;

    if (reading_prefix)
    {
      if (ch == '0')
      {
        ++x.prefix_zeroes;
      }
      else
      {
        reading_prefix = false;
        x.key = 1;
      }
    }
    else
    {
      x.key <<= 1;
      if (ch == '1')
      {
        x.key |= 1;
      }
    }
  }

  if (bit_count >= max_bits)
  {
    in.setstate(std::ios::failbit);
  }
  else if (has_bits)
  {
    if (in)
    {
      in.unget();
    }
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& ivanova::operator>>(std::istream& in, StringT& x)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  StreamGuard guard(in);
  in >> std::noskipws;
  in >> ExpectCharT{'"'};

  char next;
  const size_t max_length = 1000;
  x.key.clear();

  while (in >> next && next != '"' && x.key.size() < max_length)
  {
    if (next == '\n')
    {
      in.setstate(std::ios_base::failbit);
      break;
    }
    x.key.push_back(next);
  }

  if (x.key.size() >= max_length)
  {
    in.setstate(std::ios_base::failbit);
  }
  else if (next != '"')
  {
    in.setstate(std::ios_base::failbit);
  }

  return in;
}

std::istream& ivanova::operator>>(std::istream& in, dataStruct& ds)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> ExpectCharT{'('};
  dataStruct result;
  bool is_key1 = false;
  bool is_key2 = false;
  bool is_key3 = false;

  const size_t max_iterations = 10;
  size_t iteration_count = 0;

  while ((!is_key1 || !is_key2 || !is_key3) && in && iteration_count < max_iterations)
  {
    iteration_count++;
    in >> ExpectCharT{':'};

    std::string keyName;
    in >> keyName;

    if (keyName == "key1" && !is_key1)
    {
      DoubleScienceT x;
      is_key1 = true;
      in >> x;
      result.key1 = x.key;
    }
    else if (keyName == "key2" && !is_key2)
    {
      UllBinT x;
      is_key2 = true;
      in >> x;
      result.key2 = x.key;
      result.key2_zeroes = x.prefix_zeroes;
    }
    else if (keyName == "key3" && !is_key3)
    {
      StringT x;
      is_key3 = true;
      in >> x;
      result.key3 = x.key;
    }
    else
    {
      in.setstate(std::ios::failbit);
      break;
    }
  }

  if (iteration_count >= max_iterations)
  {
    in.setstate(std::ios::failbit);
  }
  else if (in)
  {
    in >> ExpectCharT{':'} >> ExpectCharT{')'};
    if (in)
    {
      ds = result;
    }
  }

  return in;
}
