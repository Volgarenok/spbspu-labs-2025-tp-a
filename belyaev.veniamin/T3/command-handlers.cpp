#include "command-handlers.hpp"
#include <stream-guard.hpp>

template< typename Pred >
void belyaev::areaOddEven(const std::vector< Polygon >& data, std::ostream& out, Pred predicate)
{
  std::vector< double > areas;
  std::vector< Polygon > polysByPredicate;

  std::copy_if(data.begin(), data.end(), std::back_inserter(polysByPredicate), predicate);
  std::transform(polysByPredicate.begin(), polysByPredicate.end(), std::back_inserter(areas), calcArea);
  double totalArea = std::accumulate(areas.begin(), areas.end(), 0.0);
  areaOut(totalArea, out);
}

void belyaev::areaMean(const std::vector< Polygon >& data, std::ostream& out)
{
  if (data.size() == 0)
  {
    throw std::logic_error("Invalid query in AREA MEAN.");
  }

  std::vector< double > areas;
  std::transform(data.begin(), data.end(), std::back_inserter(areas), calcArea);

  double totalArea = std::accumulate(areas.begin(), areas.end(), 0.0);
  double meanArea = totalArea / data.size();
  areaOut(meanArea, out);
}

void belyaev::areaVertices(const std::vector< Polygon >& data, std::ostream& out, size_t vertices)
{
  using namespace std::placeholders;

  std::vector< double > areas;
  std::vector< Polygon > matchingPolygons;

  auto isPolyOfSizeBind = std::bind(isPolygonOfSize, _1, std::ref(vertices));
  std::copy_if(data.begin(), data.end(), std::back_inserter(matchingPolygons), isPolyOfSizeBind);
  std::transform(matchingPolygons.begin(), matchingPolygons.end(), std::back_inserter(areas), calcArea);

  double totalArea = std::accumulate(areas.begin(), areas.end(), 0.0);
  areaOut(totalArea, out);
}

void belyaev::area(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;

  std::string subcommand;
  in >> subcommand;
  if (in.fail())
  {
    throw std::logic_error("Input failed in AREA.");
  }

  std::map< std::string, std::function< void() > > subCmds;
  using functionBoolPoly = std::function< bool(const Polygon&) >;
  subCmds["ODD"] = std::bind(areaOddEven< functionBoolPoly >, std::ref(data), std::ref(out), isPolygonOdd);
  subCmds["EVEN"] = std::bind(areaOddEven< functionBoolPoly >, std::ref(data), std::ref(out), isPolygonEven);
  subCmds["MEAN"] = std::bind(areaMean, std::ref(data), std::ref(out));
  try
  {
    subCmds.at(subcommand)();
  }
  catch (const std::out_of_range& e)
  {
    if (isStringNumeric(subcommand))
    {
      size_t vertices = std::stoull(subcommand);
      if (vertices < 3)
      {
        throw std::logic_error("Invalid query in AREA.");
      }
      areaVertices(data, out, vertices);
    }
    else
    {
      throw std::logic_error("Unknown command.");
    }
  }
}

void belyaev::minMaxArea(const std::vector< Polygon >& data, std::ostream& out, const std::string& command)
{
  Polygon resultingPolygon;
  std::map< std::string, std::function< Polygon() > > minOrMax;
  minOrMax["min"] = std::bind(minElement, std::cref(data), compareAreas);
  minOrMax["max"] = std::bind(maxElement, std::cref(data), compareAreas);
  try
  {
    resultingPolygon = minOrMax.at(command)();
  }
  catch (const std::out_of_range& e)
  {
    throw std::logic_error("minMaxArea failed.");
  }

  areaOut(calcArea(resultingPolygon), out);
}

void belyaev::minMaxVertices(const std::vector< Polygon >& data, std::ostream& out, const std::string& command)
{
  Polygon resultingPolygon;
  std::map< std::string, std::function< Polygon() > > minOrMax;
  minOrMax["min"] = std::bind(minElement, std::cref(data), compareVertices);
  minOrMax["max"] = std::bind(maxElement, std::cref(data), compareVertices);
  try
  {
    resultingPolygon = minOrMax.at(command)();
  }
  catch (const std::out_of_range& e)
  {
    throw std::logic_error("minMaxVertices failed.");
  }

  StreamGuard guard(out);
  out << getVertices(resultingPolygon) << '\n';
}

void belyaev::minMax(const std::vector< Polygon >& data, std::istream& in, std::ostream& out, const std::string& command)
{
  using namespace std::placeholders;

  std::string subcommand;
  in >> subcommand;
  if (in.fail() || data.size() == 0)
  {
    throw std::logic_error("Input failed in MIN/MAX.");
  }

  std::map< std::string, std::function< void() > > subCmds;
  subCmds["AREA"] = std::bind(minMaxArea, std::ref(data), std::ref(out), std::cref(command));
  subCmds["VERTEXES"] = std::bind(minMaxVertices, std::ref(data), std::ref(out), std::cref(command));
  try
  {
    subCmds.at(subcommand)();
  }
  catch (const std::out_of_range& e)
  {
    throw std::logic_error("Unknown command.");
  }
}

void belyaev::countEven(const std::vector< Polygon >& data, std::ostream& out)
{
  using namespace std::placeholders;
  auto isPolygonEvenBind = std::bind(isPolygonEven, _1);
  out << std::count_if(data.begin(), data.end(), isPolygonEvenBind) << '\n';
}

void belyaev::countOdd(const std::vector< Polygon >& data, std::ostream& out)
{
  using namespace std::placeholders;
  auto isPolygonOddBind = std::bind(isPolygonOdd, _1);
  out << std::count_if(data.begin(), data.end(), isPolygonOddBind) << '\n';
}

void belyaev::countVertices(const std::vector< Polygon >& data, std::ostream& out, size_t givenSize)
{
  using namespace std::placeholders;
  auto isPolygonOfSizeBind = std::bind(isPolygonOfSize, _1, givenSize);
  out << std::count_if(data.begin(), data.end(), isPolygonOfSizeBind) << '\n';
}

void belyaev::count(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  std::map< std::string, std::function< void() > > subCmds;
  subCmds["EVEN"] = std::bind(countEven, std::cref(data), std::ref(out));
  subCmds["ODD"] = std::bind(countOdd, std::cref(data), std::ref(out));

  std::string subcommand;
  in >> subcommand;
  if (in.fail())
  {
    throw std::logic_error("Input failed in COUNT.");
  }

  try
  {
    subCmds.at(subcommand)();
  }
  catch (const std::out_of_range& e)
  {
    if (isStringNumeric(subcommand))
    {
      size_t vertices = std::stoull(subcommand);
      if (vertices < 3)
      {
        throw std::logic_error("Invalid query in COUNT.");
      }
      countVertices(data, out, std::stoull(subcommand));
    }
    else
    {
      throw std::logic_error("Unknown command.");
    }
  }
}

void belyaev::rmecho(std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;

  Polygon rmPolygon;
  in >> rmPolygon;
  if (in.fail() || in.peek() != '\n')
  {
    throw std::logic_error("Input failed in RMECHO.");
  }

  size_t oldSize = data.size();
  auto helperBind = std::bind(rmEchoHelper, rmPolygon, _1, _2);
  auto new_end = std::unique(data.begin(), data.end(), helperBind);
  data.erase(new_end, data.end());
  size_t removed = oldSize - data.size();

  StreamGuard guard(out);
  out << removed << '\n';
}

void belyaev::inframe(const std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;

  Polygon inframePoly;
  in >> inframePoly;
  if (in.fail() || in.peek() != '\n')
  {
    throw std::logic_error("Input failed in INFRAME.");
  }

  Borders polysBox = getMaxPolygonBox(data);
  auto isPointInBordersBind = std::bind(isPointInBorders, _1, std::cref(polysBox));
  bool inside = std::all_of(inframePoly.points.begin(), inframePoly.points.end(), isPointInBordersBind);

  StreamGuard guard(out);
  if (inside)
  {
    out << "<TRUE>\n";
  }
  else
  {
    out << "<FALSE>\n";
  }
}

belyaev::commandMap belyaev::mapCommandHandlers(std::vector< Polygon >& data)
{
  using namespace std::placeholders;
  commandMap cmds;
  const std::string minCommand = "min";
  const std::string maxCommand = "max";
  cmds["AREA"] = std::bind(area, std::cref(data), _1, _2);
  cmds["MAX"] = std::bind(minMax, std::cref(data), _1, _2, maxCommand);
  cmds["MIN"] = std::bind(minMax, std::cref(data), _1, _2, minCommand);
  cmds["COUNT"] = std::bind(count, std::cref(data), _1, _2);
  cmds["RMECHO"] = std::bind(rmecho, std::ref(data), _1, _2);
  cmds["INFRAME"] = std::bind(inframe, std::cref(data), _1, _2);
  return cmds;
}
