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
