#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <functional>
#include "commands.hpp"

namespace
{
  using list_t = std::list< std::string >;
  using tree_t = std::map< std::string, list_t >;
  using dict_t = std::map< std::string, tree_t >;

  void inputDict(std::istream& in, dict_t& dicts)
  {
    std::string dict_name;
    while (std::getline(in, dict_name))
    {
      if (dict_name.empty())
      {
        continue;
      }

      tree_t current_dict;
      std::string key;
      while (in >> key)
      {
        list_t translations;
        std::string val;
        while (in >> val)
        {
          translations.push_back(val);
          if (in.get() == '\n')
          {
            break;
          }
        }

        current_dict.insert(std::make_pair(key, translations));
        if (in.get() == '\n')
        {
          break;
        }
        else
        {
          in.unget();
        }
      }
      dicts.insert(std::make_pair(dict_name, current_dict));
    }
  }
}

int main(int argc, char* argv[])
{
  using namespace demehin;

  if (argc != 2)
  {
    std::cerr << "ERROR: invalid number of parameters";
    return 1;
  }

  if (std::string(argv[1]) == "--help")
  {
    demehin::printHelp(std::cout);
    return 0;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "<INVALID FILE>\n";
    return 1;
  }
  dict_t dicts;
  try
  {
    inputDict(file, dicts);
  }
  catch (...)
  {
    std::cerr << "<RUNTIME ERROR>\n";
    return 1;
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["createdict"] = std::bind(createDict, std::ref(std::cin), std::ref(dicts));
  cmds["deletedict"] = std::bind(deleteDict, std::ref(std::cin), std::ref(dicts));
  cmds["printdict"] = std::bind(printDict, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["gettranslationeng"] = std::bind(getTranslationEng, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["gettranslationru"] = std::bind(getTranslationRu, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["deleteeng"] = std::bind(deleteEng, std::ref(std::cin), std::ref(dicts));
  cmds["addeng"] = std::bind(addEng, std::ref(std::cin), std::ref(dicts));
  cmds["deleteru"] = std::bind(deleteRu, std::ref(std::cin), std::ref(dicts));
  cmds["addru"] = std::bind(addRu, std::ref(std::cin), std::ref(dicts));
  cmds["union"] = std::bind(makeUnion, std::ref(std::cin), std::ref(dicts));
  cmds["intersect"] = std::bind(makeIntersect, std::ref(std::cin), std::ref(dicts));
  cmds["complement"] = std::bind(makeComplement, std::ref(std::cin), std::ref(dicts));
  cmds["rewritefile"] = std::bind(rewriteFile, std::ref(std::cin), std::cref(dicts));
  cmds["writedicts"] = std::bind(writeToFile, std::ref(std::cin), std::cref(dicts));
  cmds["mostcommon"] = std::bind(printMostCommons, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
