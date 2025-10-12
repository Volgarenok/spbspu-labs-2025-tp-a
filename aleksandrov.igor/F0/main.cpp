#include <functional>
#include <iostream>
#include "sequence-commands.hpp"

int main(int argc, char* argv[])
{
  using namespace aleksandrov;

  if (argc > 2)
  {
    std::cerr << "ERROR: Too many arguments!\n";
    return 1;
  }
  else if (argc == 2)
  {
    std::string param(argv[1]);
    if (param != "--help")
    {
      std::cerr << "ERROR: Incorrect argument!\n";
      return 1;
    }
    else
    {
      printHelp(std::cout);
      return 0;
    }
  }

  Sequences sequences;
  try
  {
    processCommands(std::cin, std::cout, sequences);
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Out of memory!\n";
    return 2;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
}

