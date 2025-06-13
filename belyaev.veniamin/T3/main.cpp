#include "command-handlers.hpp"
#include "shapes.hpp"
#include <algorithm>
#include <fstream>
#include <iterator>

int main(int argc, char* argv[])
{
  using namespace belyaev;
  using namespace std::placeholders;

  if (argc != 2)
  {
    std::cerr << "<INVALID NUMBER OF ARGUMENTS>\n";
    return 1;
  }
  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "<INVALID FILE>\n";
    return 1;
  }
  std::vector<Polygon> data;

  using PolyIStream = std::istream_iterator<Polygon>;
  while (!file.eof())
  {
    if (file.fail())
    {
      file.clear(file.rdstate() ^ std::ios_base::failbit);
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::copy(PolyIStream{file}, PolyIStream{}, std::back_inserter(data));
  }

  std::map<std::string, std::function<void(std::istream&, std::ostream&)>> cmds;
  const std::string minCommand = "min";
  const std::string maxCommand = "max";
  cmds["AREA"] = std::bind(area, std::cref(data), _1, _2);
  cmds["MAX"] = std::bind(minMax, std::cref(data), _1, _2, std::cref(maxCommand));
  cmds["MIN"] = std::bind(minMax, std::cref(data), _1, _2, std::cref(minCommand));
  cmds["COUNT"] = std::bind(count, std::cref(data), _1, _2);
  cmds["RMECHO"] = std::bind(rmecho, std::ref(data), _1, _2);
  cmds["INFRAME"] = std::bind(inframe, std::cref(data), _1, _2);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)(std::cin, std::cout);
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
