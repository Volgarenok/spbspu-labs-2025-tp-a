#include <cstring>
#include <functional>
#include <iostream>

#include "commands.hpp"
#include "utils.hpp"

int main(int argc, char* argv[])
{
  using namespace zholobov;
  if (argc > 2) {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  if (argc == 2) {
    if (std::strcmp(argv[1], "--help") == 0) {
      printHelp(std::cout);
      std::cout << '\n';
      return 0;
    }
  }

  zholobov::Dictionaries dictionaries;

  std::map< Word, std::function< void(zholobov::Dictionaries&, const std::vector< Word >&) > > keywords;

  keywords["dict-create"] = cmdDictCreate;
  keywords["dict-remove"] = cmdDictRemove;
  keywords["dict-import"] = cmdDictImport;
  keywords["dict-export"] = cmdDictExport;
  keywords["dict-count"] = cmdDictCount;
  keywords["dict-list"] = cmdDictList;
  keywords["dict-print-words"] = cmdDictPrintWords;
  keywords["dict-print-translations"] = cmdDictPrintTranslations;
  keywords["dict-clear"] = cmdDictClear;
  keywords["add-word"] = cmdAddWord;
  keywords["remove-word"] = cmdRemoveWord;
  keywords["count-words"] = cmdCountWords;
  keywords["add-translation"] = cmdAddTranslation;
  keywords["remove-translation"] = cmdRemoveTranslation;
  keywords["change-word"] = cmdChangeWord;
  keywords["translate-word"] = cmdTranslateWord;
  keywords["union"] = cmdUnion;
  keywords["intersect"] = cmdIntersect;
  keywords["rare"] = cmdRare;

  std::string line;
  while (std::getline(std::cin, line)) {
    std::vector< std::string > tokens = splitTokens(line);
    if (tokens.empty()) {
      continue;
    }

    const std::string& cmd = tokens[0];
    try {
      keywords.at(cmd)(dictionaries, tokens);
    } catch (const InvalidParams&) {
      std::cout << "<ERROR IN PARAMETERS>\n";
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
