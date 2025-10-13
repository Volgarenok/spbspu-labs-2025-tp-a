#include "commands.hpp"
#include <limits>
#include <iterator>
#include <functional>
#include <iostream>
#include <numeric>

int main()
{
  using namespace shak;
  shak::Dictionaries dicts;
  std::map< std::string, std::function< void() > > cmds;
  cmds["create"] = std::bind(createDict, std::ref(dicts), std::ref(std::cin));
  cmds["clear"] = std::bind(clearDict, std::ref(dicts), std::ref(std::cin));
  cmds["delete"] = std::bind(deleteDict, std::ref(dicts), std::ref(std::cin));
  cmds["print"] = std::bind(printDict, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["insert"] = std::bind(insertW, std::ref(dicts), std::ref(std::cin));
  cmds["remove"] = std::bind(removeW, std::ref(dicts), std::ref(std::cin));
  cmds["frequency"] = std::bind(getFreq, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["uniqe"] = std::bind(getUniqe, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["most"] = std::bind(mostPopular, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["equal"] = std::bind(equal, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["compare"] = std::bind(compare, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["intersection"] = std::bind(intersection, std::ref(dicts), std::ref(std::cin));
   std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
