#include "datastruct.h"
#include <stdexcept>
#include <iterator>
#include <cctype>
#include <string>

namespace
{
  bool expect(char expected, std::istream_iterator<char>& in, const std::istream_iterator<char>& end) {
    if (in == end || *in != expected) return false;
    ++in;
    return true;
  }

  bool readDouble(double& num, std::istream_iterator<char>& in, const std::istream_iterator<char>& end) {
    std::string buffer;

    if (in != end && (*in == '-' || *in == '+')) {
      buffer += *in++;
    }

    while (in != end && std::isdigit(*in)) {
      buffer += *in++;
    }

    if (in == end || *in != '.') return false;
    buffer += *in++;  // consume '.'

    while (in != end && std::isdigit(*in)) {
      buffer += *in++;
    }

    try {
      num = std::stod(buffer);
    } catch (...) {
      return false;
    }

    return true;
  }

  bool readUllBin(unsigned long long& num, std::istream_iterator<char>& in, const std::istream_iterator<char>& end) {
    num = 0;
    while (in != end && *in != ':') {
      if (*in != '0' && *in != '1') return false;
      num = (num << 1) | (*in - '0');
      ++in;
    }
    return true;
  }

  bool readKey(asafov::DataStruct& data, std::istream_iterator<char>& in, const std::istream_iterator<char>& end) {
    if (!expect('k', in, end) || !expect('e', in, end) || !expect('y', in, end)) return false;

    if (in == end) return false;
    char keyType = *in++;

    if (keyType == '1') {
      if (!expect('0', in, end) || !expect('b', in, end)) return false;
      unsigned long long val;
      if (!readUllBin(val, in, end)) return false;
      data.key1 = val;
    }
    else if (keyType == '2') {
      if (!expect('#', in, end) || !expect('c', in, end) || !expect('(', in, end)) return false;
      double re, im;
      if (!readDouble(re, in, end)) return false;
      if (in == end || *in++ != ' ') return false;
      if (!readDouble(im, in, end)) return false;
      if (!expect(')', in, end)) return false;
      data.key2 = {re, im};
    }
    else if (keyType == '3') {
      if (!expect('"', in, end)) return false;
      std::string str;
      while (in != end && *in != '"') {
        str += *in++;
      }
      if (!expect('"', in, end)) return false;
      data.key3 = str;
    }
    else {
      return false;
    }

    return true;
  }
}

std::istream& asafov::operator>>(std::istream& is, DataStruct& data) {
  is >> std::noskipws;
  std::istream_iterator<char> in(is);
  std::istream_iterator<char> end;

  bool err = true;

  err = err && expect('(', in, end);
  err = err && expect(':', in, end);
  err = err && readKey(data, in, end);
  err = err && expect(':', in, end);
  err = err && readKey(data, in, end);
  err = err && expect(':', in, end);
  err = err && readKey(data, in, end);
  err = err && expect(':', in, end);
  err = err && expect(')', in, end);

  if (!err) {
    is.setstate(std::ios::failbit);
  }

  is >> std::skipws;
  return is;
}
