#include <map>
#include <limits>
#include <functional>
#include <iostream>
#include "commands.hpp"

int main()
{
  using dictionary_system = std::unordered_map< std::string, std::unordered_map< std::string, std::set < std::string > > >;
  using namespace kushekbaev;
  dictionary_system curr_ds;
  std::map< std::string, std::function< void() > > commands;
  commands["insert"] = std::bind(insert, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["insert_without_translation"] = std::bind(insert_without_translation,
    std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["print"] = std::bind(print, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["remove"] = std::bind(kushekbaev::remove, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["save"] = std::bind(save, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["import_dictionary"] = std::bind(import_dictionary,
    std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["search"] = std::bind(search, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["clear_dictionary"] = std::bind(clear_dictionary,
    std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["reverse_search"] = std::bind(reverse_search, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["remove_translation"] = std::bind(remove_translation,
    std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["remove_translation_at_all"] = std::bind(remove_translation_at_all,
    std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["delete_all_translations"] = std::bind(delete_all_translations,
    std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["prefix_search"] = std::bind(prefix_search, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["no_prefix_search"] = std::bind(no_prefix_search,
    std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["suffix_search"] = std::bind(suffix_search, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["no_suffix_search"] = std::bind(no_suffix_search,
    std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["merge"] = std::bind(merge, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["split"] = std::bind(split, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["find_words_without_translation"] = std::bind(find_words_without_translations,
    std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["complement"] = std::bind(complement, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["intersect"] = std::bind(intersect, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));
  commands["unification"] = std::bind(unification, std::ref(std::cout), std::ref(std::cin), std::ref(curr_ds));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)();
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << "\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
