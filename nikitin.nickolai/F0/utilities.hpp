#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <map>
#include <regex>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

namespace nikitin
{
  using dictionary = std::map< std::string, std::vector< std::string > >;
  using dictionaries = std::map< std::string, dictionary >;
  using pairType = std::pair< const std::string, std::vector< std::string > >;

  void printTranslations(const std::vector< std::string > &ts, std::ostream &os);
  void printDictionary(const dictionary &d, std::ostream &os);

  struct LineToDict
  {
    dictionary &d;
    bool &bad;

    void operator()(const std::string &line) const;
  };

  struct GetKey
  {
    std::string operator()(const dictionary::value_type &p) const
    {
      return p.first;
    }
  };

  struct BuildEntry
  {
    const dictionary *a;
    const dictionary *b;

    dictionary::value_type operator()(const std::string &key) const;
  };
}
#endif
