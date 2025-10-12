#ifndef PROJECT_BODY_HPP
#define PROJECT_BODY_HPP
#include <map>
#include <list>
#include <string>
#include <shape.hpp>

namespace savintsev
{
  using Layer = std::pair< std::string, Shape * >;
  using Project = std::list< Layer >;
  using Projects = std::map< std::string, Project >;
}

#endif
