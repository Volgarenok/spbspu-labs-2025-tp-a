#include "commands.hpp"
#include <iostream>
#include <functional>
#include <map>
#include <limits>

int main()
{
  filonova::DictionarySet dictionaries;
  using namespace std::placeholders;

  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands;
  commands["CREATE"] = std::bind(filonova::createDictionary, std::ref(dictionaries), _1);
  commands["DELETE"] = std::bind(filonova::deleteDictionary, std::ref(dictionaries), _1);
  commands["INSERT"] = std::bind(filonova::insert, std::ref(dictionaries), _1);
  commands["REMOVE"] = std::bind(filonova::remove, std::ref(dictionaries), _1);
  commands["CLEAR"] = std::bind(filonova::clear, std::ref(dictionaries), _1);
  commands["CONTAINS"] = std::bind(filonova::contains, std::ref(dictionaries), _1, _2);
  commands["MERGE"] = std::bind(filonova::merge, std::ref(dictionaries), _1);
  commands["PRINT"] = std::bind(filonova::print, std::ref(dictionaries), _1, _2);
  commands["COUNT"] = std::bind(filonova::count, std::ref(dictionaries), _1, _2);
  commands["TOP"] = std::bind(filonova::top, std::ref(dictionaries), _1, _2);
  commands["UNIQUE"] = std::bind(filonova::unique, std::ref(dictionaries), _1, _2);
  commands["MOSTRARE"] = std::bind(filonova::mostrare, std::ref(dictionaries), _1, _2);
  commands["INTERSECT"] = std::bind(filonova::intersectDictionary, std::ref(dictionaries), _1);
  commands["EXCLUDE"] = std::bind(filonova::excludeDictionary, std::ref(dictionaries), _1);

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
