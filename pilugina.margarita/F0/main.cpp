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
    {"CreateDict", std::bind(createDict, _1, _2, std::ref(dicts))},
    {"DeleteDict", std::bind(deleteDict, _1, _2, std::ref(dicts))},
    {"InsertWord", std::bind(insertWord, _1, _2, std::ref(dicts))},
    {"InsertTranslation", std::bind(insertTranslation, _1, _2, std::ref(dicts))},
    {"RemoveWord", std::bind(removeWord, _1, _2, std::ref(dicts))},
    {"RemoveTranslation", std::bind(removeTranslation, _1, _2, std::ref(dicts))},
    {"SearchTranslation", std::bind(searchTranslation, _1, _2, std::cref(dicts))},
    {"PrintSizeDict", std::bind(printSizeDict, _1, _2, std::cref(dicts))},
    {"PrintDict", std::bind(printDict, _1, _2, std::cref(dicts))},
    {"SaveDict", std::bind(saveDict, _1, _2, std::cref(dicts))},
    {"Merge", std::bind(mergeDicts, _1, _2, std::ref(dicts))},
    {"Join", std::bind(joinDicts, _1, _2, std::ref(dicts))},
    {"InnerMerge", std::bind(innerMerge, _1, _2, std::ref(dicts))},
    {"MergeTranslations", std::bind(mergeTranslations, _1, _2, std::ref(dicts))}
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
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  return 0;
}
