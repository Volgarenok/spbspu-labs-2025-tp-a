#include "data_struct.hpp"

#include <sstream>
#include <regex>
#include <iomanip>

namespace {

  bool parse_rational(std::istream &in, std::pair<long long, unsigned long long> &rat) {
    std::string token;
    std::getline(in, token, ')');

    std::regex rat_regex(R"(\(:N (-?\d+):D (\d+):\))");
    std::smatch match;

    std::string full = token + ")";
    if (std::regex_match(full, match, rat_regex)) {
      rat.first = std::stoll(match[1]);
      rat.second = std::stoull(match[2]);
      return true;
    }

    return false;
  }

  bool parse_complex(std::istream &in, std::complex<double> &c) {
    std::string token;
    std::getline(in, token, ')');

    std::regex comp_regex(
      R"(#c\((-?\d+\.?\d*(e[+-]?\d+)?)[ ]+(-?\d+\.?\d*(e[+-]?\d+)?)\))",
      std::regex::icase
    );
    std::smatch match;

    if (std::regex_search(token, match, comp_regex)) {
      double re = std::stod(match[1]);
      double im = std::stod(match[3]);
      c = { re, im };
      return true;
    }

    return false;
  }

}

std::istream &operator>>(std::istream &in, data_struct_t &data) {
  std::string line;
  std::getline(in, line);

  if (line.empty() || line.front() != '(' || line.back() != ')') {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::istringstream iss(line.substr(1, line.size() - 2));
  std::string token;
  bool has_key1 = false;
  bool has_key2 = false;
  bool has_key3 = false;

  while (std::getline(iss, token, ':')) {
    if (token == "key1") {
      std::string rest;
      std::getline(iss, rest, ':');
      std::istringstream tmp("#c" + rest);
      has_key1 = parse_complex(tmp, data.key1);
    } else if (token == "key2") {
      std::string rest;
      std::getline(iss, rest, ':');
      std::istringstream tmp(rest + ":)");
      has_key2 = parse_rational(tmp, data.key2);
    } else if (token == "key3") {
      std::string value;
      if (std::getline(iss, value, ':')) {
        if (value.front() == '"' && value.back() == '"') {
          data.key3 = value.substr(1, value.size() - 2);
          has_key3 = true;
        }
      }
    }
  }

  if (!(has_key1 && has_key2 && has_key3)) {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream &operator<<(std::ostream &out, const data_struct_t &data) {
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
