#include <iostream>
#include <fstream>
#include <clocale>
#include <cstring>
#include <limits>
#include "commands.hpp"
#include "dict-utils.hpp"

int main(int argc, char** argv)
{
  using namespace alymova;
  using WordSet = std::list< std::string >;
  using Dictionary = std::map< std::string, WordSet >;
  using DictSet = std::map< std::string, Dictionary >;
  using CommandSet = std::map< std::string, std::function< void() > >;
  std::setlocale(LC_CTYPE, "rus");

  if (argc > 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  std::ifstream file;
  if (argc == 2)
  {
    if (std::strcmp(argv[1], "--help") == 0)
    {
      printHelp(std::cout);
      std::cout << '\n';
      return 0;
    }
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "<INCORRECT FILE>\n";
      return 1;
    }
  }

  try
  {
    DictSet dataset;
    if (argc == 2)
    {
      file >> dataset;
      if (file.fail() && !file.eof())
      {
        throw std::logic_error("<INVALID DICTIONARIES DESCRIPTION>");
      }
    }
    CommandSet commands;
    commands["create"] = std::bind(create, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["dictionary_list"] = std::bind(dicts, std::ref(std::cout), std::cref(dataset));
    commands["size"] = std::bind(size, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["remove_dictionary"] = std::bind(removeDict, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["add_word"] = std::bind(addWord, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["fix_word"] = std::bind(fixWord, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["find_word"] = std::bind(findWord, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["contain"] = std::bind(containSubword, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["remove_word"] = std::bind(removeWord, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["add_translate"] = std::bind(addTranslate, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));

    auto short_name1 = std::bind(findEnglishEquivalent, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["find_english_equivalent"] = short_name1;

    auto short_name2 = std::bind(removeTranslate, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["remove_translate"] = short_name2;

    commands["print_content"] = std::bind(printContent, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["translate"] = std::bind(translate, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["union"] = std::bind(unionDicts, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["intersect"] = std::bind(intersectDicts, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["day_word"] = std::bind(printDayWord, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));

    std::string command;
    while (!(std::cin >> command).eof())
    {
      try
      {
        commands.at(command)();
        std::cout << '\n';
      }
      catch (const std::exception& e)
      {
        if (std::cin.fail())
        {
          std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
        }
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    std::ofstream file_save;
    if (argc == 2)
    {
      file_save.open(argv[1]);
    }
    else
    {
      file_save.open("dictionaries.txt");
    }
    file_save << dataset << '\n';
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}
