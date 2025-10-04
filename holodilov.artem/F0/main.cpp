#include <functional>
#include <iostream>
#include <map>
#include <limits>
#include <cstring>
#include "Dictionary.hpp"
#include "consoleOptions.hpp"
#include "commands.hpp"

int main(const int argc, const char** argv)
{
  using namespace holodilov;

  if ((argc == 2) && (std::strcmp(argv[1], "--help") == 0))
  {
    printHelpOption(std::cout);
    return 0;
  }
  if (argc != 1)
  {
    std::cout << "Error: invalid console option\n";
    return 1;
  }

  MapDicts dictionaries;

  std::map< std::string, std::function< void() > > cmds;
  cmds["CreateDict"] = std::bind(createDict, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["ExportDict"] = std::bind(exportDict, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["ImportDict"] = std::bind(importDict, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["ClearDict"] = std::bind(clearDict, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["DeleteDict"] = std::bind(deleteDict, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["AddWord"] = std::bind(addWord, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["DeleteWord"] = std::bind(deleteWord, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["UpdateWord"] = std::bind(updateWord, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["PrintDict"] = std::bind(printDict, std::ref(std::cin), std::ref(std::cout), std::cref(dictionaries));
  cmds["FindWord"] = std::bind(findWord, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["Merge"] = std::bind(merge, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["Intersect"] = std::bind(intersect, std::ref(std::cin), std::ref(std::cout), std::ref(dictionaries));
  cmds["PrintDictNames"] = std::bind(printDictNames, std::ref(std::cout), std::cref(dictionaries));
  cmds["ExportAlphabet"] = std::bind(exportAlphabet, std::ref(std::cin), std::ref(std::cout), std::cref(dictionaries));
  cmds["CheckAlphabet"] = std::bind(checkAlphabet, std::ref(std::cin), std::ref(std::cout), std::cref(dictionaries));

  std::string strCommand;
  while (!(std::cin >> strCommand).eof())
  {
    try
    {
      cmds.at(strCommand)();
      std::cout << '\n';
    }
    catch (const std::out_of_range&)
    {
      std::cout << "Error: invalid command\n";
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << "\n";
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
