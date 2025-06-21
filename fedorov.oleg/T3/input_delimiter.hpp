#ifndef INPUT_DELIMITER
#define INPUT_DELIMITER

#include <istream>
#include <string>

namespace fedorov
{
  struct Line
  {
    std::string data;
  };

  std::istream &operator>>(std::istream &in, Line &line);

  struct DelimiterInput
  {
    char exp;
  };
  std::istream &operator>>(std::istream &in, DelimiterInput &&dest);
}
#endif
