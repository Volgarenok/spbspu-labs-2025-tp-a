#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <string>
#include <utility>
#include "commands.h"

namespace
{
  void input(std::istream& in, kharlamov::dictionaries& dicts)
  {
    std::string dictName;
    while (in >> dictName)
    {
      kharlamov::dictionary dict;
      std::string eng;
      while (in >> eng)
      {
        std::string rusword;
        in >> rusword;
        dict[eng] = rusword;
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
  using namespace kharlamov;
  dictionaries dicts;
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
  commands.insert(std::make_pair("download", std::bind(doDownload, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("save", std::bind(doSave, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("add", std::bind(doAdd, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("remove", std::bind(doRemove, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("translate", std::bind(doTranslate, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("find", std::bind(doFind, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("search", std::bind(doSearch, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("count", std::bind(doCount, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("list", std::bind(doList, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("clear", std::bind(doClear, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("createDict", std::bind(doCreatedict, std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("listDicts", std::bind(doListdicts, std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("copy", std::bind(doCopy, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("merge", std::bind(doMerge, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("diff", std::bind(doDiff, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("intersect", std::bind(doIntersect, std::ref(std::cin), std::ref(std::cout), std::ref(dicts))));
  commands.insert(std::make_pair("globalSearch", std::bind(doGlobalsearch, std::ref(std::cin), std::ref(std::cout), std::cref(dicts))));
  commands.insert(std::make_pair("globalCount", std::bind(doGlobalcount, std::ref(std::cout), std::cref(dicts))));

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
