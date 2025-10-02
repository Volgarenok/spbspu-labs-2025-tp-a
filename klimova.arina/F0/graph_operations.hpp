#ifndef GRAPH_OPERATIONS_HPP
#define GRAPH_OPERATIONS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "graph.hpp"

namespace klimova {
  using GraphType = Graph< std::string >;
  using GraphManager = std::unordered_map< std::string, GraphType >;
  void createGraph(GraphManager& graphs, std::istream& in, std::ostream& out);
  void addVertex(GraphManager& graphs, std::istream& in, std::ostream& out);
  void addEdge(GraphManager& graphs, std::istream& in, std::ostream& out);
  void printGraph(const GraphManager& graphs, std::istream& in, std::ostream& out);
  void removeVertex(GraphManager& graphs, std::istream& in, std::ostream& out);
  void removeEdge(GraphManager& graphs, std::istream& in, std::ostream& out);
  void clearGraph(GraphManager& graphs, std::istream& in, std::ostream& out);
  void checkConnectivity(const GraphManager& graphs, std::istream& in, std::ostream& out);
  void countVertices(const GraphManager& graphs, std::istream& in, std::ostream& out);
  void countEdges(const GraphManager& graphs, std::istream& in, std::ostream& out);
  void findNeighbors(const GraphManager& graphs, std::istream& in, std::ostream& out);
  void degreeOfVertex(const GraphManager& graphs, std::istream& in, std::ostream& out);
  void findShortestPath(const GraphManager& graphs, std::istream& in, std::ostream& out);
  void findLongestPath(const GraphManager& graphs, std::istream& in, std::ostream& out);
  void showGraphs(const GraphManager& graphs, std::ostream& out);
}

#endif
