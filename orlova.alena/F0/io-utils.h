#ifndef IO_UTILS_H
#define IO_UTILS_H
#include <list>
#include <map>
#include <istream>
#include <ostream>

namespace orlova
{
  using Translations = std::list< std::string >;
  using Dictionary = std::map< std::string, Translations >;
  using Dictionaries = std::map< std::string, Dictionary >;

  struct DictPairWrapper
  {
    std::pair< std::string, std::list< std::string > > p;
  };

  std::istream& operator>>(std::istream&, std::list< std::string >&);
  std::ostream& operator<<(std::ostream&, const Dictionary&);
  std::ostream& operator<<(std::ostream&, const std::list< std::string >&);
  std::ostream& operator<<(std::ostream&, const DictPairWrapper&);
}

#endif
