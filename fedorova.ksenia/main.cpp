#include <iostream>
#include <fstream>
#include <clocale>
#include <cstring>
#include <limits>
#include "commands.h"
#include "Dictionary.h"

int main(int argc, char** argv)
{
  using namespace fedorova;
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
      print_help(std::cout);
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
    commands["create_dict"] = std::bind(create_dict, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["list_dicts"] = std::bind(list_dicts, std::ref(std::cout), std::cref(dataset));
    commands["size_of_dict"] = std::bind(size_of_dict, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["delete_dictionary"] = std::bind(delete_dict, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["add_word"] = std::bind(add_word, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["change_translation"] = std::bind(change_translation, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["translate"] = std::bind(translate, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["contains"] = std::bind(contains, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["delete_word"] = std::bind(delete_word, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["add_translate"] = std::bind(add_translate, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));

    auto short_name1 = std::bind(delete_translate, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["delete_translate"] = short_name1;

    commands["see_dict"] = std::bind(see_dict, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["translate_in_dicts"] = std::bind(translate_in_dicts, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["merge"] = std::bind(merge, std::ref(std::cin), std::ref(std::cout), std::ref(dataset));
    commands["start_with"] = std::bind(start_with, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["export_range"] = std::bind(export_range, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));
    commands["export_no_range"] = std::bind(export_no_range, std::ref(std::cin), std::ref(std::cout), std::cref(dataset));

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
