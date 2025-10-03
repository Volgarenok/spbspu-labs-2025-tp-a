#include <string>
#include <map>
#include <functional>
#include <iostream>
#include <limits>
#include "commands.hpp"

using namespace pilugina;

int main()
{
  std::map< std::string, pilugina::dictionary > dicts;
  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > cmds;
  
  {
    using std::placeholders::_1;
    using std::placeholders::_2;
    cmds["CreateDict"] = std::bind(createDict, _1, _2, std::ref(dicts));
    cmds["DeleteDict"] = std::bind(deleteDict, _1, _2, std::ref(dicts));
    cmds["InsertWord"] = std::bind(insertWord, _1, _2, std::ref(dicts));
    cmds["InsertTranslation"] = std::bind(insertTranslation, _1, _2, std::ref(dicts));
    cmds["RemoveWord"] = std::bind(removeWord, _1, _2, std::ref(dicts));
    cmds["RemoveTranslation"] = std::bind(removeTranslation, _1, _2, std::ref(dicts));
    cmds["SearchTranslation"] = std::bind(searchTranslation, _1, _2, std::cref(dicts));
    cmds["PrintSizeDict"] = std::bind(printSizeDict, _1, _2, std::cref(dicts));
    cmds["PrintDict"] = std::bind(printDict, _1, _2, std::cref(dicts));
    cmds["SaveDict"] = std::bind(saveDict, _1, _2, std::cref(dicts));
    cmds["Merge"] = std::bind(mergeDicts, _1, _2, std::ref(dicts));
    cmds["Join"] = std::bind(joinDicts, _1, _2, std::ref(dicts));
    cmds["InnerMerge"] = std::bind(innerMerge, _1, _2, std::ref(dicts));
    cmds["MergeTranslations"] = std::bind(mergeTranslations, _1, _2, std::ref(dicts));
  }
  
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
