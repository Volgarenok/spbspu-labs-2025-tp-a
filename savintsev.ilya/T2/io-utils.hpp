#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP
#include <cstddef>
#include <string>
#include <iostream>

namespace savintsev
{
  struct DoubleI
  {
    double & ref_;
  };

  struct DoubleO
  {
    double exp_;
  };

  struct UllIO
  {
    size_t & ref_;
  };

  struct StringIO
  {
    std::string & ref_;
  };

  struct LabelIO
  {
    std::string exp_;
  };

  std::istream & operator>>(std::istream & in, DoubleI && dest);
  std::istream & operator>>(std::istream & in, StringIO && dest);
  std::istream & operator>>(std::istream & in, UllIO && dest);
  std::istream & operator>>(std::istream & in, LabelIO & dest);

  std::ostream & operator<<(std::ostream & out, const DoubleO & dest);
}

#endif
