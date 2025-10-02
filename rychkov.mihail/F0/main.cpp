#include <iostream>
#include <exception>
#include "main_processor.hpp"

int main(int argc, char** argv)
{
  try
  {
    rychkov::ParserContext context{std::cin, std::cout, std::cerr};
    rychkov::MainProcessor processor;
    if (!processor.init(context, argc, argv))
    {
      return 0;
    }

    while (rychkov::Parser::parse(context, processor, rychkov::MainProcessor::call_map))
    {}
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
