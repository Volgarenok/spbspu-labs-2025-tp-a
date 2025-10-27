#ifndef GRAPHSYSTEM_HPP
#define GRAPHSYSTEM_HPP

#include <iostream>
#include <string>
#include <map>
#include "Graph.hpp"

namespace milchevskiy
{
  class GraphSystem
  {
  private:
    std::map<std::string, Graph> graphs;

  public:
    void create(const std::string& name);
    void isEmpty(const std::string& name);
    void searchNode(const std::string& graphName, const std::string& nodeName);
    void searchArc(const std::string& graphName, const std::string& node1, const std::string& node2);
    void insertNode(const std::string& graphName, const std::string& nodeName);
    void insertArc(const std::string& graphName, const std::string& node1, const std::string& node2);
    void deleteNode(const std::string& graphName, const std::string& nodeName);
    void print(const std::string& graphName);
    void clear(const std::string& graphName);
    void isCyclical(const std::string& graphName, const std::string& newGraphName);
    void shortestPath(const std::string& graphName, const std::string& node1, const std::string& node2, const std::string& resultGraphName);
    void topologicalSort(const std::string& graphName, const std::string& newGraphName);
  };
}
#endif
