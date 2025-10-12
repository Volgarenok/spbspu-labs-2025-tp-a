#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <string>
#include <utility>
#include <iterator>
#include <limits>
#include "commands.hpp"
namespace
{
  void input(std::istream& in, kiselev::Dicts& dicts)
  {
    std::string dictName;
    while (in >> dictName)
    {
      kiselev::Dict dict;
      std::string eng;
      while (in >> eng)
      {
        std::vector< std::string > ruswords;
        std::string rusword;
        while (in >> rusword)
        {
          ruswords.push_back(rusword);
          if (in.get() == '\n')
          {
            break;
          }
        }
        dict[eng] = ruswords;
        if (in.get() == '\n')
        {
          break;
        }
        else
        {
          in.unget();
        }
      }
      dicts[dictName] = dict;
    }
  }
}
int main(int argc, char** argv)
{
  using namespace kiselev;
  Dicts dicts;
  if (argc > 1)
  {
    std::ifstream file(argv[1]);
    if (!file)
    {
      std::cout << "FILE ERROR\n";
      return 1;
    }
    input(file, dicts);
    file.close();
  }
  std::map< std::string, std::function< void() > > commands;
  commands.insert(std::make_pair("newdict", std::bind(doNewDict, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("deletedict", std::bind(doDeleteDict, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("addelement", std::bind(doAddElement, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("deleteelement", std::bind(doDeleteElement, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("listdict", std::bind(doListDict, std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("printdict", std::bind(doPrintDict, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("translateword", std::bind(doTranslateWord, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("uniondict", std::bind(doUnionDict, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("savedict", std::bind(doSaveDict, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("countword", std::bind(doCountWord, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("searchletter", std::bind(doSearchLetter, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("loaddict", std::bind(doLoadDict, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("cleardict", std::bind(doClearDict, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("intersectdict", std::bind(doIntersectDict, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("complementdict", std::bind(doComplementDict, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 1;
}
