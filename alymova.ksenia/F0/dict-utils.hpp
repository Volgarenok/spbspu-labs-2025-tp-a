#ifndef DICT_UTILS_HPP
#define DICT_UTILS_HPP
#include <iostream>
#include <list>
#include <map>

namespace alymova
{
  using Dictionary = std::map< std::string, std::list< std::string > >;
  using DictSet = std::map< std::string, Dictionary >;
  using ContentDict = std::map< char, std::list< std::string > >;

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
  std::istream& operator>>(std::istream& in, std::list< std::string >& list);
  std::istream& operator>>(std::istream& in, Dictionary& dict);
  std::istream& operator>>(std::istream& in, DictSet& set);
  std::istream& operator>>(std::istream& in, DictPairWrapper& pair);
  std::istream& operator>>(std::istream& in, DictSetPairWrapper& pair);
  std::ostream& operator<<(std::ostream& out, const std::list< std::string >& list);
  std::ostream& operator<<(std::ostream& out, const Dictionary& dict);
  std::ostream& operator<<(std::ostream& out, const DictSet& set);
  std::ostream& operator<<(std::ostream& out, const DictPairWrapper& pair);
  std::ostream& operator<<(std::ostream& out, const DictSetPairWrapper& pair);
  std::ostream& operator<<(std::ostream& out, const ContentPairWrapper& pair);
  std::ostream& operator<<(std::ostream& out, const ContentDict& dict);
}
#endif
