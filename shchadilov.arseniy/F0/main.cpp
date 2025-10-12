#include <iostream>
#include <unordered_map>
#include <limits>
#include <functional>
#include <map>
#include "commands.hpp"
#include "text_processor.hpp"

using namespace shchadilov;

int main(int argc, const char* argv[])
{
  if (argc > 1)
  {
    std::string arg = argv[1];
    if (arg == "--help")
    {
      help(std::cout);
      return 0;
    }
  }

  DictionarySet dictionaries;

  std::map< std::string, std::function< void(std::istream&, std::ostream&) > > commands;
  using namespace std::placeholders;

  commands["create"] = std::bind(create, std::ref(dictionaries), _1);
  commands["remove"] = std::bind(dismiss, std::ref(dictionaries), _1);
  commands["list"] = std::bind(list_dicts, std::cref(dictionaries), _2);
  commands["build"] = std::bind(build, std::ref(dictionaries), _1);
  commands["show"] = std::bind(show, std::cref(dictionaries), _1, _2);
  commands["clear"] = std::bind(clear, std::ref(dictionaries), _1);
  commands["merge"] = std::bind(merge, std::ref(dictionaries), _1, _2);
  commands["intersect"] = std::bind(intersect, std::ref(dictionaries), _1);
  commands["save"] = std::bind(save, std::cref(dictionaries), _1);
  commands["load"] = std::bind(load, std::ref(dictionaries), _1);
  commands["wordfreq"] = std::bind(wordfreq, std::cref(dictionaries), _1, _2);
  commands["topwords"] = std::bind(topwords, std::cref(dictionaries), _1, _2);
  commands["diff"] = std::bind(diff, std::ref(dictionaries), _1, _2);
  commands["unionavg"] = std::bind(unionavg, std::ref(dictionaries), _1, _2);
  commands["compare"] = std::bind(compare, std::cref(dictionaries), _1, _2);
  commands["medianword"] = std::bind(medianword, std::cref(dictionaries), _1, _2);

  std::string commandKey;
  std::cin >> commandKey;
  while (!std::cin.eof())
  {
    try
    {
      commands.at(commandKey)(std::cin, std::cout);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<UNKNOWN COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    catch (const std::exception& e)
    {
      std::cout << e.what();
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin >> commandKey;
  }

  return 0;
}
