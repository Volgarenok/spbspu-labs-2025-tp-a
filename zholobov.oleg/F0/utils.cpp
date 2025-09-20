#include "utils.hpp"

std::vector< std::string > zholobov::splitTokens(const std::string& line)
{
  std::vector< std::string > tokens;
  std::size_t pos = 0;
  while (true) {
    pos = line.find_first_not_of(" \t\r\n", pos);
    if (pos == std::string::npos) {
      break;
    }
    std::size_t end = line.find_first_of(" \t\r\n", pos);
    tokens.push_back(line.substr(pos, end - pos));
    if (end == std::string::npos) {
      break;
    }
    pos = end + 1;
  }
  return tokens;
}
