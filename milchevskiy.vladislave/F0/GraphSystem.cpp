#include "GraphSystem.hpp"
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

namespace milchevskiy
{
  void GraphSystem::create(const std::string& name)
  {
    if (graphs.find(name) != graphs.end())
    {
      std::cout << "<GRAPH ALREADY EXISTS>" << std::endl;
      return;
    }
    graphs[name] = Graph();
    std::cout << "Граф \"" << name << "\" создан" << std::endl;
  }

  void GraphSystem::isEmpty(const std::string& name)
  {
    if (graphs.find(name) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    if (graphs[name].isEmpty())
    {
      std::cout << "<GRAPH IS EMPTY>" << std::endl;
    }
    else
    {
      std::cout << "<GRAPH IS NOT EMPTY>" << std::endl;
    }
  }

  void GraphSystem::searchNode(const std::string& graphName, const std::string& nodeName)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    if (graphs[graphName].searchNode(nodeName))
    {
      std::cout << "<NODE FOUND>" << std::endl;
    }
    else
    {
      std::cout << "<NODE NOT FOUND>" << std::endl;
    }
  }

  void GraphSystem::searchArc(const std::string& graphName, const std::string& node1, const std::string& node2)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    if (!graphs[graphName].searchNode(node1) || !graphs[graphName].searchNode(node2))
    {
      std::cout << "<NODE NOT FOUND>" << std::endl;
      return;
    }
    if (graphs[graphName].searchArc(node1, node2))
    {
      std::cout << "<ARC FOUND>" << std::endl;
    }
    else
    {
      std::cout << "<ARC NOT FOUND>" << std::endl;
    }
  }

  void GraphSystem::insertNode(const std::string& graphName, const std::string& nodeName)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    if (graphs[graphName].insertNode(nodeName))
    {
      std::cout << "Узел \"" << nodeName << "\" добавлен в граф \"" << graphName << "\"" << std::endl;
    }
    else
    {
      std::cout << "<NODE ALREADY EXISTS>" << std::endl;
    }
  }

  void GraphSystem::insertArc(const std::string& graphName, const std::string& node1, const std::string& node2)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    if (!graphs[graphName].searchNode(node1) || !graphs[graphName].searchNode(node2))
    {
      std::cout << "<NODE NOT FOUND>" << std::endl;
      return;
    }
    if (graphs[graphName].insertArc(node1, node2))
    {
      std::cout << "Дуга \"" << node1 << " -> " << node2 << "\" добавлена в граф \"" << graphName << "\"" << std::endl;
    }
    else
    {
      std::cout << "<ARC ALREADY EXISTS>" << std::endl;
    }
  }

  void GraphSystem::deleteNode(const std::string& graphName, const std::string& nodeName)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    if (graphs[graphName].deleteNode(nodeName))
    {
      std::cout << "Узел \"" << nodeName << "\" удален из графа \"" << graphName << "\"" << std::endl;
    }
    else
    {
      std::cout << "<NODE NOT FOUND>" << std::endl;
    }
  }

  void GraphSystem::print(const std::string& graphName)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    std::cout << graphs[graphName].print();
  }

  void GraphSystem::clear(const std::string& graphName)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    graphs[graphName].clear();
    std::cout << "Граф \"" << graphName << "\" очищен" << std::endl;
  }

  void GraphSystem::isCyclical(const std::string& graphName, const std::string& newGraphName)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    if (graphs.find(newGraphName) != graphs.end())
    {
      std::cout << "<GRAPH ALREADY EXISTS>" << std::endl;
      return;
    }

    const auto& graph = graphs[graphName];
    auto cycleInfo = graph.findCycleNodes();

    if (cycleInfo.hasCycle)
    {
      Graph cycleGraph;

      for (const auto& node : cycleInfo.cycleNodes)
      {
        cycleGraph.insertNode(node);
      }

      for (const auto& node : cycleInfo.cycleNodes)
      {
        for (const auto& neighbor : graph.getNeighbors(node))
        {
          if (cycleInfo.cycleNodes.count(neighbor))
          {
            cycleGraph.insertArc(node, neighbor);
          }
        }
      }

      graphs[newGraphName] = cycleGraph;
      std::cout << "<" << newGraphName << "><GRAPH IS CYCLICAL>" << std::endl;
    }
    else
    {
      std::cout << "<GRAPH IS ACYCLICAL>" << std::endl;
    }
  }

  void GraphSystem::shortestPath(const std::string& graphName, const std::string& node1, const std::string& node2, const std::string& resultGraphName)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    if (graphs.find(resultGraphName) != graphs.end())
    {
      std::cout << "<GRAPH ALREADY EXISTS>" << std::endl;
      return;
    }
    if (!graphs[graphName].searchNode(node1) || !graphs[graphName].searchNode(node2))
    {
      std::cout << "<NODE DOES NOT EXIST>" << std::endl;
      return;
    }

    std::vector<std::string> path = graphs[graphName].shortestPath(node1, node2);
    if (path.empty())
    {
      std::cout << "<NO PATH EXISTS>" << std::endl;
      return;
    }

    Graph resultGraph;
    for (const auto& node : path)
    {
      resultGraph.insertNode(node);
    }

    for (size_t i = 0; i < path.size() - 1; i++)
    {
      resultGraph.insertArc(path[i], path[i + 1]);
    }

    graphs[resultGraphName] = resultGraph;
    std::cout << "Граф \"" << resultGraphName << "\", содержащий кратчайший путь" << std::endl;
  }

  void GraphSystem::topologicalSort(const std::string& graphName, const std::string& newGraphName)
  {
    if (graphs.find(graphName) == graphs.end())
    {
      std::cout << "<GRAPH DOES NOT EXIST>" << std::endl;
      return;
    }
    if (graphs.find(newGraphName) != graphs.end())
    {
      std::cout << "<GRAPH ALREADY EXISTS>" << std::endl;
      return;
    }

    const auto& graph = graphs[graphName];
    auto cycleInfo = graph.findCycleNodes();

    if (cycleInfo.hasCycle)
    {
      std::cout << "<GRAPH IS CYCLICAL>" << std::endl;
      return;
    }

    std::vector<std::string> sorted = graph.topologicalSort();

    std::map<std::string, std::string> renameMap;
    for (size_t i = 0; i < sorted.size(); i++)
    {
      renameMap[sorted[i]] = std::to_string(i + 1);
    }

    Graph sortedGraph;

    for (const auto& oldName : sorted)
    {
      sortedGraph.insertNode(renameMap[oldName]);
    }

    const auto& adjList = graph.getAdjacencyList();
    for (const auto& pair : adjList)
    {
      const std::string& oldFrom = pair.first;
      const std::string& newFrom = renameMap[oldFrom];

      for (const auto& oldTo : pair.second)
      {
        const std::string& newTo = renameMap[oldTo];
        sortedGraph.insertArc(newFrom, newTo);
      }
    }

    graphs[newGraphName] = sortedGraph;
    std::cout << "<" << newGraphName << ">" << std::endl;
  }
}
