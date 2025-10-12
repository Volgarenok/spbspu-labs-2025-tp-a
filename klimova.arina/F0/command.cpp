#include "command.hpp"
#include <fstream>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include "graph_operations.hpp"

void klimova::loadGraphFromFile(klimova::GraphManager& graphs, const std::string& filename)
{
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  std::string graphName;
  if (!std::getline(file, graphName)) {
    throw std::runtime_error("Empty file");
  }

   if (graphs.find(graphName) != graphs.end()) {
     throw std::runtime_error("Graph with this name already exists");
   }

    std::string vertexLine;
    if (!std::getline(file, vertexLine)) {
      throw std::runtime_error("Missing vertices line");
    }

    Graph<std::string> graph;
    auto start = vertexLine.begin();
    auto end = vertexLine.end();
    auto next = std::find(start, end, ' ');

    while (next != end) {
      graph.addVertex(std::string(start, next));
      start = next + 1;
      next = std::find(start, end, ' ');
    }

    if (start != end) {
      graph.addVertex(std::string(start, end));
    }

    std::string edgeLine;
    while (std::getline(file, edgeLine)) {
      if (edgeLine.empty()) {
        continue;
      }
      auto space_pos = edgeLine.find(' ');
      if (space_pos == std::string::npos || space_pos == 0 || space_pos == edgeLine.length()-1) {
        throw std::runtime_error("Invalid edge format: " + edgeLine);
      }

      std::string src = edgeLine.substr(0, space_pos);
      std::string dest = edgeLine.substr(space_pos + 1);

      try {
        graph.addEdge(src, dest);
      } catch (...) {
        throw std::runtime_error("Invalid edge vertices: " + edgeLine);
      }
    }

    graphs.emplace(graphName, std::move(graph));
}

void klimova::showHelp(std::ostream& out)
{
  out << "Graph Program - Available Commands:\n\n";
  out << " - creategraph <graph> - Create graph with name <graph>";
  out << " - shortestpath <graph> <start> <end> - Find shortest path between vertices\n";
  out << " - longestpath <graph> <start> <end> - Find longest path between vertices\n";
  out << " - checkconnectivity <graph> - Check if graph is connected\n\n";

  out << " - addvertex <graph> <vertex> - Add vertex to graph\n";
  out << " - addedge <graph> <v1> <v2>  - Add edge between vertices\n";
  out << " - removevertex <graph> <vertex> - Remove vertex from graph\n";
  out << " - removeedge <graph> <v1> <v2> - Remove edge between vertices\n";
  out << " - clear <graph>  - Clear graph\n\n";

  out << " - printgraph <graph> - Print graph structure\n";
  out << " - countvertices <graph> - Count vertices in graph\n";
  out << " - countedges <graph> - Count edges in graph\n";
  out << " - findneighbors <graph> <vertex> - Find neighbors of vertex\n";
  out << " - degree <graph> <vertex> - Get degree of vertex\n\n";

  out << " - help - Show this help message\n";
  out << " - GRAPHS - Show all graphs\n\n";
}

klimova::CommandHandler klimova::createCommandHandler(klimova::GraphManager& graphs)
{
  CommandHandler cmdMap;
  cmdMap["creategraph"] = std::bind(createGraph, std::ref(graphs), _1, _2);
  cmdMap["addvertex"] = std::bind(addVertex, std::ref(graphs), _1, _2);
  cmdMap["addedge"] = std::bind(addEdge, std::ref(graphs), _1, _2);
  cmdMap["printgraph"] = std::bind(printGraph, std::cref(graphs), _1, _2);
  cmdMap["removevertex"] = std::bind(removeVertex, std::ref(graphs), _1, _2);
  cmdMap["removeedge"] = std::bind(removeEdge, std::ref(graphs), _1, _2);
  cmdMap["clear"] = std::bind(clearGraph, std::ref(graphs), _1, _2);
  cmdMap["checkconnectivity"] = std::bind(checkConnectivity, std::cref(graphs), _1, _2);
  cmdMap["countvertices"] = std::bind(countVertices, std::cref(graphs), _1, _2);
  cmdMap["countedges"] = std::bind(countEdges, std::cref(graphs), _1, _2);
  cmdMap["findneighbors"] = std::bind(findNeighbors, std::cref(graphs), _1, _2);
  cmdMap["degree"] = std::bind(degreeOfVertex, std::cref(graphs), _1, _2);
  cmdMap["findshortestpath"] = std::bind(findShortestPath, std::cref(graphs), _1, _2);
  cmdMap["findlongestpath"] = std::bind(findLongestPath, std::cref(graphs), _1, _2);
  cmdMap["help"] = std::bind(showHelp, _2);
  cmdMap["GRAPHS"] = std::bind(showGraphs, std::cref(graphs), _2);
  return cmdMap;
}
