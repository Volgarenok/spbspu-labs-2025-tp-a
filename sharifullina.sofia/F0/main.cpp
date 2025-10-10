#include <map>
#include <string>
#include <limits>
#include <fstream>
#include <iostream>
#include <functional>
#include <unordered_map>
#include "commands.hpp"

int main(int argc, char * argv[])
{
  using namespace sharifullina;
  DictCollection dicts;

  if (argc == 2)
  {
    std::string arg = argv[1];
    if (arg == "--help")
    {
      printHelp(std::cout);
      return 0;
    }
    try
    {
      loadFile(arg, dicts);
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["createdict"] = std::bind(createDict, std::ref(std::cin), std::ref(dicts));
  cmds["deletedict"] = std::bind(deleteDict, std::ref(std::cin), std::ref(dicts));
  cmds["listdicts"] = std::bind(listDicts, std::ref(std::cin), std::ref(dicts));
  cmds["addword"] = std::bind(addWord, std::ref(std::cin), std::ref(dicts));
  cmds["addtranslation"] = std::bind(addTranslation, std::ref(std::cin), std::ref(dicts));
  cmds["removetranslation"] = std::bind(removeTranslation, std::ref(std::cin), std::ref(dicts));
  cmds["deleteword"] = std::bind(deleteWord, std::ref(std::cin), std::ref(dicts));
  cmds["findtranslations"] = std::bind(findTranslations, std::ref(std::cin), std::ref(dicts));
  cmds["listwords"] = std::bind(listWords, std::ref(std::cin), std::ref(dicts));
  cmds["merge"] = std::bind(mergeDicts, std::ref(std::cin), std::ref(dicts));
  cmds["findcommon"] = std::bind(findCommon, std::ref(std::cin), std::ref(dicts));
  cmds["save"] = std::bind(saveDict, std::ref(std::cin), std::ref(dicts));
  cmds["load"] = std::bind(loadDict, std::ref(std::cin), std::ref(dicts));
  cmds["stat"] = std::bind(statDict, std::ref(std::cin), std::ref(dicts));
  cmds["subtract"] = std::bind(subtractDicts, std::ref(std::cin), std::ref(dicts));
  cmds["symdiff"] = std::bind(symdiffDicts, std::ref(std::cin), std::ref(dicts));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::exception & e)
    {
      std::cout << e.what() << '\n';
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return 0;
}
