#include <limits>
#include "wrapper.hpp"
#include "shannonFano.hpp"

int main(int argc, char* argv[])
{
  if (argc == 1 && std::string(argv[0]) == "--help")
  {
    duhanina::print_help(std::cout);
  }
  auto commands = duhanina::initialize_commands(std::cin, std::cout);
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)();
    }
    catch (const std::out_of_range&)
    {
      std::cerr << "INVALID COMMAND: " << cmd << "\n";
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
