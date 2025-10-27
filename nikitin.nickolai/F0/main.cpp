#include <ios>
#include <string>
#include <map>
#include <functional>
#include <iostream>
#include <limits>
#include "commands.hpp"

using namespace nikitin;

int main()
{
  std::map< std::string, nikitin::dictionary > dicts;
  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > cmds;

  {
    using std::placeholders::_1;
    using std::placeholders::_2;
    cmds["create"] = std::bind(createDict, _1, _2, std::ref(dicts));
    cmds["load"] = std::bind(loadCommand, _1, _2, std::ref(dicts));
    cmds["delete_dict"] = std::bind(deleteDict, _1, _2, std::ref(dicts));
    cmds["add_word"] = std::bind(insertWord, _1, _2, std::ref(dicts));
    cmds["remove_word"] = std::bind(removeWord, _1, _2, std::ref(dicts));
    cmds["add_translation"] = std::bind(insertTranslation, _1, _2, std::ref(dicts));
    cmds["remove_translation"] = std::bind(removeTranslation, _1, _2, std::ref(dicts));
    cmds["find_word"] = std::bind(search, _1, _2, std::ref(dicts));
    cmds["intersect"] = std::bind(intersect, _1, _2, std::ref(dicts));
    cmds["subtract"] = std::bind(subtract, _1, _2, std::ref(dicts));
    cmds["union"] = std::bind(unite, _1, _2, std::ref(dicts));
    cmds["save"] = std::bind(saveDict, _1, _2, std::ref(dicts));
    cmds["print"] = std::bind(printDict, _1, _2, std::ref(dicts));
    cmds["size"] = std::bind(printSize, _1, _2, std::ref(dicts));
  }

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)(std::cin, std::cout);
      std::cout << '\n';
    }
    catch (...)
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
