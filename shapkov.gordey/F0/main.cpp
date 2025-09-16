#include <iostream>
#include <unordered_map>
#include <functional>
#include <limits>
#include "FrequencyDictionary.hpp"

int main(int argc, char* argv[])
{
  shapkov::FrequencyDictionary dict;
  if (argc == 2)
  {
      shapkov::load(std::cout, argv[1], dict);
  }
  std::unordered_map< std::string, std::function< void() > > cmds;
  cmds["ANALYZETEXT"] = std::bind(shapkov::analyzeText, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["PRINT"] = std::bind(shapkov::print, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["WORDINFO"] = std::bind(shapkov::wordInfo, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["ANAGRAMS"] = std::bind(shapkov::anagrams, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["SIMILARFREQ"] = std::bind(shapkov::similarFrequency, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["ENTROPY"] = std::bind(shapkov::entropy, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["TOTALWORDS"] = std::bind(shapkov::totalWords, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["PALINDROMES"] = std::bind(shapkov::palindromes, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["TOP"] = std::bind(shapkov::top, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["PATTERN"] = std::bind(shapkov::showWithPattern, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["COMPARE"] = std::bind(shapkov::compare, std::ref(std::cin), std::ref(std::cout), std::cref(dict));
  cmds["INTERSECT"] = std::bind(shapkov::intersect, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["DIFF"] = std::bind(shapkov::diff, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["MERGE"] = std::bind(shapkov::merge, std::ref(std::cin), std::ref(std::cout), std::ref(dict));
  cmds["SAVE"] = std::bind(shapkov::save, std::ref(std::cin), std::ref(std::cout), std::cref(dict));

  std::string command;
  while (!(std::cin >> command).eof())
  {
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
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
      }
  }
  return 0;
}
