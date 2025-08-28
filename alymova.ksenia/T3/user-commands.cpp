#include "user-commands.hpp"
#include <algorithm>
#include <exception>
#include <string>

void alymova::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using Subcommands = std::map< std::string, std::function< double(const std::vector< Polygon >&) > >;
  Subcommands subs{{"EVEN", areaEven}, {"ODD", areaOdd}, {"MEAN", areaMean}};

  double res;
  std::string command;
  in >> command;
  try
  {
    res = subs.at(command)(polygons);
  }
  catch (const std::out_of_range& e)
  {
    size_t vertexes = getVertexes(command);
    res = areaNumber(vertexes, polygons);
  }
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << res;
}

void alymova::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using CompareCommands = std::map< std::string, std::function< bool(const Polygon&, const Polygon&) > >;
  using PrintCommands = std::map< std::string, std::function< void(std::ostream&, const Polygon&) > >;
  CompareCommands cmp_subs{{"AREA", compareArea}, {"VERTEXES", compareVertexes}};
  PrintCommands print_subs{{"AREA", printArea}, {"VERTEXES", printVertexes}};

  if (polygons.empty())
  {
    throw std::logic_error("");
  }
  std::string command;
  in >> command;
  auto res = std::max_element(polygons.begin(), polygons.end(), cmp_subs.at(command));
  print_subs[command](out, *res);
}

void alymova::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using CompareCommands = std::map< std::string, std::function< bool(const Polygon&, const Polygon&) > >;
  using PrintCommands = std::map< std::string, std::function< void(std::ostream&, const Polygon&) > >;
  CompareCommands cmp_subs{{"AREA", compareArea}, {"VERTEXES", compareVertexes}};
  PrintCommands print_subs{{"AREA", printArea}, {"VERTEXES", printVertexes}};

  if (polygons.empty())
  {
    throw std::logic_error("");
  }
  std::string command;
  in >> command;
  auto res = std::min_element(polygons.begin(), polygons.end(), cmp_subs.at(command));
  print_subs[command](out, *res);
}

void alymova::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  using CountSubcommands = std::map< std::string, std::function< bool(const Polygon&) > >;
  CountSubcommands subs{{"EVEN", isPolygonEven}, {"ODD", isPolygonOdd}};

  std::string command;
  in >> command;
  try
  {
    out << std::count_if(polygons.begin(), polygons.end(), subs.at(command));
  }
  catch (const std::out_of_range& e)
  {
    size_t vertexes = getVertexes(command);
    out << std::count_if(polygons.begin(), polygons.end(), std::bind(isEqualSize, vertexes, _1));
  }
}

void alymova::inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon framed;
  in >> framed;
  if (in.fail() || polygons.empty())
  {
    throw std::logic_error("");
  }
  int max_framed_x = findMaxXPolygon(framed);
  int max_framed_y = findMaxYPolygon(framed);
  int min_framed_x = findMinXPolygon(framed);
  int min_framed_y = findMinYPolygon(framed);

  int max_x = findMaxXVector(polygons);
  int max_y = findMaxYVector(polygons);
  int min_x = findMinXVector(polygons);
  int min_y = findMinYVector(polygons);

  bool res = max_framed_x <= max_x && max_framed_y <= max_y && min_framed_x >= min_x && min_framed_y >= min_y;
  inFrameOutput(out, res);
}

void alymova::rightShapes(std::ostream& out, const std::vector< Polygon >& polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), haveRightAngles);
}
