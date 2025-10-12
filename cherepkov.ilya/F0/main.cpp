#include "commands.hpp"
#include "dictionary_types.hpp"

#include <iostream>
#include <functional>
#include <map>
#include <limits>
#include <string>

int main()
{
  cherepkov::DictionarySet dictionaries;
  using namespace std::placeholders;

  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands;

  commands["CREATE"] = std::bind(cherepkov::createDictionary, std::ref(dictionaries), _1);
  commands["DELETE"] = std::bind(cherepkov::deleteDictionary, std::ref(dictionaries), _1);
  commands["INSERT"] = std::bind(cherepkov::insert, std::ref(dictionaries), _1);
  commands["REMOVE"] = std::bind(cherepkov::removeWord, std::ref(dictionaries), _1);
  commands["CLEAR"] = std::bind(cherepkov::clearDictionary, std::ref(dictionaries), _1);
  commands["CONTAINS"] = std::bind(cherepkov::contains, std::ref(dictionaries), _1, _2);
  commands["MERGE"] = std::bind(cherepkov::mergeDicts, std::ref(dictionaries), _1);
  commands["MERGEDICTS"] = std::bind(cherepkov::mergeDicts, std::ref(dictionaries), _1);
  commands["PRINT"] = std::bind(cherepkov::print, std::ref(dictionaries), _1, _2);
  commands["COUNT"] = std::bind(cherepkov::count, std::ref(dictionaries), _1, _2);
  commands["SEARCH"] = std::bind(cherepkov::search, std::ref(dictionaries), _1, _2);
  commands["TOP"] = std::bind(cherepkov::top, std::ref(dictionaries), _1, _2);
  commands["LISTTOP"] = std::bind(cherepkov::listTop, std::ref(dictionaries), _1, _2);
  commands["UNIQUE"] = std::bind(cherepkov::uniqueMultiple, std::ref(dictionaries), _1, _2);
  commands["INTERSECT"] = std::bind(cherepkov::intersectDicts, std::ref(dictionaries), _1);
  commands["DIFF"] = std::bind(cherepkov::diffDicts, std::ref(dictionaries), _1);
  commands["DIFFDICS"] = std::bind(cherepkov::diffDicts, std::ref(dictionaries), _1);
  commands["FILTERBYFREQUENCY"] = std::bind(cherepkov::filterByFrequency, std::ref(dictionaries), _1);
  commands["MEDIAN"] = std::bind(cherepkov::median, std::ref(dictionaries), _1, _2);

  std::string commandKey;
  while (std::cin >> commandKey)
  {
    auto it = commands.find(commandKey);
    if (it != commands.end())
    {
      try
      {
        it->second(std::cin, std::cout);
      }
      catch (const std::exception &e)
      {
        std::cout << e.what() << "\n";
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
