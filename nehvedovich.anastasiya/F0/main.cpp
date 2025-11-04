#include "commands.hpp"
#include <iostream>
#include <functional>
#include <map>
#include <limits>

int main()
{
  nehvedovich::DictionarySet dictionaries;
  using namespace std::placeholders;

  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > cmd;

  cmd["CREATE"] = std::bind(nehvedovich::createDict, _1, std::ref(dictionaries));
  cmd["DELETE"] = std::bind(nehvedovich::deleteDict, _1, std::ref(dictionaries));
  cmd["ADD"] = std::bind(nehvedovich::add, _1, std::ref(dictionaries));
  cmd["REMOVE"] = std::bind(nehvedovich::remove, _1, std::ref(dictionaries));
  cmd["TRANSLATE"] = std::bind(nehvedovich::translate, _1, std::cref(dictionaries), _2);
  cmd["CONTAINS"] = std::bind(nehvedovich::contains, _1, std::cref(dictionaries), _2);
  cmd["LIST"] = std::bind(nehvedovich::list, _1, std::cref(dictionaries), _2);
  cmd["MERGE"] = std::bind(nehvedovich::merge, _1, std::ref(dictionaries));
  cmd["QUANTITY"] = std::bind(nehvedovich::quantity, _1, std::cref(dictionaries), _2);
  cmd["REVERSE_TRANSLATE"] = std::bind(nehvedovich::reverseTranslate, _1, std::cref(dictionaries), _2);
  cmd["EDIT"] = std::bind(nehvedovich::edit, _1, std::ref(dictionaries));
  cmd["MORE_TRANSLATIONS"] = std::bind(nehvedovich::moreTranslations, _1, std::cref(dictionaries), _2);

  std::string cmdKey;

  while (std::cin >> cmdKey)
  {
    auto it = cmd.find(cmdKey);
    if (it != cmd.end())
    {
      try
      {
        it->second(std::cin, std::cout);
      }
      catch (const std::exception& e)
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
