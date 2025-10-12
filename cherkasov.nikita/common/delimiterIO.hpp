#ifndef DELIMITERIO_HPP
#define DELIMITERIO_HPP

#include <istream>

class DelimiterIO
{
public:
  explicit DelimiterIO(char c): ch_(c) {}
  friend std::istream& operator>>(std::istream& in, const DelimiterIO& d);
private:
  char ch_;
};

#endif
