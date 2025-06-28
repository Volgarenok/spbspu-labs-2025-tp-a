#include <fstream>
#include <iterator>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char * argv[])
{
  using namespace smirnov;
  if (argc != 2)
  {
    std::cerr << "Error: filename required\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: can't open file\n";
    return 2;
  }
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    using input_it = std::istream_iterator< Polygon >;
    std::copy(input_it{file}, input_it{}, std::back_inserter(polygons));
    if (file.fail() && !file.eof())
    {
      file.clear();
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > cmds;
  using namespace std::placeholders;
  cmds["AREA"] = std::bind(areaCommand, std::cref(polygons), _1, _2);
  cmds["MAX"] = std::bind(maxCommand, std::cref(polygons), _1, _2);
  cmds["MIN"] = std::bind(minCommand, std::cref(polygons), _1, _2);
  cmds["COUNT"] = std::bind(countCommand, std::cref(polygons), _1, _2);
  cmds["INFRAME"] = std::bind(inframeCommand, std::cref(polygons), _1, _2);
  cmds["MAXSEQ"] = std::bind(maxseqCommand, std::cref(polygons), _1, _2);
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout);
    }
    catch (const std::logic_error &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}

