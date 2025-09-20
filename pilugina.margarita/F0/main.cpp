#include <string>
#include <map>
#include <functional>
#include <iostream>
#include <limits>
#include "commands.hpp"

using namespace pilugina;

int main()
{
  using namespace std::placeholders;
  std::map< std::string, pilugina::dictionary > dicts;
  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > cmds
  {
    {"CreateDict", std::bind(createDict, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))},
    {"DeleteDict", std::bind(deleteDict, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))},
    {"InsertWord", std::bind(insertWord, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))},
    {"InsertTranslation", std::bind(insertTranslation, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))},
    {"RemoveWord", std::bind(removeWord, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))},
    {"RemoveTranslation", std::bind(removeTranslation, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))},
    {"SearchTranslation", std::bind(searchTranslation, std::placeholders::_1, std::placeholders::_2, std::cref(dicts))},
    {"PrintSizeDict", std::bind(printSizeDict, std::placeholders::_1, std::placeholders::_2, std::cref(dicts))},
    {"PrintDict", std::bind(printDict, std::placeholders::_1, std::placeholders::_2, std::cref(dicts))},
    {"SaveDict", std::bind(saveDict, std::placeholders::_1, std::placeholders::_2, std::cref(dicts))},
    {"Merge", std::bind(mergeDicts, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))},
    {"Join", std::bind(joinDicts, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))},
    {"InnerMerge", std::bind(innerMerge, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))},
    {"MergeTranslations", std::bind(mergeTranslations, std::placeholders::_1, std::placeholders::_2, std::ref(dicts))}
  };

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)(std::cin, std::cout);
      std::cout << '\n';
    }
    catch (std::exception &)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::ios::goodbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
