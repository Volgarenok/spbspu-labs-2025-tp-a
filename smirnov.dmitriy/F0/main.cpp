#include <functional>
#include <limits>
#include <map>
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace smirnov;
  using namespace std::placeholders;

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

  const std::map<std::string, std::function<void(std::istream&, std::ostream&)>> commands = {
    { "createemptydict", std::bind(createemptydict, std::ref(dictOfDicts), _1) },
    { "insertword", std::bind(insertword, std::ref(dictOfDicts), _1) },
    { "insertfile", std::bind(insertfile, std::ref(dictOfDicts), _1) },
    { "deleteword", std::bind(deleteword, std::ref(dictOfDicts), _1) },
    { "clear", std::bind(clear, std::ref(dictOfDicts), _1) },
    { "merge", std::bind(merge, std::ref(dictOfDicts), _1) },
    { "intersect", std::bind(intersect, std::ref(dictOfDicts), _1) },
    { "delete", std::bind(deleteDict, std::ref(dictOfDicts), _1) },
    { "getfrequency", std::bind(getfrequency, std::ref(dictOfDicts), _1, std::ref(std::cout)) },
    { "mostfrequent", std::bind(mostfrequent, std::ref(dictOfDicts), _1, std::ref(std::cout)) },
    { "printsorted", std::bind(printsorted, std::ref(dictOfDicts), _1, std::ref(std::cout)) },
    { "printbyfrequency", std::bind(printbyfrequency, std::ref(dictOfDicts), _1, std::ref(std::cout)) },
    { "size", std::bind(size, std::ref(dictOfDicts), _1, std::ref(std::cout)) },
    { "topprelativefrequent", std::bind(topprelativefrequent, std::ref(dictOfDicts), _1, std::ref(std::cout)) },
    { "getrelativefrequency", std::bind(getrelativefrequency, std::ref(dictOfDicts), _1, std::ref(std::cout)) },
    { "frequencyrangerelative", std::bind(frequencyrangerelative, std::ref(dictOfDicts), _1, std::ref(std::cout)) },
    { "bottomprelativefrequent", std::bind(bottomprelativefrequent, std::ref(dictOfDicts), _1, std::ref(std::cout)) },
    { "medianfrequency", std::bind(medianfrequency, std::ref(dictOfDicts), _1, std::ref(std::cout)) }
  };

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      auto it = commands.find(cmd);
      if (it == commands.end())
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      it->second(std::cin, std::cout);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}

