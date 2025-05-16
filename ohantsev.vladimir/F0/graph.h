#ifndef GRAPH_H
#define GRAPH_H
#include <queue>
#include <limits>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include "vector_hash.h"

namespace ohantsev
{
  template< class Key,
    class Hash = std::hash< Key >,
    class KeyEqual = std::equal_to< Key > >
  class Graph
  {
  public:
    using this_t = Graph;
    using ConnectionMap = std::unordered_map< Key, std::size_t, Hash, KeyEqual >;
    using GraphMap = std::unordered_map< Key, ConnectionMap >;
    struct Way;

    explicit Graph(std::size_t capacity = 10);
    ~Graph() noexcept = default;
    Graph(this_t&& rhs) noexcept = default;
    this_t& operator=(this_t&& rhs) noexcept = default;
    Graph(const this_t& rhs) = default;
    this_t& operator=(const this_t& rhs) = default;

    void clear() noexcept;
    std::size_t size() const noexcept;
    bool insert(const Key& key);
    bool link(const Key& from, const Key& to, std::size_t weight);
    const GraphMap& watch() const noexcept;
    const ConnectionMap& watch(const Key& key) const noexcept;
    bool contains(const Key& key) const;
    bool remove(const Key& key);
    bool removeForce(const Key& key);
    bool removeLink(const Key& first, const Key& second);
    void removeCycles();
    Way path(const Key& start, const Key& end) const;
    std::vector< Way > nPaths(const Key& start, const Key& end, std::size_t k) const;
    std::vector< Way > nPathsNoCycles(const Key& start, const Key& end, std::size_t k) const;

  private:
    GraphMap graph_;

    struct DSU;
    struct Edge;
    struct DijkstraContainers;
    class DijkstraPathFinder;
    template< bool AllowCycles >
    class NPathsFinder;

    std::vector< Edge > collectEdges() const;
  };

  template< class Key, class Hash, class KeyEqual >
  Graph< Key, Hash, KeyEqual >::Graph(size_t capacity)
  {
    graph_.reserve(capacity);
  }

  template< class Key, class Hash, class KeyEqual >
  void Graph< Key, Hash, KeyEqual >::clear() noexcept
  {
    graph_.clear();
  }

  template< class Key, class Hash, class KeyEqual >
  std::size_t Graph< Key, Hash, KeyEqual >::size() const noexcept
  {
    return graph_.size();
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::insert(const Key& key)
  {
    return graph_.emplace(key, ConnectionMap{}).second;
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::link(const Key& from, const Key& to, std::size_t weight)
  {
    if (from == to || weight == 0)
    {
      return false;
    }
    bool success = graph_[from].emplace(to, weight).second;
    success &= graph_[to].emplace(from, weight).second;
    return success;
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::watch() const noexcept -> const GraphMap&
  {
    return graph_;
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::watch(const Key& key) const noexcept -> const ConnectionMap&
  {
    return graph_.at(key);
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::contains(const Key& key) const
  {
    return graph_.find(key) != graph_.end();
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::remove(const Key& key)
  {
    auto iter = graph_.find(key);
    if (iter == graph_.end() || !iter->second.empty())
    {
      return false;
    }
    return graph_.erase(iter->first);
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::removeForce(const Key& key)
  {
    auto iter = graph_.find(key);
    if (iter == graph_.end())
    {
      return false;
    }
    for (auto& cnt: iter->second)
    {
      graph_.at(cnt.first).erase(key);
    }
    return graph_.erase(key);
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::removeLink(const Key& first, const Key& second)
  {
    try
    {
      return graph_.at(first).erase(second) && graph_.at(second).erase(first);
    }
    catch (const std::out_of_range&)
    {
      return false;
    }
  }

  template< class Key, class Hash, class KeyEqual >
  struct Graph< Key, Hash, KeyEqual >::DSU
  {
    std::unordered_map< Key, Key > parent;
    std::unordered_map< Key, std::size_t > rank;

    explicit DSU(const Graph& graph);
    Key findRoot(const Key& key);
    void unionSets(const Key& first, const Key& second);
  };

  template< class Key, class Hash, class KeyEqual >
  Graph< Key, Hash, KeyEqual >::DSU::DSU(const Graph& graph)
  {
    for (const auto& pair: graph.graph_)
    {
      const Key& key = pair.first;
      parent.emplace(key, key);
      rank.emplace(key, 0);
    }
  }

  template< class Key, class Hash, class KeyEqual >
  Key Graph< Key, Hash, KeyEqual >::DSU::findRoot(const Key& key)
  {
    if (parent[key] != key)
    {
      parent[key] = findRoot(parent[key]);
    }
    return parent[key];
  }

  template< class Key, class Hash, class KeyEqual >
  void Graph< Key, Hash, KeyEqual >::DSU::unionSets(const Key& first, const Key& second)
  {
    Key rootFirst = findRoot(first);
    Key rootSecond = findRoot(second);
    if (KeyEqual{}(rootFirst, rootSecond))
    {
      return;
    }
    if (rank[rootFirst] < rank[rootSecond])
    {
      parent[rootFirst] = rootSecond;
    }
    else
    {
      parent[rootSecond] = rootFirst;
      if (rank[rootFirst] == rank[rootSecond])
      {
        ++rank[rootFirst];
      }
    }
  }

  template< class Key, class Hash, class KeyEqual >
  struct Graph< Key, Hash, KeyEqual >::Edge
  {
    Key from_;
    Key to_;
    std::size_t weight_;

    Edge(Key from, Key to, std::size_t weight);
    bool operator<(const Edge& rhs) const;
  };

  template< class Key, class Hash, class KeyEqual >
  Graph< Key, Hash, KeyEqual >::Edge::Edge(Key from, Key to, std::size_t weight):
    from_(from),
    to_(to),
    weight_(weight)
  {}

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::Edge::operator<(const Edge& rhs) const
  {
    return weight_ < rhs.weight_;
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::collectEdges() const -> std::vector< Edge >
  {
    std::vector< Edge > edges;
    edges.reserve(graph_.size() * 2);
    for (const auto& pair: graph_)
    {
      const Key& from = pair.first;
      for (const auto& connection: pair.second)
      {
        const Key& to = connection.first;
        if (from < to)
        {
          edges.emplace_back(from, to, connection.second);
        }
      }
    }
    std::sort(edges.begin(), edges.end());
    return edges;
  }

  template< class Key, class Hash, class KeyEqual >
  void Graph< Key, Hash, KeyEqual >::removeCycles()
  {
    if (graph_.size() < 3)
    {
      return;
    }
    auto edges = collectEdges();
    DSU dsu(*this);
    for (const auto& edge: edges)
    {
      const Key& from = edge.from_;
      const Key& to = edge.to_;

      if (dsu.findRoot(from) != dsu.findRoot(to))
      {
        dsu.unionSets(from, to);
      }
      else
      {
        removeLink(from, to);
      }
    }
  }

  template< class Key, class Hash, class KeyEqual >
  struct Graph< Key, Hash, KeyEqual >::Way
  {
    std::vector< Key > steps_;
    std::size_t length_{ 0 };

    bool operator>(const Way& rhs) const;
    bool operator==(const Way& rhs) const;
    bool operator!=(const Way& rhs) const;
  };

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::Way::operator>(const Way& rhs) const
  {
    return length_ > rhs.length_;
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::Way::operator==(const Way& rhs) const
  {
    if (steps_.size() != rhs.steps_.size())
    {
      return false;
    }
    KeyEqual equal;
    for (std::size_t i = 0; i < steps_.size(); ++i)
    {
      if (!equal(steps_[i], rhs.steps_[i]))
      {
        return false;
      }
    }
    return true;
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::Way::operator!=(const Way& rhs) const
  {
    return !(*this == rhs);
  }

  template< class Key, class Hash, class KeyEqual >
  class Graph< Key, Hash, KeyEqual >::DijkstraPathFinder
  {
  public:
    DijkstraPathFinder(const Graph& graph, const Key& start, const Key& end);
    Way operator()();

  private:
    const Key& start_;
    const Key& end_;
    const Graph& graph_;
    std::unordered_map< Key, std::size_t > distances_;
    using Step = std::pair< Key, std::size_t >;
    std::unordered_map< Key, Step > previous_;
    using Distances = std::pair< std::size_t, Key >;
    std::priority_queue< Distances, std::vector< Distances >, std::greater< Distances > > queue_;

    void pushNeighbors(const Key& key);
    Way constructPath();
    Way releasePath();
  };

  template< class Key, class Hash, class KeyEqual >
  Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::DijkstraPathFinder(const Graph& graph, const Key& start,
                                                                       const Key& end):
    start_(start),
    end_(end),
    graph_(graph)
  {
    for (const auto& pair: graph.graph_)
    {
      distances_[pair.first] = std::numeric_limits< std::size_t >::max();
    }
    distances_[start] = 0;
    queue_.emplace(0, start);
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::operator()() -> Way
  {
    return constructPath();
  }

  template< class Key, class Hash, class KeyEqual >
  void Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::pushNeighbors(const Key& key)
  {
    for (const auto& cnt: graph_.graph_.at(key))
    {
      const Key& neighbor = cnt.first;
      auto weight = cnt.second;
      auto newDistance = distances_[key] + weight;
      if (newDistance < distances_[neighbor])
      {
        distances_[neighbor] = newDistance;
        previous_[neighbor] = { key, weight };
        queue_.emplace(newDistance, neighbor);
      }
    }
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::constructPath() -> Way
  {
    while (!queue_.empty())
    {
      auto current = queue_.top();
      queue_.pop();
      auto currentDistance = current.first;
      const Key& currentKey = current.second;
      if (currentDistance > distances_[currentKey])
      {
        continue;
      }
      if (currentKey == end_)
      {
        break;
      }
      pushNeighbors(currentKey);
    }
    return releasePath();
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::releasePath() -> Way
  {
    if (distances_[end_] == std::numeric_limits< std::size_t >::max())
    {
      return {};
    }
    Way path;
    Key current = end_;
    while (current != start_)
    {
      const auto& prevStep = previous_[current];
      const Key& prev = prevStep.first;
      auto weight = prevStep.second;
      path.steps_.push_back(current);
      path.length_ += weight;
      current = prev;
    }
    path.steps_.push_back(start_);
    std::reverse(path.steps_.begin(), path.steps_.end());
    return path;
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::path(const Key& start, const Key& end) const -> Way
  {
    if (!contains(start) || !contains(end))
    {
      throw std::invalid_argument("Key not found");
    }
    return DijkstraPathFinder{ *this, start, end }();
  }

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  class Graph< Key, Hash, KeyEqual >::NPathsFinder
  {
  public:
    NPathsFinder(const Graph& graph, const Key& start, const Key& end);
    std::vector< Way > operator()(std::size_t k);

  private:
    const Graph& graph_;
    const Key& start_;
    const Key& end_;
    std::vector< Way > result;
    std::priority_queue< Way, std::vector< Way >, std::greater< Way > > candidates;
    std::unordered_set< std::vector< Key >, VectorHash< Key, Hash > > visited;

    void finishWayProcessing(const Way& current);
    template< bool AC = AllowCycles >
    std::enable_if_t< !AC >
    connectNeighbor(const Way& current, const Key& neighbor, std::size_t weight);
    template< bool AC = AllowCycles >
    std::enable_if_t< AC >
    connectNeighbor(const Way& current, const Key& neighbor, std::size_t weight);
    void processNextPath();
  };

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  Graph< Key, Hash, KeyEqual >::NPathsFinder< AllowCycles >::NPathsFinder(const Graph& graph,
                                                                          const Key& start, const Key& end):
    graph_(graph),
    start_(start),
    end_(end)
  {
    Way initialPath;
    initialPath.steps_.push_back(start_);
    initialPath.length_ = 0;
    candidates.push(initialPath);
  }

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  auto Graph< Key, Hash, KeyEqual >::NPathsFinder< AllowCycles >::operator()(std::size_t k) -> std::vector< Way >
  {
    if (!graph_.contains(start_) || !graph_.contains(end_))
    {
      throw std::invalid_argument("Key not found");
    }
    while (!candidates.empty() && result.size() < k)
    {
      processNextPath();
    }
    return result;
  }

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  void Graph< Key, Hash, KeyEqual >::NPathsFinder< AllowCycles >::finishWayProcessing(const Way& current)
  {
    if (visited.find(current.steps_) == visited.end())
    {
      result.push_back(current);
      visited.insert(current.steps_);
    }
  }

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  template< bool AC >
  std::enable_if_t< !AC >
  Graph< Key, Hash, KeyEqual >::NPathsFinder< AllowCycles >::connectNeighbor(const Way& current,
                                                                             const Key& neighbor, std::size_t weight)
  {
    if (std::find(current.steps_.begin(), current.steps_.end(), neighbor) == current.steps_.end())
    {
      connectNeighbor< true >(current, neighbor, weight);
    }
  }

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  template< bool AC >
  std::enable_if_t< AC >
  Graph< Key, Hash, KeyEqual >::NPathsFinder< AllowCycles >::connectNeighbor(const Way& current,
                                                                             const Key& neighbor, std::size_t weight)
  {
    Way newPath = current;
    newPath.steps_.push_back(neighbor);
    newPath.length_ += weight;
    if (visited.find(newPath.steps_) == visited.end())
    {
      candidates.push(newPath);
    }
  }

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  void Graph< Key, Hash, KeyEqual >::NPathsFinder< AllowCycles >::processNextPath()
  {
    Way current = candidates.top();
    candidates.pop();
    Key lastKey = current.steps_.back();
    if (lastKey == end_)
    {
      finishWayProcessing(current);
      return;
    }
    for (const auto& neighbor: graph_.graph_.at(lastKey))
    {
      connectNeighbor< >(current, neighbor.first, neighbor.second);
    }
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::nPaths(const Key& start, const Key& end, std::size_t k) const -> std::vector< Way >
  {
    return NPathsFinder< true >{ *this, start, end }(k);
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::nPathsNoCycles(const Key& start, const Key& end,
                                                    std::size_t k) const -> std::vector< Way >
  {
    return NPathsFinder< false >{ *this, start, end }(k);
  }
}
#endif
