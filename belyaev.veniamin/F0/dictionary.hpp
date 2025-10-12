#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <iostream>
#include <vector>
#include <string>
#include <map>

namespace belyaev
{
  struct Dictionary
  {
    std::map< std::string, std::string > dict;
  };
  using entryPair = std::pair< const std::string, std::string >;
  using nonConstEntryPair = std::pair< std::string, std::string >;
  std::ostream& operator<<(std::ostream& out, const entryPair& pair);
  std::string formPairString(const entryPair& pair);
  std::ostream& operator<<(std::ostream& out, const Dictionary& dictionary);

  struct Dictionaries
  {
    std::map< std::string, Dictionary > dicts;
  };
}

#endif
