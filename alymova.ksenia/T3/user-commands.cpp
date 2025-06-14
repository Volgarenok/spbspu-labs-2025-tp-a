#include "user-commands.hpp"
#include <algorithm>
#include <numeric>
#include <exception>
#include <string>
#include <iomanip>
#include <stream-guard.hpp>
#include "sub-utils.hpp"

void alymova::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  auto bindEven = std::bind(areaEven, _1, _2, polygons.size());
  auto bindOdd = std::bind(areaOdd, _1, _2, polygons.size());
  auto bindMean = std::bind(areaMean, _1, _2, polygons.size());
  AreaSubcommands subs{{"EVEN", bindEven}, {"ODD", bindOdd}, {"MEAN", bindMean}};

  double res;
  std::string command;
  in >> command;
  try
  {
    subs.at(command)(0.0, *polygons.begin());
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, subs[command]);
  }
  catch (const std::out_of_range& e)
  {
    size_t vertexes = getVertexes(command);
    res = std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(areaNumber, _1, _2, vertexes));
  }
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << res;
}

template< class CmpArea, class CmpVertexes >
void alymova::maxAndMin(CmpArea cmp_area, CmpVertexes cmp_vertexes,
  std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::logic_error("");
  }
  std::string command;
  in >> command;
  if (command == "AREA")
  {
    double res = std::accumulate(polygons.begin(), polygons.end(), areaPolygon(polygons[0]), cmp_area);
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << res;
    return;
  }
  if (command == "VERTEXES")
  {
    out << std::accumulate(polygons.begin(), polygons.end(), polygons[0].points.size(), cmp_vertexes);
    return;
  }
  throw std::logic_error("");
}

void alymova::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  auto bindOdd = std::not1(std::function< bool(const Polygon&) >(isPolygonEven));
  CountSubcommands subs{{"EVEN", isPolygonEven}, {"ODD", bindOdd}};

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
  int start_frame_x = framed.points[0].x;
  int start_frame_y = framed.points[0].y;
  int max_framed_x = findMaxXPolygon(start_frame_x, framed);
  int max_framed_y = findMaxXPolygon(start_frame_y, framed);
  int min_framed_x = findMinXPolygon(start_frame_x, framed);
  int min_framed_y = findMinXPolygon(start_frame_y, framed);

  int start_x = polygons[0].points[0].x;
  int start_y = polygons[0].points[0].y;
  int max_x = findMaxXVector(start_x, polygons);
  int max_y = findMaxYVector(start_y, polygons);
  int min_x = findMinXVector(start_x, polygons);
  int min_y = findMinYVector(start_y, polygons);

  bool res = max_framed_x <= max_x && max_framed_y <= max_y && min_framed_x >= min_x && min_framed_y >= min_y;
  inFrameOutput(out, res);
}

void alymova::rightShapes(std::ostream& out, const std::vector< Polygon >& polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), haveRightAngles);
}

alymova::CommandDataset alymova::complectCommands(std::istream& in, std::ostream& out)
{
  return {
    {"AREA", std::bind(area, std::ref(in), std::ref(out), _1)},
    {"MAX", std::bind(maxAndMin< CmpArea, CmpVertexes >, maxArea, maxVertexes, std::ref(in), std::ref(out), _1)},
    {"MIN", std::bind(maxAndMin< CmpArea, CmpVertexes >, minArea, minVertexes, std::ref(in), std::ref(out), _1)},
    {"COUNT", std::bind(count, std::ref(in), std::ref(out), _1)},
    {"INFRAME", std::bind(inFrame, std::ref(in), std::ref(out), _1)},
    {"RIGHTSHAPES", std::bind(rightShapes, std::ref(out), _1)}
  };
}
