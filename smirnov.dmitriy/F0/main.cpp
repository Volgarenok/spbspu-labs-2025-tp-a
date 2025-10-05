#include <functional>
#include <limits>
#include <map>
#include "commands.hpp"

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

  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > commands;

  commands["createemptydict"] = std::bind(createemptydict, std::ref(dictOfDicts), std::placeholders::_1);
  commands["insertword"] = std::bind(insertword, std::ref(dictOfDicts), std::placeholders::_1);
  commands["insertfile"] = std::bind(insertfile, std::ref(dictOfDicts), std::placeholders::_1);
  commands["deleteword"] = std::bind(deleteword, std::ref(dictOfDicts), std::placeholders::_1);
  commands["clear"] = std::bind(clear, std::ref(dictOfDicts), std::placeholders::_1);
  commands["merge"] = std::bind(merge, std::ref(dictOfDicts), std::placeholders::_1);
  commands["intersect"] = std::bind(intersect, std::ref(dictOfDicts), std::placeholders::_1);
  commands["delete"] = std::bind(deleteDict, std::ref(dictOfDicts), std::placeholders::_1);

  commands["getfrequency"] = std::bind(getfrequency, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));
  commands["mostfrequent"] = std::bind(mostfrequent, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));
  commands["printsorted"] = std::bind(printsorted, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));
  commands["printbyfrequency"] = std::bind(printbyfrequency, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));
  commands["size"] = std::bind(size, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));
  commands["topprelativefrequent"] = std::bind(topprelativefrequent, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));
  commands["getrelativefrequency"] = std::bind(getrelativefrequency, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));
  commands["frequencyrangerelative"] = std::bind(frequencyrangerelative, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));
  commands["bottomprelativefrequent"] = std::bind(bottomprelativefrequent, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));
  commands["medianfrequency"] = std::bind(medianfrequency, std::ref(dictOfDicts), std::placeholders::_1, std::ref(std::cout));

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

