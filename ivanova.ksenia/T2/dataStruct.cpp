#include <iomanip>
#include <bitset>
#include "dataStruct.hpp"

#include <iostream>

namespace ivanova
{
  inline std::string::size_type endKey(const std::string& line, std::string::size_type start)
  {
    bool in_raw = false;
    for (std::string::size_type i = start; i < line.size(); ++i)
    {
      if (line[i] == ':' && !in_raw)
      {
        return i;
      }
      if (line[i] == '"')
      {
        in_raw = !in_raw;
      }
    }
    return line.size();
  }

  inline std::string getKey(const std::string& line, const std::string& key_preffix)
  {
    std::string::size_type key_pos = line.find(key_preffix);
    if (key_pos == std::string::npos)
    {
      throw std::logic_error("");
    }
    std::string::size_type key_end = endKey(line, key_pos + key_preffix.size());
    return line.substr(key_pos + key_preffix.size(), key_end - (key_pos + key_preffix.size()));
  }

  inline std::string::size_type countZeroes(const std::string& key) {
    if (key.empty()) {
      return 0;
    }
    std::string::size_type cnt = 0;
    while (cnt < key.size() && key[cnt] == '0') {
      ++cnt;
    }
    return cnt;
  }

  std::istream& operator>>(std::istream& in, dataStruct& ds)
  {
    try
    {
      std::string line;
      std::getline(in, line);
      if (line.empty() || line.front() != '(' || line.back() != ')')
      {
        throw std::logic_error("");
      }
      std::string key1 = getKey(line, ":key1 ");
      std::string key2 = getKey(line, ":key2 ");
      std::string key3 = getKey(line, ":key3 ");

      if (key2.size() <= 2 || (key2.substr(0, 2) != "0b" && key2.substr(0, 2) != "0B") || key3.size() < 2)
      {
        throw std::logic_error("");
      }
      ds.key1 = std::stod(key1, nullptr);
      ds.key2 = std::stoull(key2.substr(2), nullptr, 2);
      ds.key2_zeroes = countZeroes(key2.substr(2));
      ds.key3 = key3.substr(1, key3.size() - 2);
    }
    catch (...)
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const dataStruct& ds)
  {
    char key1Str[32];
    snprintf(key1Str, sizeof(key1Str), "%.1e", ds.key1);
    std::string key1(key1Str);

    if (*(key1.end() - 2) == '0')
    {
      key1.erase(key1.end() - 2);
    }
    out << "(:key1 " << key1;

    out << ":key2 0b";
    std::bitset<64> bits(ds.key2);
    std::string binaryStr = bits.to_string();

    size_t firstOne = binaryStr.find('1');
    if (firstOne != std::string::npos)
    {
      std::string preffix(ds.key2_zeroes, '0');
      out << preffix << binaryStr.substr(firstOne);
    }
    else
    {
      out << "0";
    }

    out << ":key3 \"" << ds.key3 << "\":)";

    out << std::defaultfloat;
    return out;
  }

  bool compareDataStructs(const ivanova::dataStruct& a, const ivanova::dataStruct& b)
  {
    if (a.key1 != b.key1)
    {
      return a.key1 < b.key1;
    }
    else if (a.key2 != b.key2)
    {
      return a.key2 < b.key2;
    }
    else
    {
      return a.key3.length() < b.key3.length();
    }
  }
}
