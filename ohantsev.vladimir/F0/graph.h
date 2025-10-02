#ifndef GRAPH_H
#define GRAPH_H
#include <queue>
#include <limits>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

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
    Graph(const this_t& rhs) = default;
    Graph(this_t&& rhs) noexcept = default;
    ~Graph() noexcept = default;
    this_t& operator=(const this_t& rhs) = default;
    this_t& operator=(this_t&& rhs) noexcept = default;

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
    template <bool AllowCycles >
    std::vector< Way > nPaths(const Key& start, const Key& end, std::size_t k) const;

  private:
    struct DSU;
    struct Edge;
    struct MakeEdge;
    struct EdgeCollector;
    struct EdgeProcessor;
    struct ConnectionRemover;
    class DijkstraPathFinder;
    template< bool AllowCycles >
    class NPathsFinder;

    GraphMap graph_;

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
  struct Graph< Key, Hash, KeyEqual >::ConnectionRemover
  {
    Graph& graph;
    const Key& toRemove;

    void operator()(const typename ConnectionMap::value_type& cnt) const
    {
      graph.graph_.at(cnt.first).erase(toRemove);
    }
  };

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::removeForce(const Key& key)
  {
    auto iter = graph_.find(key);
    if (iter == graph_.end())
    {
      return false;
    }
    auto& cnts = iter->second;
    std::for_each(cnts.begin(), cnts.end(), ConnectionRemover{ *this, key });
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
    static std::pair< Key, Key > InitParent(const typename GraphMap::value_type& pair);
    static std::pair< Key, std::size_t > InitRank(const typename GraphMap::value_type& pair);
  };

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::DSU::InitParent(const typename GraphMap::value_type& pair) -> std::pair< Key, Key >
  {
    return std::make_pair(pair.first, pair.first);
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::DSU::InitRank(const typename GraphMap::value_type& pair) -> std::pair< Key, std::size_t >
  {
    return std::make_pair(pair.first, 0);
  }

  template< class Key, class Hash, class KeyEqual >
  Graph< Key, Hash, KeyEqual >::DSU::DSU(const Graph& graph)
  {
    std::transform(graph.graph_.cbegin(), graph.graph_.cend(), std::inserter(parent, parent.end()), InitParent);
    std::transform(graph.graph_.cbegin(), graph.graph_.cend(), std::inserter(rank, rank.end()), InitRank);
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
    static bool uniformSelection(const Edge& edge);
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
  bool Graph< Key, Hash, KeyEqual >::Edge::uniformSelection(const Edge& edge)
  {
    return edge.from_ < edge.to_;
  }

  template< class Key, class Hash, class KeyEqual >
  struct Graph< Key, Hash, KeyEqual >::MakeEdge
   {
    const Key& from;

    auto operator()(const typename ConnectionMap::value_type& cnt) const -> Edge
    {
        return Edge{ from, cnt.first, cnt.second };
    }
  };

  template< class Key, class Hash, class KeyEqual >
  struct Graph< Key, Hash, KeyEqual >::EdgeCollector
  {
    std::vector< Edge >& edges;

    void operator()(const typename GraphMap::value_type& pair)
    {
      const Key& from = pair.first;
      const auto& to = pair.second;
      std::vector< Edge > temp;
      temp.reserve(to.size());
      std::transform(to.cbegin(), to.cend(), std::back_inserter(temp), MakeEdge{ from });
      std::copy_if(temp.cbegin(), temp.cend(), std::back_inserter(edges), Edge::uniformSelection);
    }
  };

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::collectEdges() const -> std::vector< Edge >
  {
    std::vector< Edge > edges;
    static constexpr std::size_t intuitiveEdgeVertexCoefficient = 2;
    edges.reserve(graph_.size() * intuitiveEdgeVertexCoefficient);
    std::for_each(graph_.begin(), graph_.end(), EdgeCollector{ edges });
    std::sort(edges.begin(), edges.end());
    return edges;
  }

  template< class Key, class Hash, class KeyEqual >
  struct Graph< Key, Hash, KeyEqual >::EdgeProcessor
  {
    DSU& dsu;
    Graph& graph;

    void operator()(const Edge& edge)
    {
      const Key& from = edge.from_;
      const Key& to = edge.to_;
      if (dsu.findRoot(from) != dsu.findRoot(to))
      {
        dsu.unionSets(from, to);
      }
      else
      {
        graph.removeLink(from, to);
      }
    }
  };

  template< class Key, class Hash, class KeyEqual >
  void Graph< Key, Hash, KeyEqual >::removeCycles()
  {
    if (graph_.size() < 3)
    {
      return;
    }
    auto edges = collectEdges();
    DSU dsu(*this);
    std::for_each(edges.begin(), edges.end(), EdgeProcessor{ dsu, *this });
  }

  template< class Key, class Hash, class KeyEqual >
  struct Graph< Key, Hash, KeyEqual >::Way
  {
    std::vector< Key > steps_;
    std::size_t length_{ 0 };

    bool operator>(const Way& rhs) const noexcept;
    bool operator==(const Way& rhs) const noexcept;
    bool operator!=(const Way& rhs) const noexcept;
  };

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::Way::operator>(const Way& rhs) const noexcept
  {
    return length_ > rhs.length_;
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::Way::operator==(const Way& rhs) const noexcept
  {
    if (steps_.size() != rhs.steps_.size())
    {
      return false;
    }
    return std::equal(steps_.begin(), steps_.end(), rhs.steps_.begin(), KeyEqual{});
  }

  template< class Key, class Hash, class KeyEqual >
  bool Graph< Key, Hash, KeyEqual >::Way::operator!=(const Way& rhs) const noexcept
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
    using Step = std::pair< Key, std::size_t >;
    using Distances = std::pair< std::size_t, Key >;
    struct InitDistance;
    struct NeighborProcessor;

    const Key& start_;
    const Key& end_;
    const Graph& graph_;
    std::unordered_map< Key, std::size_t > distances_;
    std::unordered_map< Key, Step > previous_;
    std::priority_queue< Distances, std::vector< Distances >, std::greater< Distances > > queue_;

    void pushNeighbors(const Key& key);
    Way constructPath();
    Way releasePath();
  };

  template< class Key, class Hash, class KeyEqual >
  struct Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::InitDistance
  {
    typename std::pair< Key, std::size_t > operator()(const typename GraphMap::value_type& pair)
    {
      return std::make_pair(pair.first, std::numeric_limits< std::size_t >::max());
    }
  };

  template< class Key, class Hash, class KeyEqual >
  Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::DijkstraPathFinder(const Graph& graph, const Key& start,
                                                                       const Key& end):
    start_(start),
    end_(end),
    graph_(graph)
  {
    std::transform(graph.graph_.cbegin(), graph.graph_.cend(), std::inserter(distances_, distances_.end()), InitDistance{});
    distances_[start] = 0;
    queue_.emplace(0, start);
  }

  template< class Key, class Hash, class KeyEqual >
  auto Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::operator()() -> Way
  {
    return constructPath();
  }

  template< class Key, class Hash, class KeyEqual >
  struct Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::NeighborProcessor
  {
    DijkstraPathFinder& finder;
    const Key& currentKey;
    std::size_t currentDistance;

    void operator()(const typename ConnectionMap::value_type& cnt)
    {
      const Key& neighbor = cnt.first;
      auto weight = cnt.second;
      auto newDistance = currentDistance + weight;

      if (newDistance < finder.distances_[neighbor])
      {
        finder.distances_[neighbor] = newDistance;
        finder.previous_[neighbor] = { currentKey, weight };
        finder.queue_.emplace(newDistance, neighbor);
      }
    }
  };

  template< class Key, class Hash, class KeyEqual >
  void Graph< Key, Hash, KeyEqual >::DijkstraPathFinder::pushNeighbors(const Key& key)
  {
    const auto& connections = graph_.graph_.at(key);
    std::for_each(connections.begin(), connections.end(), NeighborProcessor{ *this, key, distances_[key] });
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
    struct NeighborConnector;

    const Graph& graph_;
    const Key& start_;
    const Key& end_;
    std::vector< Way > result;
    std::priority_queue< Way, std::vector< Way >, std::greater< Way > > candidates;

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
    candidates.push(newPath);
  }

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  struct Graph< Key, Hash, KeyEqual >::NPathsFinder< AllowCycles >::NeighborConnector
  {
    NPathsFinder& finder;
    const Way& current;

    void operator()(const typename ConnectionMap::value_type& neighbor)
    {
      finder.connectNeighbor< AllowCycles >(current, neighbor.first, neighbor.second);
    }
  };

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  void Graph< Key, Hash, KeyEqual >::NPathsFinder< AllowCycles >::processNextPath()
  {
    Way current = candidates.top();
    candidates.pop();
    Key lastKey = current.steps_.back();
    if (lastKey == end_)
    {
      result.push_back(current);
      return;
    }
    const auto& neighbors = graph_.graph_.at(lastKey);
    std::for_each(neighbors.begin(), neighbors.end(), NeighborConnector{ *this, current });
  }

  template< class Key, class Hash, class KeyEqual >
  template< bool AllowCycles >
  auto Graph< Key, Hash, KeyEqual >::nPaths(const Key& start, const Key& end, std::size_t k) const -> std::vector< Way >
  {
    if (path(start, end).length_ == 0)
    {
      return {};
    }
    return NPathsFinder< AllowCycles >{ *this, start, end }(k);
  }
}
#endif
