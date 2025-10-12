#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <parser.hpp>
#include "processors.hpp"

int main(int argc, char** argv)
{
  std::cout << std::fixed << std::setprecision(1);
  try
  {
    rychkov::MainProcessor processor{argc, argv};

    rychkov::ParserContext context{std::cin, std::cout, std::cerr};
    while (rychkov::Parser::parse(context, processor, rychkov::MainProcessor::call_map))
    {}
  }
  catch (const std::invalid_argument& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
