#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <functional>
#include <iostream>
#include "polygon.hpp"
#include "commands.hpp"

int main(int argc, char * argv[])
{
  using namespace guseynov;
  if (argc != 2)
  {
    std::cerr << "Error: filename required\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Error: can't open file\n";
    return 1;
  }
  std::vector< Polygon > polygons;
  while (!file.eof())
  {
    Polygon poly;
    if (file >> poly)
    {
      polygons.push_back(poly);
    }
    else
    {
      file.clear(file.rdstate() & ~std::ios::failbit);
      file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::map< std::string, std::function< void() > > cmds;
  cmds["AREA"] = std::bind(printArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MAX"] = std::bind(printMax, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["MIN"] = std::bind(printMin, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["COUNT"] = std::bind(printCount, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["INFRAME"] = std::bind(printInFrame, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  cmds["LESSAREA"] = std::bind(printLessArea, std::ref(std::cin), std::ref(std::cout), std::cref(polygons));
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)();
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::logic_error& e)
    {
      std::cout << e.what() << "\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
