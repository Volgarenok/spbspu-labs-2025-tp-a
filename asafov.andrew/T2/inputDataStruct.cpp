#include "datastruct.h"
#include <stdexcept>

namespace
{
  std::string trim(const std::string& s)
  {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
  }

  unsigned long long parseBinaryULL(const std::string& s)
  {
    if (s.size() < 2 || (s[0] != '0' || (s[1] != 'b' && s[1] != 'B')))
    {
      throw std::logic_error("invalid binary format");
    }
    const char* str = s.c_str() + 2;
    char* endptr = nullptr;
    unsigned long long value = std::strtoull(str, &endptr, 2);
    if (endptr == str || *endptr != '\0')
    {
      throw std::logic_error("invalid binary format");
    }
    return value;
  }

  std::complex<double> parseComplex(const std::string& s)
  {
    if (s.size() < 5 || s.substr(0, 2) != "#c" || s[2] != '(' || s.back() != ')')
    {
      throw std::logic_error("invalid complex format");
    }

    std::string content = s.substr(3, s.size() - 4);
    size_t space_pos = content.find(' ');
    if (space_pos == std::string::npos)
    {
      throw std::logic_error("invalid complex format");
    }

    std::string real_str = trim(content.substr(0, space_pos));
    std::string imag_str = trim(content.substr(space_pos + 1));

    char* endptr = nullptr;
    double real = std::strtod(real_str.c_str(), &endptr);
    if (endptr == real_str.c_str() || *endptr != '\0')
    {
      throw std::logic_error("invalid complex format");
    }

    endptr = nullptr;
    double imag = std::strtod(imag_str.c_str(), &endptr);
    if (endptr == imag_str.c_str() || *endptr != '\0')
    {
      throw std::logic_error("invalid complex format");
    }

    return std::complex<double>(real, imag);
  }

  std::string parseQuotedString(const std::string& s)
  {
    if (s.size() < 2 || s.front() != '"' || s.back() != '"')
    {
      throw std::invalid_argument("invalid string format");
    }
    return s.substr(1, s.size() - 2);
  }

  asafov::DataStruct parseDataStruct(const std::string& input)
  {
    asafov::DataStruct result;
    size_t pos = 0;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

    while (pos < input.size())
    {
      if (input[pos] != ':')
      {
        pos++;
        continue;
      }

      size_t key_start = pos + 1;
      size_t key_end = input.find_first_of(" \t\n\r", key_start);
      if (key_end == std::string::npos) key_end = input.size();
      std::string key = input.substr(key_start, key_end - key_start);

      pos = key_end;
      while (pos < input.size() && (input[pos] == ' ' || input[pos] == '\t')) pos++;

      size_t value_end = input.find(':', pos);
      if (value_end == std::string::npos) value_end = input.size();
      std::string value = trim(input.substr(pos, value_end - pos));

      if (key == "key1")
      {
        result.key1 = parseBinaryULL(value);
        hasKey1 = true;
      }
      else if (key == "key2")
      {
        result.key2 = parseComplex(value);
        hasKey2 = true;
      }
      else if (key == "key3")
      {
        result.key3 = parseQuotedString(value);
        hasKey3 = true;
      }
      else
      {
        throw std::logic_error("Unknown key: " + key);
      }

      pos = value_end;
    }

    if (!hasKey1 || !hasKey2 || !hasKey3)
    {
      throw std::logic_error("missing required keys in DataStruct");
    }

    return result;
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data)
{
  std::string line;
  if (!std::getline(is, line))
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  try
  {
    data = parseDataStruct(line);
  }
  catch (const std::exception& e)
  {
    is.setstate(std::ios::failbit);
    return is;
  }
  return is;
}
