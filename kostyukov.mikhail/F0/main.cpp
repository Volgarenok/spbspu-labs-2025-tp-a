#include <exception>
#include <functional>
#include <ios>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>

#include "freqDict.hpp"

int main()
{
  kostyukov::FreqDictManager dict_manager;
  std::map< std::string, std::function< void() > > commands;
  {
    using namespace std::placeholders;
    using namespace kostyukov;
    commands["help"] = std::bind(printHelp, std::ref(std::cout));
    commands["create"] = std::bind(createDict, std::ref(std::cin), std::ref(std::cout), std::ref(dict_manager));
    commands["load"] = std::bind(loadDict, std::ref(std::cin), std::ref(std::cout), std::ref(dict_manager));
    commands["getfreq"] = std::bind(getFreq, std::ref(std::cin), std::ref(std::cout), std::cref(dict_manager));
    commands["top"] = std::bind(top, std::ref(std::cin), std::ref(std::cout), std::cref(dict_manager));
    commands["bottom"] = std::bind(bottom, std::ref(std::cin), std::ref(std::cout), std::cref(dict_manager));
    commands["listdicts"] = std::bind(listDicts, std::ref(std::cout), std::cref(dict_manager));
    commands["delete"] = std::bind(deleteDict, std::ref(std::cin), std::ref(std::cout), std::ref(dict_manager));
    commands["merge"] = std::bind(merge, std::ref(std::cin), std::ref(std::cout), std::ref(dict_manager));
    commands["intersect"] = std::bind(intersect, std::ref(std::cin), std::ref(std::cout), std::ref(dict_manager));
    commands["clear"] = std::bind(clear, std::ref(std::cin), std::ref(std::cout), std::ref(dict_manager));
    commands["finduniq"] = std::bind(findUniq, std::ref(std::cin), std::ref(std::cout), std::cref(dict_manager));
    commands["findsame"] = std::bind(findSame, std::ref(std::cin), std::ref(std::cout), std::cref(dict_manager));
    commands["removebatch"] = std::bind(removeBatch, std::ref(std::cin), std::ref(std::cout), std::ref(dict_manager));
    commands["rangefreq"] = std::bind(rangeFreq, std::ref(std::cin), std::ref(std::cout), std::cref(dict_manager));
    commands["outrangefreq"] = std::bind(outRangeFreq, std::ref(std::cin), std::ref(std::cout), std::cref(dict_manager));
  }
  std::cout << "Write help if you don't know what to do\n";
  std::string command;
  while(!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
      std::cout << '\n';
    }
    catch(const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch(const std::exception& error)
    {
      std::cout << "<ERROR: " << error.what() << ">\n";
    }
    if (std::cin.fail())
    {
      std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
