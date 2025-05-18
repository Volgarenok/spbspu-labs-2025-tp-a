#include "data_struct.hpp"
#include <sstream>
#include <regex>
#include <iomanip>
#include <iostream>

namespace {

bool parseRational(std::istream& in, std::pair<long long, unsigned long long>& rat)
{
  std::string token;
  std::getline(in, token, ')');

  std::regex ratRegex(R"(\(:N (-?\d+):D (\d+):\))");
  std::smatch match;

  if (std::regex_search(token, match, ratRegex))
  {
    rat.first = std::stoll(match[1]);
    rat.second = std::stoull(match[2]);
    return true;
  }
  return false;
}

bool parseComplex(std::istream& in, std::complex<double>& c)
{
  std::string token;
  std::getline(in, token, ')');

  std::regex compRegex(
    R"(#c\((-?\d+\.?\d*)(?:e[+-]?\d+)?[ ]+(-?\d+\.?\d*)(?:e[+-]?\d+)?\))",
    std::regex::icase
  );
  std::smatch match;

  if (std::regex_search(token, match, compRegex))
  {
    double re = std::stod(match[1]);
    double im = std::stod(match[2]);
    c = {re, im};
    return true;
  }
  return false;
}

}

std::istream& operator>>(std::istream& in, data_struct_t& data)
{
  std::string line;
  if (!std::getline(in, line))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  if (line.empty() || line.front() != '(' || line.back() != ')')
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::istringstream iss(line.substr(1, line.size() - 2));
  std::string token;
  bool hasKey1 = false;
  bool hasKey2 = false;
  bool hasKey3 = false;

  while (std::getline(iss, token, ':'))
  {
    if (token == "key1")
    {
      std::string rest;
      if (std::getline(iss, rest, ':'))
      {
        std::istringstream tmp("#c" + rest);
        hasKey1 = parseComplex(tmp, data.key1);
      }
    }
    else if (token == "key2")
    {
      std::string rest;
      if (std::getline(iss, rest, ':'))
      {
        std::istringstream tmp("(:N" + rest + ":)");
        hasKey2 = parseRational(tmp, data.key2);
      }
    }
    else if (token == "key3")
    {
      std::string value;
      if (std::getline(iss, value, ':'))
      {
        if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
        {
          data.key3 = value.substr(1, value.size() - 2);
          hasKey3 = true;
        }
      }
    }
  }

  if (!(hasKey1 && hasKey2 && hasKey3))
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, const data_struct_t& data)
{
  out << "(:key1 #c("
      << std::fixed << std::setprecision(1)
      << data.key1.real() << " "
      << data.key1.imag()
      << "):key2 (:N "
      << data.key2.first << ":D "
      << data.key2.second
      << ":):key3 \""
      << data.key3 << "\":)";
  return out;
}
