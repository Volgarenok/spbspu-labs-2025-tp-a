#ifndef POLYGON_COMMANDS_H
#define POLYGON_COMMANDS_H
#include <vector>
#include <iosfwd>
#include <functional>
#include <command_handler.h>
#include "polygon.h"

namespace ohantsev
{
  bool isOdd(const Polygon& polygon) noexcept;
  bool isEven(const Polygon& polygon) noexcept;
  bool thisSize(const Polygon& polygon, std::size_t size) noexcept;
  bool lessSize(const Polygon& lhs, const Polygon& rhs) noexcept;

  class Area: public CommandHandler
  {
  public:
    void operator()() override;
    Area(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out);

  private:
    const std::vector< Polygon >& polygons_;

    static void odd(const std::vector< Polygon >& polygons, std::ostream& out);
    static void even(const std::vector< Polygon >& polygons, std::ostream& out);
    static void numOfVertexes(const std::vector< Polygon >& polygons, std::ostream& out, std::size_t num);
    static void mean(const std::vector< Polygon >& polygons, std::ostream& out);

    static double accumulateArea(const std::vector< Polygon >& polygons);
    static double accumulateAreaIf(const std::vector< Polygon >& polygons, const std::function< bool(const Polygon&) >& sign);
  };

  class Max: public CommandHandler
  {
  public:
    Max(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out);

  private:
    const std::vector< Polygon >& polygons_;

    static void area(const std::vector< Polygon >& polygons, std::ostream& out);
    static void vertexes(const std::vector< Polygon >& polygons, std::ostream& out);
  };

  class Min: public CommandHandler
  {
  public:
    Min(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out);

  private:
    const std::vector< Polygon >& polygons_;

    static void area(const std::vector< Polygon >& polygons, std::ostream& out);
    static void vertexes(const std::vector< Polygon >& polygons, std::ostream& out);
  };

  class Count: public CommandHandler
  {
  public:
    void operator()() override;
    Count(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out);

  private:
    const std::vector< Polygon >& polygons_;

    static void odd(const std::vector< Polygon >& polygons, std::ostream& out);
    static void even(const std::vector< Polygon >& polygons, std::ostream& out);
    static void numOfVertexes(const std::vector< Polygon >& polygons, std::ostream& out, std::size_t num);
  };

  class PolygonCmdsHandler: public CommandHandler
  {
  public:
    PolygonCmdsHandler(std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out);
    void operator()() override;
    void processUntilEOF();
  };

  void perms(const std::vector< Polygon >& polygons,  std::istream& in, std::ostream& out);
  Polygon getSorted(const Polygon& polygon);

  void rects(const std::vector< Polygon >& polygons, std::ostream& out);
  bool isRect(const Polygon& polygon);
  Point getVec(const Point& lhs, const Point& rhs);
  bool isOrthogonal(const Point& lhs, const Point& rhs);
}
#endif
