#include "DataStruct.hpp"

int quotesCount(const std::string& str)
{
  int count = 0;
  for (const auto c: str)
  {
    if (c == '"')
    {
      ++count;
    }
  }
  return count;
}
std::istream& operator>>(std::istream& in, DataStruct& data)
{
  std::string line;
  DataStruct tempData;
  data.key1 = tempData.key1;
  data.key2 = tempData.key2;
  data.key3 = tempData.key3;
  if (!std::getline(in, line))
  {
    return in;
  }
  if (line.front() != '(' || line.back() != ')')
  {
    return in;
  }
  try
  {
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    std::istringstream iss(line.substr(1, line.size() - 2));
    std::string token;
    while (std::getline(iss, token, ':'))
    {
      if (token.empty()) continue;
      std::string key = token.substr(0, 4);
      if (key == "key3")
      {
        if (quotesCount(token) < 2)
        {
          token += ':';
        }
        while (quotesCount(token) < 2)
        {
          char c;
          iss >> c;
          if (iss.eof())
          {
            return in;
          }
          token += c;
        }
      }
      std::string value = token.substr(4);
      std::istringstream val_stream(value);
      if (key == "key1")
      {
        if (hasKey1)
        {
          return in;
        }
        val_stream >> value;
        if (value.size() < 4)
        {
          return in;
        }
        if (value.substr(value.size() - 3) != "ull" && value.substr(value.size() - 3) != "ULL")
        {
          return in;
        }
        tempData.key1 = std::stoull(value.substr(0, value.size() - 3));
        hasKey1 = true;
      }
      else if (key == "key2")
      {
        if (hasKey2)
        {
          return in;
        }
        char c;
        val_stream >> c;
        if (c != '#')
        {
          return in;
        }
        val_stream >> c;
        if (c != 'c')
        {
          return in;
        }
        val_stream >> c;
        if (c != '(')
        {
          return in;
        }
        double real, imag;
        val_stream >> real >> imag;
        val_stream >> c;
        if (c != ')')
        {
          return in;
        }
        tempData.key2 = {real, imag};
        hasKey2 = true;
      }
      else if (key == "key3")
      {
        if (hasKey3)
        {
          return in;
        }
        std::string value = val_stream.str().substr(1);
        if (value.size() < 2 || value.front() != '"' || value.back() != '"')
        {
          return in;
        }
        tempData.key3 = value.substr(1, value.size() - 2);
        hasKey3 = true;
      }
    }
    if (!hasKey1 || !hasKey2 || !hasKey3)
    {
      return in;
    }
    data.key1 = tempData.key1;
    data.key2 = tempData.key2;
    data.key3 = tempData.key3;

  }
  catch (...)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
  out << "(:key1 " << data.key1 << "ull"
      << ":key2 #c(" << std::fixed << std::setprecision(1)
      << static_cast<double>(data.key2.real()) << " "
      << static_cast<double>(data.key2.imag()) << ")"
      << ":key3 \"" << data.key3 << "\":)";
  return out;
}
