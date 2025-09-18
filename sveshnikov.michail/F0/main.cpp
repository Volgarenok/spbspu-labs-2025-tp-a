#include <functional>
#include <fstream>
#include <limits>
#include "commands.hpp"
#include "programme-options.hpp"

int main(int argc, char *argv[])
{
  using namespace sveshnikov;

  if (argc == 2 && std::string(argv[1]) == "--help")
  {
    print_help_manual(std::cout);
    return 0;
  }

  if (argc == 3 && std::string(argv[1]) == "--check")
  {
    std::ifstream file(argv[2]);
    if (!file)
    {
      std::cout << "<NON-EXISTENT FILE>\n";
      return 1;
    }

    if (check_format(file))
    {
      std::cout << "FILE FORMAT IS CORRECT\n";
    }
    else
    {
      std::cout << "FILE FORMAT IS INCORRECT\n";
    }
    return 0;
  }

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " <population_file> or " << argv[0] << " --help\n";
    return 1;
  }

  populations_t populations;
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cout << "<NON-EXISTENT FILE>\n";
    return 1;
  }

  try
  {
    load(populations, file);
  }
  catch (const std::exception &e)
  {
    std::cout << "ERROR: download populations aborted: " << e.what() << "\n";
    return 1;
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["ADD_POPULATION"] = std::bind(addPopulation, std::ref(populations), std::ref(std::cin));
  cmds["REMOVE_POPULATION"] =
      std::bind(removePopulation, std::ref(populations), std::ref(std::cin));
  cmds["ADD"] = std::bind(add, std::ref(populations), std::ref(std::cin));
  cmds["REMOVE"] = std::bind(removeInds, std::ref(populations), std::ref(std::cin));
  cmds["MUTATE"] = std::bind(mutate, std::ref(populations), std::ref(std::cin));
  cmds["FITNESS"] =
      std::bind(calcFitness, std::cref(populations), std::ref(std::cin), std::ref(std::cout));
  cmds["PEDIGREE"] =
      std::bind(printPedigree, std::cref(populations), std::ref(std::cin), std::ref(std::cout));
  cmds["CROSSOVER"] = std::bind(crossover, std::ref(populations), std::ref(std::cin));
  cmds["LIST"] =
      std::bind(printList, std::cref(populations), std::ref(std::cin), std::ref(std::cout));
  cmds["STATS"] =
      std::bind(printStats, std::cref(populations), std::ref(std::cin), std::ref(std::cout));
  cmds["SAVE"] = std::bind(save, std::cref(populations), std::ref(std::cin), std::ref(std::cout));
  cmds["SELECT"] = std::bind(selectInds, std::ref(populations), std::ref(std::cin));
  cmds["YEARS"] = std::bind(makeOlder, std::ref(populations), std::ref(std::cin));
  cmds["UNITE"] = std::bind(unite, std::ref(populations), std::ref(std::cin));
  cmds["LIST_POPULATION"] = std::bind(listPopulation, std::cref(populations), std::ref(std::cout));

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      cmds.at(command)();
    }
    catch (const std::out_of_range &e)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  return 0;
}
