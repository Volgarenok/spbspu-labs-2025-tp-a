#include "Commands.h"

#include <exception>
#include <algorithm>
#include <iomanip>
#include <string>

#include "Delimiter.h"
#include "Iofmtguard.h"
#include "Subcommands.h"

const int MIN_AMOUNT_OF_VERTEXES = 3;

void voronina::area(const std::vector<Polygon> &shapes, std::istream &in, std::ostream &out)
{
  if (in.peek() == '\n')
  {
    throw std::invalid_argument("ERROR: No parametrs");
  }

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  if (in.peek() == '\n')
  {
    throw std::invalid_argument("ERROR: No parametrs");
  }

  std::string parametr = "";
  in >> parametr;

  if (in.peek() != '\n')
  {
    throw std::invalid_argument("ERROR: Wrong command");
  }
  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  if (parametr == "EVEN")
  {
    out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, getAreaForEvenPolygons);
  }
  else if (parametr == "ODD")
  {
    out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, getAreaForOddPolygons);
  }
  else if (parametr == "MEAN")
  {
    if (shapes.size() == 0)
    {
      throw std::invalid_argument("ERROR: No shapes");
    }
    out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, getSumArea) / shapes.size();
  }
  else
  {
    if (!std::all_of(parametr.cbegin(), parametr.cend(), isDigitBool))
    {
      throw std::invalid_argument("ERROR: Wrong argument");
    }

    int vertexes = std::stoi(parametr);

    if (vertexes >= MIN_AMOUNT_OF_VERTEXES)
    {
      auto bindForGetArea = std::bind(getAreaOfGivenAmountOfVertexes, _1, _2, vertexes);
      out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, bindForGetArea);
    }
    else
    {
      throw std::invalid_argument("ERROR: Invalid amount of vertexes");
    }
  }
}
void voronina::max(const std::vector<Polygon> &shapes, std::istream &in, std::ostream &out)
{
  if (in.peek() == '\n')
  {
    throw std::invalid_argument("ERROR: No parametrs");
  }

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  if (in.peek() == '\n')
  {
    throw std::invalid_argument("ERROR: No parametrs");
  }

  std::string parametr = "";
  in >> parametr;

  if (in.peek() != '\n')
  {
    throw std::invalid_argument("ERROR: Wrong command");
  }

  if (shapes.size() == 0)
  {
    throw std::invalid_argument("ERROR: No shapes");
  }

  if (parametr == "AREA")
  {
    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);
    out << getAreaOfPolygon(*std::max_element(shapes.cbegin(), shapes.cend(), areaComporator));
  }
  else if (parametr == "VERTEXES")
  {
    out << (*std::max_element(shapes.cbegin(), shapes.cend(), vertexesComporator)).points.size();
  }
}
void voronina::min(const std::vector<Polygon> &shapes, std::istream &in, std::ostream &out)
{
  if (in.peek() == '\n')
  {
    throw std::invalid_argument("ERROR: No parametrs");
  }

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  if (in.peek() == '\n')
  {
    throw std::invalid_argument("ERROR: No parametrs");
  }

  std::string parametr = "";
  in >> parametr;

  if (in.peek() != '\n')
  {
    throw std::invalid_argument("ERROR: Wrong command");
  }

  if (shapes.size() == 0)
  {
    throw std::invalid_argument("ERROR: No shapes");
  }

  if (parametr == "AREA")
  {
    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);
    out << getAreaOfPolygon(*std::min_element(shapes.cbegin(), shapes.cend(), areaComporator));
  }
  else if (parametr == "VERTEXES")
  {
    out << (*std::min_element(shapes.cbegin(), shapes.cend(), vertexesComporator)).points.size();
  }
}
void voronina::count(const std::vector<Polygon> &shapes, std::istream &in, std::ostream &out)
{
  if (in.peek() == '\n')
  {
    throw std::invalid_argument("ERROR: No parametrs");
  }

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  if (in.peek() == '\n')
  {
    throw std::invalid_argument("ERROR: No parametrs");
  }

  std::string parametr = "";
  in >> parametr;

  if (in.peek() != '\n')
  {
    throw std::invalid_argument("ERROR: Wrong command");
  }
  iofmtguard ofmtguard(out);
  out << std::fixed << std::setprecision(1);
  if (parametr == "EVEN")
  {
    out << std::count_if(shapes.cbegin(), shapes.cend(), isEven);
  }
  else if (parametr == "ODD")
  {
    out << std::count_if(shapes.cbegin(), shapes.cend(), isOdd);
  }
  else
  {
    if (!std::all_of(parametr.cbegin(), parametr.cend(), isDigitBool))
    {
      throw std::invalid_argument("ERROR: Wrong argument");
    }

    int vertexes = std::stoi(parametr);

    if (vertexes >= MIN_AMOUNT_OF_VERTEXES)
    {
      out << std::count_if(shapes.cbegin(), shapes.cend(), std::bind(hasGivenAmountOfVertexes, _1, vertexes));
    }
    else
    {
      throw std::invalid_argument("ERROR: Invalid amount of vertexes");
    }
  }
}
void voronina::maxseq(const std::vector<Polygon> &shapes, std::istream &in, std::ostream &out)
{
  if (in.peek() == '\n')
  {
    throw std::invalid_argument("ERROR: No parametrs");
  }

  iofmtguard ifmtguard(in);
  in >> std::noskipws;

  in >> DelimiterIO{' '};

  Polygon polygon;
  in >> polygon;
  if (in.fail())
  {
    throw std::invalid_argument("ERROR: Wrong polygon");
  }

  std::vector<int> supVector(shapes.size());
  std::vector<int>::iterator subVecBegin = supVector.begin();
  std::transform(shapes.cbegin(), shapes.cend(), subVecBegin, std::bind(isEqual, _1, std::cref(polygon)));

  std::transform(subVecBegin + 1, supVector.end(), subVecBegin, subVecBegin + 1, getMaxSeq);
  out << *std::max_element(supVector.cbegin(), supVector.cend());
}
void voronina::rightshapes(const std::vector<Polygon> &shapes, std::istream &in, std::ostream &out)
{
  if (in.peek() != '\n')
  {
    throw std::invalid_argument("ERROR: Do not need any parametrs");
  }
  out << std::count_if(shapes.cbegin(), shapes.cend(), isThereRightAngleInPolygon);
}
