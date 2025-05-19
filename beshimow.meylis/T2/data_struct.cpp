#include "data_struct.hpp"

#include <regex>
#include <sstream>
#include <iomanip>
#include <algorithm>

bool isValidRecord(const std::string& line)
{
  std::regex format(R"(^\(\s*:.*:\s*\)$)");
  std::regex key1Re(R"delim(:key1\s+[+-]?\d+(\.\d+)?[eE][+-]?\d+:)delim");
  std::regex key2Re(R"delim(:key2\s+\(:N\s+-?\d+:D\s+\d+:\):)delim");
  std::regex key3Re(R"delim(:key3\s+"[^"]*":)delim");

  return std::regex_match(line, format)
    && std::regex_search(line, key1Re)
    && std::regex_search(line, key2Re)
    && std::regex_search(line, key3Re);
}

bool parseRecord(const std::string& line, DataStruct& data)
{
  std::smatch match;

  std::regex key1Re(R"delim(:key1\s+([+-]?\d+(?:\.\d+)?[eE][+-]?\d+):)delim");
  if (!std::regex_search(line, match, key1Re)) {
    return false;
  }
  data.key1 = std::stod(match[1]);

  std::regex key2Re(R"delim(:key2\s+\(:N\s+(-?\d+):D\s+(\d+):\):)delim");
  if (!std::regex_search(line, match, key2Re)) {
    return false;
  }
  data.key2 = {std::stoll(match[1]), std::stoull(match[2])};

  std::regex key3Re(R"delim(:key3\s+"([^"]*)":)delim");
  if (!std::regex_search(line, match, key3Re)) {
    return false;
  }
  data.key3 = match[1];

  return true;
}

void sortData(std::vector<DataStruct>& data)
{
  std::sort(data.begin(), data.end(),
    [](const DataStruct& a, const DataStruct& b) {
      if (a.key1 != b.key1) return a.key1 < b.key1;
      if (a.key2 != b.key2) return a.key2 < b.key2;
      return a.key3.length() < b.key3.length();
    });
}

void printData(std::ostream& out, const DataStruct& data)
{
  out << "(:key1 " << std::scientific << std::setprecision(6) << data.key1
      << ":key2 (:N " << data.key2.first << ":D " << data.key2.second << ":)"
      << ":key3 \"" << data.key3 << "\":)";
}
