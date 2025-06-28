#ifndef DATAIO_HPP
#define DATAIO_HPP

#include <istream>
#include <string>
#include <utility>
#include "iofmtguard.hpp"

namespace averenkov
{
  struct DelimiterIO
  {
    char exp;
  };
  struct CharIO
  {
    char& ref;
  };
  struct RationalIO
  {
    std::pair< long long, unsigned long long >& ref;
  };
  struct StringIO
  {
    std::string& ref;
  };
  struct LabelIO
  {
    std::string exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, CharIO&& dest);
  std::istream& operator>>(std::istream& in, RationalIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
}

#endif
