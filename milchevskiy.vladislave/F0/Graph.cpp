#include "Graph.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <numeric>
#include <unordered_map>
#include <functional>

struct NodeRemover
{
  const std::string& node;

  NodeRemover(const std::string& n) : node(n)
  {
  }

  int operator()(int acc, std::pair<const std::string, std::set<std::string>>& pair) const
  {
    pair.second.erase(node);
    return 0;
  }
};

struct NeighborAccumulator
{
  std::string operator()(const std::string& acc, const std::string& neighbor) const
  {
    return acc.empty() ? neighbor : acc + ", " + neighbor;
  }
};

struct GraphPrinter
{
  std::string operator()(const std::string& acc, const std::pair<const std::string, std::set<std::string>>& pair) const
  {
    std::string neighbors = std::accumulate(pair.second.begin(), pair.second.end(), std::string{}, NeighborAccumulator{});
    return acc + pair.first + ": " + neighbors + "\n";
  }
};

namespace milchevskiy
{
  bool Graph::isEmpty() const
  {
    return adjacencyList.empty();
  }

  bool Graph::searchNode(const std::string& node) const
  {
    return adjacencyList.find(node) != adjacencyList.end();
  }

  bool Graph::searchArc(const std::string& node1, const std::string& node2) const
  {
    auto it = adjacencyList.find(node1);
    if (it == adjacencyList.end()) return false;
    return it->second.find(node2) != it->second.end();
  }

  bool Graph::insertNode(const std::string& node)
  {
    if (searchNode(node)) return false;
    adjacencyList[node] = std::set<std::string>();
    return true;
  }

  bool Graph::insertArc(const std::string& node1, const std::string& node2)
  {
    if (!searchNode(node1) || !searchNode(node2)) return false;
    if (searchArc(node1, node2)) return false;
    adjacencyList[node1].insert(node2);
    return true;
  }

  bool Graph::deleteNode(const std::string& node)
  {
    if (!searchNode(node)) return false;

    adjacencyList.erase(node);

    std::accumulate(adjacencyList.begin(), adjacencyList.end(), 0, NodeRemover(node));

    return true;
  }

  void Graph::clear()
  {
    adjacencyList.clear();
  }

  std::string Graph::print() const
  {
    if (isEmpty()) return "GRAPH IS EMPTY";
    return std::accumulate(adjacencyList.begin(), adjacencyList.end(), std::string{}, GraphPrinter{});
  }

  std::set<std::string> Graph::getNeighbors(const std::string& node) const
  {
    auto it = adjacencyList.find(node);
    if (it != adjacencyList.end())
    {
      return it->second;
    }
    return {};
  }

  CycleInfo Graph::findCycleNodes() const
  {
    std::map<std::string, int> color;
    std::map<std::string, std::string> parent;
    CycleInfo result{ false, {} };

    for (const auto& pair : adjacencyList)
    {
      color[pair.first] = 0;
    }

    for (const auto& pair : adjacencyList)
    {
      if (color[pair.first] == 0)
      {
        if (dfsFindCycleNodes(pair.first, color, parent, result))
        {
          result.hasCycle = true;
          break;
        }
      }
    }

    return result;
  }

  std::vector<std::string> Graph::topologicalSort() const
  {
    std::map<std::string, int> inDegree;
    for (const auto& pair : adjacencyList)
    {
      inDegree[pair.first] = 0;
    }

    for (const auto& pair : adjacencyList)
    {
      for (const auto& neighbor : pair.second)
      {
        inDegree[neighbor]++;
      }
    }

    std::queue<std::string> q;
    for (const auto& pair : inDegree)
    {
      if (pair.second == 0)
      {
        q.push(pair.first);
      }
    }

    std::vector<std::string> result;
    while (!q.empty())
    {
      std::string node = q.front();
      q.pop();
      result.push_back(node);

      for (const auto& neighbor : adjacencyList.at(node))
      {
        inDegree[neighbor]--;
        if (inDegree[neighbor] == 0)
        {
          q.push(neighbor);
        }
      }
    }

    return result;
  }

  std::vector<std::string> Graph::shortestPath(const std::string& start, const std::string& end) const
  {
    if (!searchNode(start) || !searchNode(end)) return {};

    std::map<std::string, std::string> predecessor;
    std::map<std::string, bool> visited;
    std::queue<std::string> q;

    for (const auto& pair : adjacencyList)
    {
      visited[pair.first] = false;
    }

    q.push(start);
    visited[start] = true;
    predecessor[start] = "";

    while (!q.empty())
    {
      std::string current = q.front();
      q.pop();

      if (current == end)
      {
        std::vector<std::string> path;
        std::string node = end;
        while (node != "")
        {
          path.push_back(node);
          node = predecessor[node];
        }
        std::reverse(path.begin(), path.end());
        return path;
      }

      for (const auto& neighbor : adjacencyList.at(current))
      {
        if (!visited[neighbor])
        {
          visited[neighbor] = true;
          predecessor[neighbor] = current;
          q.push(neighbor);
        }
      }
    }

    return {};
  }

  const std::map<std::string, std::set<std::string>>& Graph::getAdjacencyList() const
  {
    return adjacencyList;
  }

  bool Graph::dfsFindCycleNodes(const std::string& node, std::map<std::string, int>& color, std::map<std::string, std::string>& parent, CycleInfo& result) const
  {
    color[node] = 1;

    for (const auto& neighbor : adjacencyList.at(node))
    {
      if (color[neighbor] == 0)
      {
        parent[neighbor] = node;
        if (dfsFindCycleNodes(neighbor, color, parent, result))
        {
          result.cycleNodes.insert(node);
          return true;
        }
      }
      else if (color[neighbor] == 1)
      {
        result.cycleNodes.insert(neighbor);
        std::string current = node;
        while (current != neighbor)
        {
          result.cycleNodes.insert(current);
          current = parent[current];
        }
        return true;
      }
    }

    color[node] = 2;
    return false;
  }

  std::vector<std::string> Graph::extractCycleFromStack(const std::vector<std::string>& stack, const std::string& cycleEnd) const
  {
    std::vector<std::string> cycle;

    auto it = std::find(stack.begin(), stack.end(), cycleEnd);
    if (it != stack.end())
    {
      for (auto cycleIt = it; cycleIt != stack.end(); ++cycleIt)
      {
        cycle.push_back(*cycleIt);
      }
    }

    return cycle;
  }
}
