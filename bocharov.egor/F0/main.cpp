#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <functional>
#include <limits>
#include <cctype>

namespace
{
  void inputDict(std::istream & in, bocharov::dict_dict_t & dicts)
  {
    std::string line;
    while (std::getline(in, line))
    {
      if (line.empty())
      {
        continue;
      }
      std::string dictName = line;

      bocharov::dict_t dict;
      while (std::getline(in, line) && !line.empty())
      {
        std::vector<std::string> words;
        std::string word;
        for (char c : line)
        {
          if (std::isspace(static_cast<unsigned char>(c)))
          {
            if (!word.empty())
            {
              words.push_back(word);
              word.clear();
            }
          }
          else
          {
              word += c;
          }
        }
        if (!word.empty())
        {
          words.push_back(word);
        }

        if (words.empty())
        {
          continue;
        }
        std::string sln = words[0];
        std::vector<std::string> translations(words.begin() + 1, words.end());
        dict[sln] = translations;
      }
      dicts[dictName] = dict;
    }
  }
}

int main(int argc, char * argv[])
{
  using namespace bocharov;
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  dict_dict_t dicts;
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "<INVALID FILE>\n";
    return 1;
  }

  try
  {
    inputDict(file, dicts);
  }
  catch (const std::exception & e)
  {
    std::cerr << "<INVALID FILE>\n";
    return 1;
  }

  std::map<std::string, std::function<void()>> commands;
  commands["createdict"] = std::bind(createDict, std::ref(std::cin), std::ref(dicts));
  commands["deletedict"] = std::bind(deleteDict, std::ref(std::cin), std::ref(dicts));
  commands["printdict"] = std::bind(printDict, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["gettranslationsln"] = std::bind(getTranslationSln, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["gettranslationru"] = std::bind(getTranslationRu, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["deletesln"] = std::bind(deleteSln, std::ref(std::cin), std::ref(dicts));
  commands["addsln"] = std::bind(addSln, std::ref(std::cin), std::ref(dicts));
  commands["deleteru"] = std::bind(deleteRu, std::ref(std::cin), std::ref(dicts));
  commands["addru"] = std::bind(addRu, std::ref(std::cin), std::ref(dicts));
  commands["union"] = std::bind(makeUnion, std::ref(std::cin), std::ref(dicts));
  commands["rewritefile"] = std::bind(rewriteFile, std::ref(std::cin), std::cref(dicts));
  commands["writedicts"] = std::bind(writeDicts, std::ref(std::cin), std::cref(dicts));
  commands["mostcommon"] = std::bind(printMostCommon, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["rare"] = std::bind(rare, std::ref(std::cin), std::ref(dicts));
  commands["help"] = std::bind(printHelp, std::ref(std::cout));

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
    }
    catch (const std::exception & e)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}

