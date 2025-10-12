#include <iostream>
#include <fstream>
#include <limits>
#include <cstddef>
#include <map>
#include <functional>
#include "commands.hpp"

int main(const int argc, const char* const* const argv)
{
  using namespace tkach;
  if (argc == 2 && std::string(argv[1]) == "--help")
  {
    help(std::cout);
    return 0;
  }
  else if (argc != 1)
  {
    std::cerr << "<ERROR: INCORRECT INPUT>\n";
    return 1;
  }
  using namespace std::placeholders;
  std::map< std::string, std::map< std::string, std::list< std::string > > > data;
  std::map< std::string, std::function< void() > > cmds;
  cmds["import"] = std::bind(import, std::ref(std::cin), std::ref(data));
  cmds["exportend"] = std::bind(doExportInEnd, std::ref(std::cin), std::cref(data));
  cmds["exportoverwrite"] = std::bind(doExportOverwrite, std::ref(std::cin), std::cref(data));
  cmds["addword"] = std::bind(addWord, std::ref(std::cin), std::ref(data));
  cmds["substractedictionaries"] = std::bind(substructDicts, std::ref(std::cin), std::ref(data));
  cmds["mergewords"] = std::bind(mergeWords, std::ref(std::cin), std::ref(data));
  cmds["copytranslations"] = std::bind(copyTranslations, std::ref(std::cin), std::ref(data));
  cmds["printtranslations"] = std::bind(printTranslations, std::ref(std::cin),std::ref(std::cout), std::cref(data));
  cmds["printengwordswithtraslation"] = std::bind(printEngWordsWithTraslation, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["commonpartdictionaries"] = std::bind(doCommonPartDicts, std::ref(std::cin), std::ref(data));
  cmds["mergedictionaries"] = std::bind(mergeNumberDicts, std::ref(std::cin), std::ref(data));
  cmds["printall"] = std::bind(printAll, std::ref(std::cout), std::cref(data));
  cmds["removeword"] = std::bind(removeWord, std::ref(std::cin), std::ref(data));
  cmds["getcommontranslations"] = std::bind(printCommonTranslations, std::ref(std::cin), std::ref(std::cout), std::cref(data));
  cmds["clear"] = std::bind(clear, std::ref(std::cin), std::ref(data));
  cmds["count"] = std::bind(printCount, std::ref(std::cin), std::ref(std::cout), std::ref(data));
  cmds["removetranslation"] = std::bind(removeTranslation, std::ref(std::cin), std::ref(data));
  cmds["addtranslation"] = std::bind(addTranslation, std::ref(std::cin), std::ref(data));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::out_of_range& e)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << "\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return 0;
}
