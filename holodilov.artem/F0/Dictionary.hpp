#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <list>
#include <string>
#include <map>

namespace holodilov
{
  struct Dictionary
  {
    std::string name;
    std::string lang;
    std::map< std::string, std::list< std::string > > dict;
  };

  std::ostream& operator<<(std::ostream& out, const Dictionary& dict);
  std::istream& operator>>(std::istream& in, Dictionary& dict);
}

#endif
