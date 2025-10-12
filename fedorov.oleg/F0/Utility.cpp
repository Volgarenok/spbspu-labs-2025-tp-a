#include "Utility.hpp"

namespace crossref
{
  void splitRecursive(const std::string &s, size_t start, std::vector< std::string > &words)
  {
    size_t end = s.find(' ', start);
    if (end == std::string::npos)
    {
      if (start < s.size())
      {
        words.push_back(s.substr(start));
      }
      return;
    }

    if (end > start)
    {
      words.push_back(s.substr(start, end - start));
    }

    splitRecursive(s, end + 1, words);
  }

  std::istream &operator>>(std::istream &is, Line &l)
  {
    return std::getline(is, l.data);
  }
}
