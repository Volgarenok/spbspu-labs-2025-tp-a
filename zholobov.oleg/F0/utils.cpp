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

std::istream& operator>>(std::istream& in, zholobov::Words& words)
{
  words.clear();
  std::string line;
  if (!std::getline(in, line)) {
    in.setstate(std::ios::eofbit);
    return in;
  }
  std::vector< std::string > toks = zholobov::splitTokens(line);
  for (const auto& t: toks) {
    words.push_back(t);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const zholobov::Words& words)
{
  if (!words.empty()) {
    auto it = words.cbegin();
    out << *it++;
    for (; it != words.cend(); ++it) {
      out << " " << *it;
    }
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const zholobov::Dictionary& dict)
{
  for (const auto& entry: dict) {
    out << entry.first;
    if (!entry.second.empty()) {
      out << " " << entry.second;
    }
    out << '\n';
  }
  return out;
}
