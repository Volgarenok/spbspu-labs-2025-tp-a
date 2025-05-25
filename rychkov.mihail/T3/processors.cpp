#include "processors.hpp"

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

bool rychkov::MainProcessor::init(ParserContext& context, int argc, char** argv)
{
  if (argc != 2)
  {
    context.err << "wrong arguments count\n";
    return false;
  }
  std::ifstream file{argv[1]};
  if (!file)
  {
    context.err << "failed to open file \"" << argv[1] << "\"\n";
    return false;
  }
  while (file.good())
  {
    using Iter = std::istream_iterator< Polygon >;
    std::copy(Iter{file}, Iter{}, std::back_inserter(polygons_));
    file.clear(file.rdstate() & ~std::ios::failbit);
  }
  return true;
}

size_t rychkov::parse_as_count(ParserContext& context)
{
  std::istream::sentry sentry{context.in};
  if (!sentry)
  {
    return ~0ULL;
  }
  size_t result = ~0ULL;
  if ((context.in.peek() != '-') && (context.in >> result))
  {
    return result;
  }
  context.in.clear(context.in.rdstate() & ~std::ios::failbit);
  return ~0ULL;
}
bool rychkov::MainProcessor::area(ParserContext& context)
{
  using processor = rychkov::AreaProcessor;
  using parser_type = rychkov::Parser< processor >;
  static parser_type::map_type call_map = {
        {"EVEN", &processor::even},
        {"ODD", &processor::odd},
        {"MEAN", &processor::mean}
      };
  static parser_type parser{context, {polygons_}, std::move(call_map), &processor::count};
  parser.run();
  return true;
}
bool rychkov::MainProcessor::max(ParserContext& context)
{
  using processor = rychkov::MaxProcessor;
  using parser_type = rychkov::Parser< processor >;
  static parser_type::map_type call_map = {
        {"AREA", &processor::area},
        {"VERTEXES", &processor::count}
      };
  static parser_type parser{context, {polygons_}, std::move(call_map)};
  parser.run();
  return true;
}
bool rychkov::MainProcessor::min(ParserContext& context)
{
  using processor = rychkov::MinProcessor;
  using parser_type = rychkov::Parser< processor >;
  static parser_type::map_type call_map = {
        {"AREA", &processor::area},
        {"VERTEXES", &processor::count}
      };
  static parser_type parser{context, {polygons_}, std::move(call_map)};
  parser.run();
  return true;
}
bool rychkov::MainProcessor::count(ParserContext& context)
{
  using processor = rychkov::CountProcessor;
  using parser_type = rychkov::Parser< processor >;
  static parser_type::map_type call_map = {
        {"EVEN", &processor::even},
        {"ODD", &processor::odd}
      };
  static parser_type parser{context, {polygons_}, std::move(call_map), &processor::count};
  parser.run();
  return true;
}

