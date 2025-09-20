#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <iostream>
#include <string>
#include <functional>
#include <vector>

namespace crossref
{

  struct Line
  {
    std::string data;
  };

  std::istream &operator>>(std::istream &is, Line &l);
  void splitRecursive(const std::string &s, size_t start, std::vector< std::string > &words);
}

#endif
