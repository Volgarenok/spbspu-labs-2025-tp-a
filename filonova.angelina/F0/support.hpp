#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <iostream>
#include <string>
#include <unordered_map>

namespace filonova
{
  using Dictionary = std::unordered_map< std::string, size_t >;
  using DictionarySet = std::unordered_map< std::string, Dictionary >;

  struct Word
  {
    std::string text;
  };

  bool isLetter(char c);
  char toLower(char c);
  bool isPunct(char c);
  std::istream &operator>>(std::istream &in, Word &w);
  std::ostream &operator<<(std::ostream &out, const Word &w);
  bool operator<(const Word &lhs, const Word &rhs);

  struct IsValidNameChar
  {
    bool operator()(char ch) const;
  };

  bool isValidName(const std::string &name);
  std::string printPair(const std::pair< std::string, size_t > &p);

  struct MergeDictEntry
  {
    Dictionary &destination;
    explicit MergeDictEntry(Dictionary &d);
    std::pair< const std::string, size_t > operator()(const std::pair< const std::string, size_t > &entry) const;
  };

  struct CompareByFrequency
  {
    bool descending;
    explicit CompareByFrequency(bool desc);
    bool operator()(const std::pair< std::string, size_t > &a, const std::pair< std::string, size_t > &b) const;
  };

  struct WordIntersectFilter
  {
    const Dictionary &dict2;
    explicit WordIntersectFilter(const Dictionary &d);
    bool operator()(const std::pair< const std::string, size_t > &entry) const;
  };

  struct WordExcludeFilter
  {
    const Dictionary &dict2;
    explicit WordExcludeFilter(const Dictionary &d);
    bool operator()(const std::pair< const std::string, size_t > &entry) const;
  };

  void printWords(const DictionarySet &dicts, const std::string &name, std::ostream &out, size_t limit, bool descending);
}

#endif
