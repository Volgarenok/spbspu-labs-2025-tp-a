#include "command.hpp"
#include "polygon.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>

std::size_t maxSequence(const std::vector<Polygon>& polys, const Polygon& target) {
  std::size_t maxCount = 0;
  std::size_t current = 0;

  std::for_each(polys.begin(), polys.end(), [&](const Polygon& p) {
    if (p == target) {
      ++current;
      maxCount = std::max(maxCount, current);
    } else {
      current = 0;
    }
  });

  return maxCount;
}

void processCommand(const std::string& input, const std::vector<Polygon>& polygons) {
  std::istringstream iss(input);
  std::string command;
  if (!(iss >> command)) {
    std::cout << "INVALID COMMAND\n";
    return;
  }

  std::string args;
  std::getline(iss, args);
  Polygon query;

  if (!parsePolygon(args, query)) {
    std::cout << "INVALID COMMAND\n";
    return;
  }

  if (command == "LESSAREA") {
    double queryArea = computeArea(query);
    auto lessArea = std::bind(
      std::less<double>(),
      std::bind(computeArea, std::placeholders::_1),
      queryArea
    );
    std::size_t count = std::count_if(polygons.begin(), polygons.end(), lessArea);
    std::cout << count << '\n';
  } else if (command == "MAXSEQ") {
    std::cout << maxSequence(polygons, query) << '\n';
  } else {
    std::cout << "INVALID COMMAND\n";
  }
}
