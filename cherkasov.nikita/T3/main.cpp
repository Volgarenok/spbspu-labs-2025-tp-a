#include <iostream>
#include <string>
#include "geometry.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  try
  {
    std::vector<cherkasov::Polygon> polys = cherkasov::readPolygons(argv[1]);
    std::string cmd;
    while (std::getline(std::cin, cmd))
    {
      if (!cmd.empty())
      {
        cherkasov::processCommand(polys, cmd);
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
