#include "datastruct.h"
#include <istream>
#include <complex>
#include <cctype>
#include <string>

namespace {
  using It = std::istreambuf_iterator<char>;

  void skipWhitespace(It& it, const It& end) {
    while (it != end && std::isspace(static_cast<unsigned char>(*it))) {
      ++it;
    }
  }

  bool matchPrefix(It& it, const It& end, const std::string& prefix) {
    auto tmp = it;
    for (char ch : prefix) {
      if (tmp == end || *tmp != ch) {
        return false;
      }
      ++tmp;
    }
    it = tmp;
    return true;
  }

  std::string parseIdentifier(It& it, const It& end) {
    std::string id;
    while (it != end && std::isalpha(static_cast<unsigned char>(*it))) {
      id += *it;
      ++it;
    }
    return id;
  }

  unsigned long long parseBinary(It& it, const It& end, std::istream& is) {
    if (!matchPrefix(it, end, "0b")) {
      is.setstate(std::ios::failbit);
      return 0;
    }

    unsigned long long value = 0;
    bool hasDigits = false;

    while (it != end && (*it == '0' || *it == '1')) {
      value = (value << 1) | (*it - '0');
      ++it;
      hasDigits = true;
    }

    if (!hasDigits) {
      is.setstate(std::ios::failbit);
    }

    return value;
  }

  std::complex<double> parseComplex(It& it, const It& end, std::istream& is) {
    if (!matchPrefix(it, end, "#c(")) {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }

    std::string token;
    while (it != end && !std::isspace(static_cast<unsigned char>(*it)) && *it != ')') {
      token += *it;
      ++it;
    }

    std::istringstream re_stream(token);
    double real = 0.0;
    re_stream >> real;
    if (re_stream.fail()) {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }

    skipWhitespace(it, end);

    token.clear();
    while (it != end && *it != ')') {
      token += *it;
      ++it;
    }

    double imag = 0.0;
    if (!token.empty()) {
      std::istringstream im_stream(token);
      im_stream >> imag;
      if (im_stream.fail()) {
        is.setstate(std::ios::failbit);
        return {0.0, 0.0};
      }
    }

    if (it == end || *it != ')') {
      is.setstate(std::ios::failbit);
      return {0.0, 0.0};
    }

    ++it;
    return {real, imag};
  }

  std::string parseQuotedString(It& it, const It& end, std::istream& is) {
    if (it == end || *it != '"') {
      is.setstate(std::ios::failbit);
      return {};
    }
    ++it;

    std::string str;
    while (it != end && *it != '"') {
      str += *it;
      ++it;
    }

    if (it == end || *it != '"') {
      is.setstate(std::ios::failbit);
      return {};
    }

    ++it;
    return str;
  }

  void skipToNextLine(It& it, const It& end)
  {
    while (it != end)
    {
      char ch = *it;
      ++it;
      if (ch == '\n') break;
    }
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data) {
  std::istreambuf_iterator<char> it(is), end;

  while (it != end) {
    DataStruct temp;
    bool has_key1 = false, has_key2 = false, has_key3 = false;

    skipWhitespace(it, end);
    while (it != end && *it == ':') {
      ++it;
      skipWhitespace(it, end);
      std::string key = parseIdentifier(it, end);
      skipWhitespace(it, end);

      if (key == "key1") {
        temp.key1 = parseBinary(it, end, is);
        has_key1 = !is.fail();
      } else if (key == "key2") {
        temp.key2 = parseComplex(it, end, is);
        has_key2 = !is.fail();
      } else if (key == "key3") {
        temp.key3 = parseQuotedString(it, end, is);
        has_key3 = !is.fail();
      } else {
        is.setstate(std::ios::failbit);
      }

      if (is.fail()) {
        skipToNextLine(it, end);
        is.clear(); // сбрасываем флаг ошибки
        break;
      }

      skipWhitespace(it, end);
    }

    if (has_key1 && has_key2 && has_key3) {
      data = temp;
      return is;
    }

    skipToNextLine(it, end);
    is.clear();
  }

  is.setstate(std::ios::failbit);
  return is;
}
