#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <ostream>
#include <iostream>
#include <functional>
#include <queue>
#include <numeric>
#include <algorithm>

namespace klimova {
  using namespace std::placeholders;

  template< typename T >
  class Graph {
  public:
    using VertexList = std::vector<T>;
    using AdjacencyList = std::vector<std::vector<size_t>>;
    using VertexMap = std::unordered_map<T, size_t>;
    using VisitedList = std::vector<bool>;
    using Path = std::vector<T>;

    Graph() = default;
    void addVertex(const T& vertex);
    void addEdge(const T& src, const T& dest);
    void removeVertex(const T& vertex);
    void removeEdge(const T& src, const T& dest);
    void printGraph() const;
    bool checkConnectivity() const;
    size_t countVertices() const;
    size_t countEdges() const;
    void findNeighbors(const T& vertex) const;
    int degreeOfVertex(const T& vertex) const;
    void clear();
    void findLongestPath(const T& startVertex, const T& endVertex) const;
    void findShortestPath(const T& startVertex, const T& endVertex) const;

  private:
    VertexList vertices;
    VertexMap vertexMap;
    AdjacencyList adjList;

    void dfsUtil(size_t v, VisitedList& visited) const;
    bool findLongestPathUtil(size_t u, size_t endIdx, VisitedList& visited, Path& path, Path& longestPath) const;
    size_t getVertexIndex(const T& vertex) const;
  };

  template < typename T >
  void Graph< T >::addVertex(const T& vertex)
  {
    if (vertexMap.find(vertex) == vertexMap.end()) {
      vertexMap[vertex] = vertices.size();
      vertices.push_back(vertex);
      adjList.emplace_back();
    }
  }

  template < typename T >
  size_t Graph< T >::getVertexIndex(const T& vertex) const
  {
    auto it = vertexMap.find(vertex);
    if (it == vertexMap.end()) {
      throw std::out_of_range("Vertex not found");
    }
    return it->second;
  }

  template < typename T >
  void Graph< T >::addEdge(const T& src, const T& dest)
  {
    size_t srcIdx = getVertexIndex(src);
    size_t destIdx = getVertexIndex(dest);
    if (std::find(adjList[srcIdx].begin(), adjList[srcIdx].end(), destIdx) == adjList[srcIdx].end()) {
      adjList[srcIdx].push_back(destIdx);
      adjList[destIdx].push_back(srcIdx);
    }
  }

  template < typename T >
  void Graph< T >::removeEdge(const T& src, const T& dest)
  {
    size_t srcIdx = getVertexIndex(src);
    size_t destIdx = getVertexIndex(dest);
    auto& srcNeighbors = adjList[srcIdx];
    srcNeighbors.erase(std::remove(srcNeighbors.begin(), srcNeighbors.end(), destIdx), srcNeighbors.end());

    auto& destNeighbors = adjList[destIdx];
    destNeighbors.erase(std::remove(destNeighbors.begin(), destNeighbors.end(), srcIdx), destNeighbors.end());
  }

  template < typename T >
  void Graph< T >::removeVertex(const T& vertex)
  {
    size_t idx = getVertexIndex(vertex);
    for (size_t neighbor : adjList[idx]) {
      auto& neighbors = adjList[neighbor];
      neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), idx), neighbors.end());
    }

    vertices.erase(vertices.begin() + idx);
    adjList.erase(adjList.begin() + idx);
    vertexMap.erase(vertex);

    for (size_t i = 0; i < vertices.size(); ++i) {
      vertexMap[vertices[i]] = i;
      for (size_t& neighbor : adjList[i]) {
        if (neighbor > idx) {
          neighbor--;
        }
      }
    }
  }

  template < typename T >
  void Graph< T >::printGraph() const
  {
    std::cout << "Vertices:\n";
    for (const auto& v : vertices) {
      std::cout << v << "\n";
    }
    std::cout << "Edges:\n";
    for (size_t i = 0; i < adjList.size(); ++i) {
      for (size_t neighbor : adjList[i]) {
        if (i < neighbor) {
          std::cout << vertices[i] << " -- " << vertices[neighbor] << "\n";
        }
      }
    }
  }

  template < typename T >
  bool Graph< T >::checkConnectivity() const
  {
    if (vertices.empty()) {
      return true;
    }
    std::vector< bool > visited(vertices.size(), false);
    dfsUtil(0, visited);
    auto binder = std::bind(std::equal_to< bool >{}, _1, true);
    return std::all_of(visited.begin(), visited.end(), binder);
  }

  template < typename T >
  void Graph< T >::dfsUtil(size_t v, VisitedList& visited) const
  {
    visited[v] = true;
    for (size_t neighbor : adjList[v]) {
      if (!visited[neighbor]) {
        dfsUtil(neighbor, visited);
      }
    }
  }

  template < typename T >
  size_t Graph< T >::countVertices() const
  {
    return vertices.size();
  }

  template < typename T >
  size_t Graph< T >::countEdges() const
  {
    struct SumNeighbors {
      size_t operator()(size_t sum, const std::vector<size_t>& neighbors) const {
        return sum + neighbors.size();
      }
    };

    return std::accumulate(adjList.begin(), adjList.end(), 0, SumNeighbors{}) / 2;
  }

  template < typename T >
  void Graph< T >::findNeighbors(const T& vertex) const
  {
    try {
      size_t idx = getVertexIndex(vertex);
      for (size_t neighbor : adjList[idx]) {
        std::cout << vertices[neighbor] << " ";
      }
      std::cout << std::endl;
    } catch (const std::out_of_range&) {
      std::cout << "Vertex not found." << std::endl;
    }
  }

  template < typename T >
  int Graph< T >::degreeOfVertex(const T& vertex) const
  {
    try {
      size_t idx = getVertexIndex(vertex);
      return static_cast< int >(adjList[idx].size());
    } catch (const std::out_of_range&) {
      std::cerr << "Vertex not found." << std::endl;
      return -1;
    }
  }

  template < typename T >
  void Graph< T >::findShortestPath(const T& startVertex, const T& endVertex) const
  {
    try {
      size_t startIdx = getVertexIndex(startVertex);
      size_t endIdx = getVertexIndex(endVertex);

      VisitedList visited(vertices.size(), false);
      std::vector< int > parent(vertices.size(), -1);
      std::queue< size_t > queue;

      visited[startIdx] = true;
      queue.push(startIdx);

      while (!queue.empty()) {
        size_t curr = queue.front();
        queue.pop();

        if (curr == endIdx) {
          break;
        }
        for (size_t neighbor : adjList[curr]) {
          if (!visited[neighbor]) {
            visited[neighbor] = true;
            parent[neighbor] = curr;
            queue.push(neighbor);
          }
        }
      }

      if (parent[endIdx] == -1 && startIdx != endIdx) {
        std::cout << "Path from " << startVertex << " to " << endVertex << " not found\n";
        return;
      }

      std::vector< T > path;
      for (int v = endIdx; v != -1; v = parent[v]) {
        path.push_back(vertices[v]);
      }

      std::cout << "Shortest path from " << startVertex << " to " << endVertex << ": ";
      for (auto it = path.rbegin(); it != path.rend(); ++it) {
        std::cout << *it << " ";
      }
      std::cout << std::endl;
    } catch (const std::out_of_range&) {
      std::cout << "One of the vertices not found." << std::endl;
    }
  }

  template < typename T >
  void Graph< T >::clear()
  {
    vertices.clear();
    vertexMap.clear();
    adjList.clear();
  }

  template < typename T >
  bool Graph< T >::findLongestPathUtil(size_t u, size_t endIdx, VisitedList& visited, Path& path, Path& longestPath) const
  {
    visited[u] = true;
    path.push_back(vertices[u]);

    if (u == endIdx) {
      if (path.size() > longestPath.size()) {
        longestPath = path;
      }
    } else {
      for (size_t neighbor : adjList[u]) {
        if (!visited[neighbor]) {
          findLongestPathUtil(neighbor, endIdx, visited, path, longestPath);
        }
      }
    }

    path.pop_back();
    visited[u] = false;
    return !longestPath.empty();
  }

  template < typename T >
  void Graph< T >::findLongestPath(const T& startVertex, const T& endVertex) const
  {
    try {
      size_t startIdx = getVertexIndex(startVertex);
      size_t endIdx = getVertexIndex(endVertex);

      VisitedList visited(vertices.size(), false);
      Path path, longestPath;

      if (findLongestPathUtil(startIdx, endIdx, visited, path, longestPath)) {
        std::cout << "Longest path from " << startVertex << " to " << endVertex << ": ";
        for (const auto& vertex : longestPath) {
          std::cout << vertex << " ";
        }
        std::cout << std::endl;
      } else {
        std::cout << "Path from " << startVertex << " to " << endVertex << " not found\n";
      }
    } catch (const std::out_of_range&) {
      std::cout << "One of the vertices not found." << std::endl;
    }
  }
}
#endif
