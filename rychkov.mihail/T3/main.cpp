#include <iostream>
#include <iomanip>
#include <algorithm>
#include "parser.hpp"
#include "processors.hpp"

int main(int argc, char** argv)
{
  std::cout << std::fixed << std::setprecision(1);
  using processor = rychkov::MainProcessor;
  using parser_type = rychkov::Parser< processor >;
  parser_type::map_type call_map = {
        {"AREA", &processor::area},
        {"MAX", &processor::max},
        {"MIN", &processor::min},
        {"COUNT", &processor::count},
        {"RMECHO", &processor::remove_repeates},
        {"RECTS", &processor::rectangles}
      };
  parser_type parser{{std::cin, std::cout, std::cerr}, {}, std::move(call_map)};
  if (!parser.processor.init(parser.context, argc, argv))
  {
    return 1;
  }

  while (parser.run())
  {}
}
