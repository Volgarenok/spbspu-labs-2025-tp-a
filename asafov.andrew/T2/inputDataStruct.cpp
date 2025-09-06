#include "datastruct.h"
#include <cctype>
#include <algorithm>

namespace
{
  unsigned long long parseULLBin(const std::string& str)
  {
    if (str.length() < 3 || (str[0] != '0' && str[1] != 'b'))
    {
      return 0;
    }

    unsigned long long result = 0;
    for (size_t i = 2; i < str.size(); ++i)
    {
      result = result << 1;
      if (str[i] == '1')
      {
        result = result | 1;
      }
      else if (str[i] != '0')
      {
        return 0;
      }
    }
    return result;
  }

  std::complex< double > parseCmpLsp(const std::string& str)
  {
    if (str.length() < 5 || str[0] != '#' || str[1] != 'c' || str[2] != '(')
    {
      return {0.0, 0.0};
    }

    size_t start = 3;
    size_t end = str.find_first_of(" )", start);
    double real = 0.0;
    double imag = 0.0;

    std::string real_str = str.substr(start, end - start);
    try
    {
      real = std::stod(real_str);
    }
    catch (const std::exception&)
    {
      return {0.0, 0.0};
    }
    if (end != std::string::npos && str[end] != ')')
    {
      start = end + 1;
      end = str.find_first_of(" )", start);
      std::string imag_str = str.substr(start, end - start);
      try
      {
        imag = std::stod(imag_str);
      }
      catch (const std::exception&)
      {
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
    DataStruct temp;
    bool has_key1 = false;
    bool has_key2 = false;
    bool has_key3 = false;

    size_t key1_pos = line.find(":key1 ");
    if (key1_pos != std::string::npos)
    {
      size_t key1_end = line.find(':', key1_pos + 1);
      if (key1_end != std::string::npos)
      {
        std::string key1_str = line.substr(key1_pos + 6, key1_end - (key1_pos + 6));
        temp.key1 = parseULLBin(key1_str);
        if (temp.key1 != 0 || key1_str == "0b0")
        {
          has_key1 = true;
        }
      }
    }

    size_t key2_pos = line.find(":key2 ");
    if (key2_pos != std::string::npos)
    {
      size_t key2_end = line.find(':', key2_pos + 1);
      if (key2_end != std::string::npos)
      {
        std::string key2_str = line.substr(key2_pos + 6, key2_end - (key2_pos + 6));
        temp.key2 = parseCmpLsp(key2_str);
        if (temp.key2 != std::complex< double >{0.0, 0.0} || key2_str == "#c(0.0 0.0)" || key2_str == "#c(0 0)" || key2_str == "#c(0. 0.)")
        {
          has_key2 = true;
        }
      }
    }

    size_t key3_pos = line.find(":key3 \"");
    if (key3_pos != std::string::npos)
    {
      size_t quote_pos = key3_pos + 7;
      size_t closing_quote = line.find('"', quote_pos);
      if (closing_quote != std::string::npos)
      {
        temp.key3 = line.substr(quote_pos, closing_quote - quote_pos);
        if (line.find(':', closing_quote + 1) != std::string::npos)
        {
          has_key3 = true;
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
