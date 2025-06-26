#include "Commands.h"

#include <exception>
#include <algorithm>
#include <iomanip>
#include <string>
#include <map>

#include <IOFmtguard.h>
#include <Delimiter.h>
#include "Subcommands.h"

void voronina::area(const std::vector< Polygon > &shapes, std::istream &in, std::ostream &out)
{
  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(voronina::getEven, std::cref(shapes), std::ref(out));
  subcmds["ODD"] = std::bind(voronina::getOdd, std::cref(shapes), std::ref(out));
  subcmds["MEAN"] = std::bind(voronina::getMean, std::cref(shapes), std::ref(out));

  std::string parametr = "";
  in >> parametr;

  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  try
  {
    subcmds.at(parametr)();
  }
  catch(...)
  {
    if (!std::all_of(parametr.cbegin(), parametr.cend(), isDigitBool))
    {
      throw std::invalid_argument("ERROR: Wrong argument");
    }

    int vertexes = std::stoi(parametr);

    voronina::getVertexes(shapes, out, vertexes);
  }
}

void voronina::max(const std::vector< Polygon > &shapes, std::istream &in, std::ostream &out)
{
  if (shapes.size() == 0)
  {
    throw std::invalid_argument("ERROR: No shapes");
  }

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(voronina::getAreaMax, std::cref(shapes), std::ref(out));
  subcmds["VERTEXES"] = std::bind(voronina::getVertexesMax, std::cref(shapes), std::ref(out));

  std::string parametr = "";
  in >> parametr;

  subcmds.at(parametr)();
}

void voronina::min(const std::vector< Polygon > &shapes, std::istream &in, std::ostream &out)
{
  if (shapes.size() == 0)
  {
    throw std::invalid_argument("ERROR: No shapes");
  }

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  std::map< std::string, std::function< void() > > subcmds;
  subcmds["AREA"] = std::bind(voronina::getAreaMin, std::cref(shapes), std::ref(out));
  subcmds["VERTEXES"] = std::bind(voronina::getVertexesMin, std::cref(shapes), std::ref(out));

  std::string parametr = "";
  in >> parametr;

  subcmds.at(parametr)();
}

void voronina::count(const std::vector< Polygon > &shapes, std::istream &in, std::ostream &out)
{

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  std::map< std::string, std::function< void() > > subcmds;
  subcmds["EVEN"] = std::bind(voronina::getEvenCount, std::cref(shapes), std::ref(out));
  subcmds["ODD"] = std::bind(voronina::getOddCount, std::cref(shapes), std::ref(out));

  std::string parametr = "";
  in >> parametr;

  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  try
  {
    subcmds.at(parametr)();
  }
  catch(...)
  {
    if (!std::all_of(parametr.cbegin(), parametr.cend(), isDigitBool))
    {
      throw std::invalid_argument("ERROR: Wrong argument");
    }

    int vertexes = std::stoi(parametr);

    voronina::getVertexesCount(shapes, out, vertexes);
  }
}

void voronina::maxseq(const std::vector< Polygon > &shapes, std::istream &in, std::ostream &out)
{
  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  Polygon polygon;
  in >> polygon;
  if (in.fail())
  {
    throw std::invalid_argument("ERROR: Wrong polygon");
  }

  std::vector< int > supVector(shapes.size());
  std::vector< int >::iterator subVecBegin = supVector.begin();
  std::transform(shapes.cbegin(), shapes.cend(), subVecBegin, std::bind(isEqual, _1, std::cref(polygon)));

  std::transform(subVecBegin + 1, supVector.end(), subVecBegin, subVecBegin + 1, getMaxSeq);
  out << *std::max_element(supVector.cbegin(), supVector.cend());
}

void voronina::rightshapes(const std::vector< Polygon > &shapes, std::ostream &out)
{
  out << std::count_if(shapes.cbegin(), shapes.cend(), isThereRightAngleInPolygon);
}
