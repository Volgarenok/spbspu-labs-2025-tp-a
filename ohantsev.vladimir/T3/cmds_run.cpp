#include "cmds_run.h"
#include <vector>
#include <limits>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <iofmtguard.h>
#include "polygon_cmds.h"

void ohantsev::fillPolygons(std::vector< Polygon >& polygons, std::ifstream& in)
{
  using inIter = std::istream_iterator< Polygon >;
  while (!in.eof())
  {
    std::copy(inIter{ in }, inIter{}, std::back_inserter(polygons));
    if (in.fail())
    {
      in.clear(in.rdstate() ^ std::ios::failbit);
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

void ohantsev::cmdsHandle(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
  PolygonCmdsHandler handler(polygons, in, out);
  handler.processUntilEOF();
}
