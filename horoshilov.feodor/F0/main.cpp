#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <functional>
#include <map>
#include <list>
#include "commands.hpp"

namespace khoroshilov
{
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
  using namespace khoroshilov;

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <dictionary_file>\n";
    return 1;
  }

  if (std::string(argv[1]) == "--help")
  {
    printHelp(std::cout);
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

  using namespace std::placeholders;
  std::map<std::string, std::function<void()>> commands;
  commands["help"] = std::bind(printHelp, std::ref(std::cout));
  commands["load"] = std::bind(load, std::ref(std::cin), std::ref(dicts));
  commands["save"] = std::bind(save, std::ref(std::cin), std::cref(dicts));
  commands["dictcreate"] = std::bind(dictcreate, std::ref(std::cin), std::ref(dicts));
  commands["dictrm"] = std::bind(dictrm, std::ref(std::cin), std::ref(dicts));
  commands["lsdict"] = std::bind(lsdict, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["engtranslate"] = std::bind(engtranslate, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["addenglish"] = std::bind(addenglish, std::ref(std::cin), std::ref(dicts));
  commands["addru"] = std::bind(addru, std::ref(std::cin), std::ref(dicts));
  commands["rmenglish"] = std::bind(rmenglish, std::ref(std::cin), std::ref(dicts));
  commands["rmru"] = std::bind(rmru, std::ref(std::cin), std::ref(dicts));
  commands["maxlen"] = std::bind(maxlen, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["alfrange"] = std::bind(alfrange, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["engcount"] = std::bind(engcount, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["prefix"] = std::bind(prefix, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["clear"] = std::bind(clear, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  commands["complement"] = std::bind(complement, std::ref(std::cin), std::ref(dicts));
  commands["intersect"] = std::bind(intersect, std::ref(std::cin), std::ref(dicts));
  commands["longest"] = std::bind(longest, std::ref(std::cin), std::ref(dicts));
  commands["meancount"] = std::bind(meancount, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  commands["meaningful"] = std::bind(meaningful, std::ref(std::cin), std::ref(dicts));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      auto it = commands.find(command);
      if (it != commands.end())
      {
        it->second();
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
      }
      catch (...)
      {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}

