#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <list>
#include <string>
#include <map>
#include <iosfwd>

namespace holodilov
{
  using MapWords = std::map< std::string, std::list< std::string > >;
  using MapWordsPair = std::pair< std::string, std::list< std::string > >;

  struct Dictionary
  {
    std::string name;
    std::string lang;
    MapWords dict;
  };

  std::ostream& operator<<(std::ostream& out, const Dictionary& dict);
  std::istream& operator>>(std::istream& in, Dictionary& dict);

  using MapDicts = std::map< std::string, Dictionary >;
  using MapDictsPair = std::pair< const std::string, Dictionary >;
}

#endif
