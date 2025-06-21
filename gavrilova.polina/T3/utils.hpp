#ifndef UTILS_HPP
#define UTILS_HPP

#include <functional>
#include <map>
#include <numeric>
#include <string>
#include <vector>
#include "Polygon.hpp"

namespace gavrilova {
  template < typename Container, typename Init, typename Func, typename Op >
  auto applyCommand(const Container& cont, Init init, Func func, Op op);

  std::vector< std::string > tokenize(std::vector< std::string >& tokens, const std::string& str);
  void readFile(const std::string& filename, std::vector< Polygon >& polygons);
  void startCommandInterface(const std::string& filename, std::istream& is, std::ostream& out);

  template < typename Container, typename Init, typename Func, typename Op >
  auto applyCommand(const Container& cont, Init init, Func func, Op op)
  {
    auto compose = [func, op](auto acc, const auto& elem) {
      return op(acc, func(elem));
    };
    return std::accumulate(cont.begin(), cont.end(), init, compose);
  }
}

#endif
