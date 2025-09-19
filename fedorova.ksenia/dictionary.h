#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <iostream>
#include <list>
#include <map>

namespace fedorova
{
  using Dictionary = std::map< std::string, std::list< std::string > >;
  using DictSet = std::map< std::string, Dictionary >;
  using DictContent = std::map< char, std::list< std::string > >;

  struct DictPairWrapper
  {
    std::pair< std::string, std::list< std::string > > p;
  };
  struct DictSetPairWrapper
  {
    std::pair< std::string, Dictionary > p;
  };
  struct ContentPairWrapper
  {
    std::pair< char, std::list< std::string > > p;
  };
  std::istream& operator>>(std::istream& is, std::list< std::string >& list);
  std::istream& operator>>(std::istream& is, Dictionary& dict);
  std::istream& operator>>(std::istream& is, DictSet& set);
  std::istream& operator>>(std::istream& is, DictPairWrapper& pair);
  std::istream& operator>>(std::istream& is, DictSetPairWrapper& pair);
  std::ostream& operator<<(std::ostream& os, const std::list< std::string >& list);
  std::ostream& operator<<(std::ostream& os, const Dictionary& dict);
  std::ostream& operator<<(std::ostream& os, const DictSet& set);
  std::ostream& operator<<(std::ostream& os, const DictPairWrapper& pair);
  std::ostream& operator<<(std::ostream& os, const DictSetPairWrapper& pair);
  std::ostream& operator<<(std::ostream& os, const ContentPairWrapper& pair);
  std::ostream& operator<<(std::ostream& os, const DictContent& dict);
}
#endif

