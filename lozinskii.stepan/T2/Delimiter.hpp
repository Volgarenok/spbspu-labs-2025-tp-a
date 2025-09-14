#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <istream>

class DelimiterIstream
{
public:
  explicit DelimiterIstream(const std::string& expected);

  friend std::istream& operator>>(std::istream& in, const DelimiterIstream& delim);

private:
  std::string expected_;
};

#endif
