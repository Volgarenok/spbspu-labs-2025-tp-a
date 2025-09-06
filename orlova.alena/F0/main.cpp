#include <limits>
#include <functional>
#include "commands.h"

template < typename T >
int main(int argc, char** argv)
{
  using namespace orlova;
  std::map< std::string, std::function< void() > > cmds;

  if (argc != 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "<INCORRECT FILE>\n";
    return 1;
  }

  Dictionaries dicts;
  if (argc == 2)
  {
    file >> dicts;
    if (file.fail() && !file.eof())
    {
      throw std::logic_error("<INVALID FILE>");
    }
  }
  cmds["addemptydictionary"] = std::bind(addEmptyDictionary, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["addtranslation"] = std::bind(addTranslation, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["adddictionary"] = std::bind(addDictionary, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["find"] = std::bind(find, std::ref(std::cin), std::ref(std::cout), std::cref(dicts));
  cmds["merge"] = std::bind(merge, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["clear"] = std::bind(clear, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["removeword"] = std::bind(removeWord, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["append"] = std::bind(append, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["dictionarysize"] = std::bind(dictionarySize, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["intersectionofdicts"] = std::bind(intersectionOfDicts, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["delete"] = std::bind(delete, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));;
  cmds["nonrepeatingwords"] = std::bind(nonrepeatingWords, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  cmds["residual"] = std::bind(residual, std::ref(std::cin), std::ref(std::cout), std::ref(dicts));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
      std::cout << '\n';
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cerr << "<INVALID COMMAND>\n";
    }
  }
}
