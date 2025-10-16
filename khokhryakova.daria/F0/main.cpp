#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <limits>
#include <string>
#include "XrefDictionary.h"

int main(int argc, char * argv[])
{
  khokhryakova::XrefDictionary dict;
  if (argc == 2)
  {
    khokhryakova::loadXrefFromPath(std::cout, argv[1], dict);
  }
  std::unordered_map< std::string, std::function< void() > > cmds;

  cmds["BUILD"] = std::bind(khokhryakova::buildXref, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["PRINT"] = std::bind(khokhryakova::printDict, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["FIND"] = std::bind(khokhryakova::findWord, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["COUNT"] = std::bind(khokhryakova::countWord, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["CONTEXT"] = std::bind(khokhryakova::getContext, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["MERGE"] = std::bind(khokhryakova::mergeXref, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["INTERSECT"] = std::bind(khokhryakova::intersectXref, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["DIFF"] = std::bind(khokhryakova::diffXref, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["SAVE"] = std::bind(khokhryakova::saveXref, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["LOAD"] = std::bind(khokhryakova::loadXref, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["RESTORE"] = std::bind(khokhryakova::restoreText, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["INSERT_WORD"] = std::bind(khokhryakova::insertWord, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["DELETE_WORD"] = std::bind(khokhryakova::deleteWord, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["OUTPUT_WORD"] = std::bind(khokhryakova::outputWord, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["--help"] = std::bind(khokhryakova::showHelp, std::ref(std::cin), std::ref(std::cout));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    if (command == "exit")
    {
      break;
    }
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore((std::numeric_limits< std::streamsize >::max)(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
