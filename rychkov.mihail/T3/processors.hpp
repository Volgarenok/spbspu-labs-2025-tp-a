#ifndef PROCESSORS_HPP
#define PROCESSORS_HPP

#include "parser.hpp"
#include "polygon.hpp"

namespace rychkov
{
  class MainProcessor
  {
  public:
    bool init(ParserContext& context, int argc, char** argv);
    bool area(ParserContext& context);
    bool max(ParserContext& context);
    bool min(ParserContext& context);
    bool count(ParserContext& context);
    bool remove_repeates(ParserContext& context);
    bool rectangles(ParserContext& context);
  private:
    std::vector< Polygon > polygons_;
  };
  class AreaProcessor
  {
  public:
    AreaProcessor(std::vector< Polygon >& polygons):
      polygons_(polygons)
    {}
    bool even(ParserContext& context);
    bool odd(ParserContext& context);
    bool mean(ParserContext& context);
    bool count(ParserContext& context);
  private:
    std::vector< Polygon >& polygons_;
  };
  class MaxProcessor
  {
  public:
    MaxProcessor(std::vector< Polygon >& polygons):
      polygons_(polygons)
    {}
    bool area(ParserContext& context);
    bool count(ParserContext& context);
  private:
    std::vector< Polygon >& polygons_;
  };
  class MinProcessor
  {
  public:
    MinProcessor(std::vector< Polygon >& polygons):
      polygons_(polygons)
    {}
    bool area(ParserContext& context);
    bool count(ParserContext& context);
  private:
    std::vector< Polygon >& polygons_;
  };
  class CountProcessor
  {
  public:
    CountProcessor(std::vector< Polygon >& polygons):
      polygons_(polygons)
    {}
    bool even(ParserContext& context);
    bool odd(ParserContext& context);
    bool count(ParserContext& context);
  private:
    std::vector< Polygon >& polygons_;
  };
  size_t parse_as_count(ParserContext& context);
  struct get_area
  {
    double operator()(const Polygon& polygon);
  };
  struct area_accumulator
  {
    double operator()(const Point& prev, const Point& cur);
  };
  template< class T >
  struct if_statement
  {
    T operator()(bool cond, T success, T fail)
    {
      return cond ? success : fail;
    }
  };
}

#endif
