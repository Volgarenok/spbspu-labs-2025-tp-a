#include "commands.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <vector>

void loadFromFile(mezentsev::DictionarySet& dicts, std::istream& file)
{
  std::string line;
  mezentsev::Dictionary* currentDict = nullptr;

  while (std::getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }

    if (line.find(' ') == std::string::npos)
    {
      std::string dictName = line;
      dicts[dictName] = mezentsev::Dictionary();
      currentDict = &dicts[dictName];
    }
    else
    {
      if (!currentDict)
      {
        continue;
      }
      std::vector< std::string > tokens = mezentsev::split(line, ' ');
      if (tokens.size() < 2)
      {
        continue;
      }

      std::string engWord = tokens[0];
      mezentsev::Translations translations;
      for (size_t i = 1; i < tokens.size(); ++i)
      {
        translations.insert(tokens[i]);
      }
      (*currentDict)[engWord] = translations;
    }
  }
}

int main(int argc, char* argv[])
{
  using namespace mezentsev;
  DictionarySet dicts;
  if (argc > 1)
  {
    std::ifstream file(argv[1]);
    if (!file)
    {
      std::cout << "FILE ERROR\n";
      return 1;
    }
    loadFromFile(dicts, file);
    file.close();
  }

  std::map< std::string, std::function< void(const std::vector< std::string >&) > > commands;
  using namespace std::placeholders;

  commands.insert(std::make_pair("add", std::bind(addCommand,std::ref(dicts), _1)));
  commands.insert(std::make_pair("remove", std::bind(removeCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("translate", std::bind(translateCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("list", std::bind(listCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("save", std::bind(saveCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("load", std::bind(loadCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("count", std::bind(countCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("clear", std::bind(clearCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("suggest", std::bind(suggestCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("merge", std::bind(mergeCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("diff", std::bind(diffCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("copy", std::bind(copyCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("intersect", std::bind(intersectCommand, std::ref(dicts), _1)));
  commands.insert(std::make_pair("export", std::bind(exportCommand, std::ref(dicts), _1)));

  std::string line;
  while (std::getline(std::cin, line))
  {
    std::vector< std::string > tokens = mezentsev::split(line, ' ');
    if (tokens.empty())
    {
      continue;
    }

    const std::string& command = tokens[0];
    try
    {
      commands.at(command)(tokens);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
