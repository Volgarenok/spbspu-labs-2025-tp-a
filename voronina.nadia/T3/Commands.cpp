#include "Commands.h"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <map>
#include <string>

#include <Delimiter.h>
#include <IOFmtguard.h>
#include "Subcommands.h"

using namespace std::placeholders;

void voronina::area(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{
  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{ ' ' };

  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getEven, std::cref(shapes), std::ref(out));
  subcmds["ODD"] = std::bind(getOdd, std::cref(shapes), std::ref(out));
  subcmds["MEAN"] = std::bind(getMean, std::cref(shapes), std::ref(out));

  std::string parametr = "";
  in >> parametr;

  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  try
  {
    subcmds.at(parametr)();
    return;
  }
  catch (...)
  {}
  int vertexes = std::stoi(parametr);
  getVertexes(shapes, out, vertexes);
}

void voronina::max(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{
  if (shapes.size() == 0)
  {
    throw std::invalid_argument("ERROR: No shapes");
  }

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{ ' ' };

  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getAreaMax, std::cref(shapes), std::ref(out));
  subcmds["VERTEXES"] = std::bind(getVertexesMax, std::cref(shapes), std::ref(out));

  std::string parametr = "";
  in >> parametr;

  subcmds.at(parametr)();
}

void voronina::min(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{
  if (shapes.size() == 0)
  {
    throw std::invalid_argument("ERROR: No shapes");
  }

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{ ' ' };

  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(getAreaMin, std::cref(shapes), std::ref(out));
  subcmds["VERTEXES"] = std::bind(getVertexesMin, std::cref(shapes), std::ref(out));

  std::string parametr = "";
  in >> parametr;

  subcmds.at(parametr)();
}

void voronina::count(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{ ' ' };

  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getEvenCount, std::cref(shapes), std::ref(out));
  subcmds["ODD"] = std::bind(getOddCount, std::cref(shapes), std::ref(out));
  subcmds["REGULAR"] = std::bind(countRegular, std::cref(shapes), std::ref(in), std::ref(out));

  std::string parametr = "";
  in >> parametr;

  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  try
  {
    subcmds.at(parametr)();
    return;
  }
  catch (...)
  {}
  int vertexes = std::stoi(parametr);
  getVertexesCount(shapes, out, vertexes);
}

void voronina::maxseq(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{
  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{ ' ' };

  Polygon polygon;
  in >> polygon;
  if (in.fail())
  {
    throw std::invalid_argument("ERROR: Wrong polygon");
  }

  std::vector< int > supVector(shapes.size());
  std::vector< int >::iterator subVecBegin = supVector.begin();
  std::transform(shapes.cbegin(), shapes.cend(), subVecBegin,
                 std::bind(std::equal_to< Polygon >{}, _1, std::cref(polygon)));

  std::transform(subVecBegin + 1, supVector.end(), subVecBegin, subVecBegin + 1, maxSeqFolder);
  out << *std::max_element(supVector.cbegin(), supVector.cend());
}

void voronina::countRegular(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{ ' ' };

  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(getRegularEvenCount, std::cref(shapes), std::ref(out));
  subcmds["ODD"] = std::bind(getRegularOddCount, std::cref(shapes), std::ref(out));

  std::string parametr = "";
  in >> parametr;

  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  try
  {
    subcmds.at(parametr)();
    return;
  }
  catch (...)
  {}
  int vertexes = std::stoi(parametr);
  getRegularVertexesCount(shapes, out, vertexes);
}

void voronina::rightshapes(const std::vector< Polygon >& shapes, std::ostream& out)
{
  out << std::count_if(shapes.cbegin(), shapes.cend(), isThereRightAngleInPolygon);
}
