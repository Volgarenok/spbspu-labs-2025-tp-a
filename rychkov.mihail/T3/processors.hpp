#ifndef PROCESSORS_HPP
#define PROCESSORS_HPP

#include <parser.hpp>
#include "polygon.hpp"

namespace rychkov
{
  class MainProcessor
  {
  public:
    static Parser::map_type< ParserContext, MainProcessor > call_map;

    MainProcessor(int argc, char** argv);

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
    static Parser::map_type< ParserContext, AreaProcessor > call_map;

    AreaProcessor(std::vector< Polygon >& polygons) noexcept;
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
    static Parser::map_type< ParserContext, MaxProcessor > call_map;

    MaxProcessor(std::vector< Polygon >& polygons) noexcept;
    bool area(ParserContext& context);
    bool count(ParserContext& context);
  private:
    std::vector< Polygon >& polygons_;
  };
  class MinProcessor
  {
  public:
    static Parser::map_type< ParserContext, MinProcessor > call_map;

    MinProcessor(std::vector< Polygon >& polygons) noexcept;
    bool area(ParserContext& context);
    bool count(ParserContext& context);
  private:
    std::vector< Polygon >& polygons_;
  };
  class CountProcessor
  {
  public:
    static Parser::map_type< ParserContext, CountProcessor > call_map;

    CountProcessor(std::vector< Polygon >& polygons) noexcept;
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
