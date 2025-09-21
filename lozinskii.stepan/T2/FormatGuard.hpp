#ifndef FORMATGUARD_HPP
#define FORMATGUARD_HPP

#include <ios>

class FormatGuard
{
public:
  explicit FormatGuard(std::ios& stream);
  ~FormatGuard();

private:
  std::ios& stream_;
  std::ios saved_;
};

#endif
