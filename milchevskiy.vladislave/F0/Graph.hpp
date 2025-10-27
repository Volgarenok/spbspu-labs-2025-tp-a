#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace milchevskiy
{
  struct CycleInfo
  {
    bool hasCycle;
    std::set<std::string> cycleNodes;
  };

  class Graph
  {
  private:
    std::map<std::string, std::set<std::string>> adjacencyList;

    bool dfsFindCycleNodes(const std::string& node, std::map<std::string, int>& color, std::map<std::string, std::string>& parent, CycleInfo& result) const;
    std::vector<std::string> extractCycleFromStack(const std::vector<std::string>& stack, const std::string& cycleEnd) const;
  public:
    Graph() = default;
    bool isEmpty() const;
    bool searchNode(const std::string& node) const;
    bool searchArc(const std::string& node1, const std::string& node2) const;
    bool insertNode(const std::string& node);
    bool insertArc(const std::string& node1, const std::string& node2);
    bool deleteNode(const std::string& node);
    void clear();
    std::string print() const;
    std::set<std::string> getNeighbors(const std::string& node) const;
    CycleInfo findCycleNodes() const;
    std::vector<std::string> topologicalSort() const;
    std::vector<std::string> shortestPath(const std::string& start, const std::string& end) const;
    const std::map<std::string, std::set<std::string>>& getAdjacencyList() const;
  };
}
#endif
