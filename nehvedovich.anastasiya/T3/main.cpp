#include "commands.hpp"

#include <functional>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <fstream>
#include <vector>
#include <limits>
#include <map>

int main(int argc, char* argv[])
{
  using namespace nehvedovich;
  if (argc != 2)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }
  else
  {
    std::ifstream in(argv[1]);
    if (!in.is_open())
    {
      std::cerr << "<INVALID COMMAND>\n";
      return 1;
    }
    std::vector< Polygon > polygons;
    using input_it_t = std::istream_iterator< Polygon >;
    while (!in.eof())
    {
      std::copy(input_it_t{ in }, input_it_t{}, std::back_inserter(polygons));
      if (!in)
      {
        in.clear();
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }

    std::map< std::string, std::function< void(std::istream&, std::ostream&) > > cmds;
    {
      using namespace std::placeholders;
      cmds["AREA"] = std::bind(areaCommand, _1, _2, std::cref(polygons));
      cmds["MAX"] = std::bind(maxCommand, _1, _2, std::cref(polygons));
      cmds["MIN"] = std::bind(minCommand, _1, _2, std::cref(polygons));
      cmds["COUNT"] = std::bind(countCommand, _1, _2, std::cref(polygons));
      cmds["INFRAME"] = std::bind(inFrameCommand, _1, _2, std::cref(polygons));
      cmds["LESSAREA"] = std::bind(lessAreaCommand, _1, _2, std::cref(polygons));

      std::string command;
      while (std::cin >> command)
      {
        try
        {
          cmds.at(command)(std::cin, std::cout);
          std::cout << '\n';
        }
        catch (...)
        {
          std::cout << "<INVALID COMMAND>" << '\n';
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }
}
