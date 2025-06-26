#include "datastruct.h"
#include <cctype>
#include <algorithm>
#include <sstream>

namespace
{
  unsigned long long parseULLBin(std::istream& is)
  {
    char ch;
    if (!is.get(ch) || ch != '0')
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    if (!is.get(ch) || ch != 'b')
    {
      is.setstate(std::ios::failbit);
      return 0;
    }

    unsigned long long result = 0;
    std::string bits;
    while (is.get(ch) && (ch == '0' || ch == '1'))
    {
      bits.push_back(ch);
    }
    if (bits.empty())
    {
      is.setstate(std::ios::failbit);
      return 0;
    }
    if (is)
    {
      is.unget();
    }

    for (char bit: bits)
    {
      result = (result << 1) | (bit == '1');
    }
    return result;
  }

  std::complex< double > parseCmpLsp(std::istream& is)
  {
    char ch;
    if (!is.get(ch) || ch != '#')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    if (!is.get(ch) || ch != 'c')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }
    if (!is.get(ch) || ch != '(')
    {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }

    double real = 0.0;
    double imag = 0.0;
    is >> real;
    if (!is)
    {
      return {0.0, 0.0};
    }

    if (is.get(ch) && ch != ')')
    {
      is.unget();
      is >> imag;
      if (!is)
      {
        return {0.0, 0.0};
      }
      if (!is.get(ch) || ch != ')')
      {
        is.setstate(std::ios::failbit);
        return {0.0, 0.0};
      }
    }
    return {real, imag};
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  std::string line;
  while (std::getline(is, line))
  {
    std::istringstream iss(line);
    DataStruct temp;
    bool has_key1 = false;
    bool has_key2 = false;
    bool has_key3 = false;
    char ch;

    while (iss.get(ch))
    {
      if (ch == ':')
      {
        std::string key;
        while (iss.get(ch) && ch != ' ' && ch != '\n' && ch != ':')
        {
          key.push_back(ch);
        }
        iss.unget();

        if (key == "key1")
        {
          iss >> std::ws;
          unsigned long long val = parseULLBin(iss);
          if (!iss.fail())
          {
            temp.key1 = val;
            has_key1 = true;
          }
        }
        else if (key == "key2")
        {
          iss >> std::ws;
          std::complex< double > val = parseCmpLsp(iss);
          if (!iss.fail())
          {
            temp.key2 = val;
            has_key2 = true;
          }
        }
        else if (key == "key3")
        {
          iss >> std::ws;
          if (iss.get(ch) && ch == '"')
          {
            std::string str;
            while (iss.get(ch) && ch != '"' && ch != '\n')
            {
              str.push_back(ch);
            }
            if (ch == '"')
            {
              temp.key3 = str;
              has_key3 = true;
            }
          }
        }
      }
    }

    if (has_key1 && has_key2 && has_key3)
    {
      data = temp;
      return is;
    }
  }

  is.setstate(std::ios::failbit);
  return is;
}
