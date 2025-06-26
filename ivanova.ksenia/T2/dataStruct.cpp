#include "dataStruct.hpp"

#include <bitset>
#include <iomanip>

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
  std::bitset<64> bits(x.key);
  std::string binaryStr = bits.to_string();
  std::size_t firstOne = binaryStr.find('1');
  if (firstOne != std::string::npos)
  {
    std::string preffix(x.prefix_zeroes, '0');
    out << preffix << binaryStr.substr(firstOne);
  }
  else
  {
    out << "0";
  }
  return out;
}

std::ostream& ivanova::operator<<(std::ostream& out, const StringT& x)
{
  out << "\"" << x.key << "\"";
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
  std::string bits;
  char ch;
  x.prefix_zeroes = 0;
  bool first = true;
  while (in.get(ch) && (ch == '0' || ch == '1')) {
    bits += ch;
    if (first && ch == '0') {
      ++x.prefix_zeroes;
    } else {
      first = false;
    }
  }
  if (!bits.empty()) {
    x.key = std::stoull(bits, nullptr, 2);
    if (in) in.unget();
  } else {
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
  while (in >> next && next != '"')
  {
    if (next == '\n')
    {
      in.setstate(std::ios_base::failbit);
      break;
    }
    x.key.push_back(next);
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
  bool is_key1 = false, is_key2 = false, is_key3 = false;
  while ((!is_key1 || !is_key2 || !is_key3) && in)
  {
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
    }
  }
  in >> ExpectCharT{':'} >> ExpectCharT{')'};
  if (in)
  {
    ds = result;
  }
  return in;
}
