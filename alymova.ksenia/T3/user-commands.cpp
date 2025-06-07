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
  AreaMaxMinSubcommands subs{{"EVEN", bindEven}, {"ODD", bindOdd}, {"MEAN", bindMean}};

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

void alymova::maxAndMin(Comparator< double > cmp_area, Comparator< size_t > cmp_vertexes,
  std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  auto compare_area = std::bind(compareArea, cmp_area, _1, _2);
  auto compare_vertexes = std::bind(compareVertexes, cmp_vertexes, _1, _2);
  AreaMaxMinSubcommands subs{{"AREA", compare_area}, {"VERTEXES", compare_vertexes}};

  if (polygons.empty())
  {
    throw std::logic_error("");
  }
  std::string command;
  in >> command;
  if (command == "AREA")
  {
    double res = std::accumulate(polygons.begin(), polygons.end(), areaPolygon(polygons[0]), subs[command]);
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << res;
    return;
  }
  StreamGuard guard(out);
  out << std::accumulate(polygons.begin(), polygons.end(), polygons[0].points.size(), subs.at(command));
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
  int max_framed_x = findMaxMinXYPolygon(compareMaxXPoint, start_frame_x, framed);
  int max_framed_y = findMaxMinXYPolygon(compareMaxYPoint, start_frame_y, framed);
  int min_framed_x = findMaxMinXYPolygon(compareMinXPoint, start_frame_x, framed);
  int min_framed_y = findMaxMinXYPolygon(compareMinYPoint, start_frame_y, framed);

  int start_x = polygons[0].points[0].x;
  int start_y = polygons[0].points[0].y;
  int max_x = findMaxMinXYVector(compareMaxXPoint, start_x, polygons);
  int max_y = findMaxMinXYVector(compareMaxYPoint, start_y, polygons);
  int min_x = findMaxMinXYVector(compareMinXPoint, start_x, polygons);
  int min_y = findMaxMinXYVector(compareMinYPoint, start_y, polygons);

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
    {"MAX", std::bind(maxAndMin, maxWrapper< double >, maxWrapper< size_t >, std::ref(in), std::ref(out), _1)},
    {"MIN", std::bind(maxAndMin, minWrapper< double >, minWrapper< size_t >, std::ref(in), std::ref(out), _1)},
    {"COUNT", std::bind(count, std::ref(in), std::ref(out), _1)},
    {"INFRAME", std::bind(inFrame, std::ref(in), std::ref(out), _1)},
    {"RIGHTSHAPES", std::bind(rightShapes, std::ref(out), _1)}
  };
}
