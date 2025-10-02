#ifndef COMMANDS_H
#define COMMANDS_H

#include <functional>
#include <iostream>
#include <vector>
#include <map>
#include "polygon.hpp"

namespace klimova
{
  using namespace std::placeholders;
  using VecPolygon = std::vector< Polygon >;

  using AreaCommand = std::function< void(const VecPolygon&, std::istream&, std::ostream&) >;
  using AreaSubs = std::map< std::string, AreaCommand >;
  using MaxCommand = std::function< void(const VecPolygon&, std::istream&, std::ostream&) >;
  using MaxSubs = std::map< std::string, MaxCommand >;
  using MinCommand = std::function< void(const VecPolygon&, std::istream&, std::ostream&) >;
  using MinSubs = std::map< std::string, MinCommand >;
  using CountCommand = std::function< void(const VecPolygon&, std::istream&, std::ostream&) >;
  using CountSubs = std::map< std::string, CountCommand >;

  using CommandHandler = std::map< std::string, std::function< void() > >;

  CommandHandler createCommandHandler(const VecPolygon& polygons);
  AreaSubs createAreaSubs();
  MaxSubs createMaxSubs();
  MinSubs createMinSubs();
  CountSubs createCountSubs();

  void area(const VecPolygon& polygons, std::istream& is, std::ostream& os);
  void max(const VecPolygon& polygons, std::istream& is, std::ostream& os);
  void min(const VecPolygon& polygons, std::istream& is, std::ostream& os);
  void count(const VecPolygon& polygons, std::istream& is, std::ostream& os);
  void perms(const VecPolygon& polygons, std::istream& is, std::ostream& os);
  void rects(const VecPolygon& polygons, std::ostream& os);
}
#endif
