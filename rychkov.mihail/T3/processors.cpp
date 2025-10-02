#include "processors.hpp"

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <limits>

rychkov::Parser::map_type< rychkov::ParserContext, rychkov::MainProcessor > rychkov::MainProcessor::call_map = {
      {"AREA", &rychkov::MainProcessor::area},
      {"MAX", &rychkov::MainProcessor::max},
      {"MIN", &rychkov::MainProcessor::min},
      {"COUNT", &rychkov::MainProcessor::count},
      {"RMECHO", &rychkov::MainProcessor::remove_repeates},
      {"RECTS", &rychkov::MainProcessor::rectangles}
    };
rychkov::Parser::map_type< rychkov::ParserContext, rychkov::AreaProcessor > rychkov::AreaProcessor::call_map = {
      {"EVEN", &rychkov::AreaProcessor::even},
      {"ODD", &rychkov::AreaProcessor::odd},
      {"MEAN", &rychkov::AreaProcessor::mean}
    };
rychkov::Parser::map_type< rychkov::ParserContext, rychkov::MaxProcessor > rychkov::MaxProcessor::call_map = {
      {"AREA", &rychkov::MaxProcessor::area},
      {"VERTEXES", &rychkov::MaxProcessor::count}
    };
rychkov::Parser::map_type< rychkov::ParserContext, rychkov::MinProcessor > rychkov::MinProcessor::call_map = {
      {"AREA", &rychkov::MinProcessor::area},
      {"VERTEXES", &rychkov::MinProcessor::count}
    };
rychkov::Parser::map_type< rychkov::ParserContext, rychkov::CountProcessor > rychkov::CountProcessor::call_map = {
      {"EVEN", &rychkov::CountProcessor::even},
      {"ODD", &rychkov::CountProcessor::odd}
    };

rychkov::MainProcessor::MainProcessor(int argc, char** argv)
{
  if (argc != 2)
  {
    throw std::invalid_argument("wrong arguments count");
  }
  std::ifstream file{argv[1]};
  if (!file)
  {
    throw std::invalid_argument("failed to open file");
  }
  while (file.good())
  {
    using Iter = std::istream_iterator< Polygon >;
    std::copy(Iter{file}, Iter{}, std::back_inserter(polygons_));
    file.clear(file.rdstate() & ~std::ios::failbit);
    file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
rychkov::AreaProcessor::AreaProcessor(std::vector< Polygon >& polygons) noexcept:
  polygons_(polygons)
{}
rychkov::MaxProcessor::MaxProcessor(std::vector< Polygon >& polygons) noexcept:
  polygons_(polygons)
{}
rychkov::MinProcessor::MinProcessor(std::vector< Polygon >& polygons) noexcept:
  polygons_(polygons)
{}
rychkov::CountProcessor::CountProcessor(std::vector< Polygon >& polygons) noexcept:
  polygons_(polygons)
{}

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
  rychkov::AreaProcessor processor{polygons_};
  if (!processor.count(context))
  {
    rychkov::Parser::parse(context, processor, rychkov::AreaProcessor::call_map);
  }
  return true;
}
bool rychkov::MainProcessor::max(ParserContext& context)
{
  rychkov::MaxProcessor processor{polygons_};
  rychkov::Parser::parse(context, processor, rychkov::MaxProcessor::call_map);
  return true;
}
bool rychkov::MainProcessor::min(ParserContext& context)
{
  rychkov::MinProcessor processor{polygons_};
  rychkov::Parser::parse(context, processor, rychkov::MinProcessor::call_map);
  return true;
}
bool rychkov::MainProcessor::count(ParserContext& context)
{
  rychkov::CountProcessor processor{polygons_};
  if (!processor.count(context))
  {
    rychkov::Parser::parse(context, processor, rychkov::CountProcessor::call_map);
  }
  return true;
}

