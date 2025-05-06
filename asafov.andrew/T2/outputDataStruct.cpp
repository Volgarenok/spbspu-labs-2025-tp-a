#include "datastruct.h"
#include <iterator>
#include <algorithm>

namespace
{
  std::string cmpLspToStr(const std::complex<double> data)
  {
    std::string str;
    str += '#';
    str += 'c';
    str += '(';
    std::string temp = std::to_string(data.real());
    temp.erase(temp.find_last_not_of('0') + 1, std::string::npos);
    if (temp.back() == '.') temp.pop_back();
    str += temp;
    str += ' ';
    temp = std::to_string(data.imag());
    temp.erase(temp.find_last_not_of('0') + 1, std::string::npos);
    if (temp.back() == '.') temp.pop_back();
    str += temp;
    str += ')';
    return str;
  }

  std::string ullBinToStr(unsigned long long num)
  {
    if (num == 0) return "0b0";
    std::string binary;
    while (num > 0)
    {
      binary += (num & 1) ? '1' : '0';
      num >>= 1;
    }
    std::reverse(binary.begin(), binary.end());
    return "0b" + binary;
  }
}

std::ostream& asafov::operator<<(std::ostream& os, const DataStruct& data)
{
  std::ostream_iterator<char> oit(os);
  const std::string out = "(:key1 " + ullBinToStr(data.key1) + ":key2 " +
    cmpLspToStr(data.key2) + ":key3 \"" + data.key3 + "\":)";
  for (const auto ch : out) *(oit++) = ch;
  return os;
}
