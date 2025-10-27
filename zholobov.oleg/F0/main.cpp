#include <cstring>
#include <functional>
#include <iostream>
#include <limits>

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
      return 0;
    }
  }

  zholobov::Dictionaries dictionaries;

  std::map< Word, std::function< void(std::istream&, std::ostream&, Dictionaries&) > > keywords;

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

  Word cmd;
  while (std::cin >> cmd) {
    try {
      if (!cmd.empty()) {
        keywords.at(cmd)(std::cin, std::cout, dictionaries);
      }
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    } catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }

  return 0;
}
