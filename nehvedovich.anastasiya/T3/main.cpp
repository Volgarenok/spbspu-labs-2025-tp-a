#include "polygon.hpp"
#include "commands.hpp"
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include <map>
#include <functional>

template < typename Fn >
struct BindPolygons
{
  typedef Fn FnType;

  BindPolygons(FnType f, const std::vector< nehvedovich::Polygon > &polys):
    fn(f),
    polygons(polys)
  {}

  void operator()(std::istream &in, std::ostream &out) const
  {
    fn(in, out, polygons);
  }

  FnType fn;
  const std::vector< nehvedovich::Polygon > &polygons;
};

template < typename Fn >
struct BindPolygonsMut
{
  typedef Fn FnType;

  BindPolygonsMut(FnType f, std::vector< nehvedovich::Polygon > &polys):
    fn(f),
    polygons(polys)
  {}

  void operator()(std::istream &in, std::ostream &out) const
  {
    fn(in, out, polygons);
  }

  FnType fn;
  std::vector< nehvedovich::Polygon > &polygons;
};

int main(int argc, char *argv[])
{
  using namespace nehvedovich;

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  std::vector< Polygon > polygons;

  {
    std::ifstream input(argv[1]);
    if (!input)
    {
      std::cerr << "Error: cannot open file " << argv[1] << '\n';
      return 1;
    }

    using input_it_t = std::istream_iterator< Polygon >;
    while (!input.eof())
    {
      std::copy(input_it_t(input), input_it_t(), std::back_inserter(polygons));
      if (input.fail())
      {
        input.clear();
        input.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
  }

  std::map< std::string, std::function< void(std::istream &, std::ostream &) > > commands {
      {"AREA", BindPolygons< decltype(&nehvedovich::areaCommand) >(&nehvedovich::areaCommand, polygons)},
      {"MAX", BindPolygons< decltype(&nehvedovich::maxCommand) >(&nehvedovich::maxCommand, polygons)},
      {"MIN", BindPolygons< decltype(&nehvedovich::minCommand) >(&nehvedovich::minCommand, polygons)},
      {"COUNT", BindPolygons< decltype(&nehvedovich::countCommand) >(&nehvedovich::countCommand, polygons)},
      {"LESSAREA", BindPolygons< decltype(&nehvedovich::lessAreaCommand) >(&nehvedovich::lessAreaCommand, polygons)},
      {"INFRAME", BindPolygons< decltype(&nehvedovich::inFrameCommand) >(&nehvedovich::inFrameCommand, polygons)},
      {"SAME", BindPolygonsMut< decltype(&nehvedovich::sameCommand) >(&nehvedovich::sameCommand, polygons)}};

  while (!std::cin.eof())
  {
    std::string cmd;
    std::cin >> cmd;
    if (cmd.empty())
      continue;

    try
    {
      auto it = commands.find(cmd);
      if (it != commands.end())
      {
        it->second(std::cin, std::cout);
      }
      else
      {
        throw std::runtime_error("Unknown command");
      }
    }
    catch (...)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
