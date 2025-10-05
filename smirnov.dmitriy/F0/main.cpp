#include <functional>
#include <limits>
#include <map>
#include "commands.hpp"

namespace smirnov
{
  struct CommandWrapper
  {
    std::function< void(dictionaries&, std::istream&) > inOnly;
    std::function< void(dictionaries&, std::istream&, std::ostream&) > inOut;

    void operator()(dictionaries& dicts, std::istream& in, std::ostream& out) const
    {
      if (inOnly)
      {
        inOnly(dicts, in);
      }
      else if (inOut)
      {
        inOut(dicts, in, out);
      }
      else
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
    }
  };
}

int main(int argc, char** argv)
{
  using namespace smirnov;

  if (argc < 2)
  {
    std::cerr << "Wrong arguments\n";
    return 1;
  }

  std::string inputCmd(argv[1]);
  if (inputCmd == "--help")
  {
    printHelp(std::cout);
  }

  dictionaries dictOfDicts;

  const std::map< std::string, CommandWrapper > commands = {
    { "createemptydict", { createemptydict, nullptr } },
    { "insertword", { insertword, nullptr } },
    { "insertfile", { insertfile, nullptr } },
    { "deleteword", { deleteword, nullptr } },
    { "clear", { clear, nullptr } },
    { "merge", { merge, nullptr } },
    { "intersect", { intersect, nullptr } },
    { "delete", { deleteDict, nullptr } },

    { "getfrequency", { nullptr, getfrequency } },
    { "mostfrequent", { nullptr, mostfrequent } },
    { "printsorted", { nullptr, printsorted } },
    { "printbyfrequency", { nullptr, printbyfrequency } },
    { "size", { nullptr, size } },
    { "topprelativefrequent", { nullptr, topprelativefrequent } },
    { "getrelativefrequency", { nullptr, getrelativefrequency } },
    { "frequencyrangerelative", { nullptr, frequencyrangerelative } },
    { "bottomprelativefrequent", { nullptr, bottomprelativefrequent } },
    { "medianfrequency", { nullptr, medianfrequency } }
  };

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)(dictOfDicts, std::cin, std::cout);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

