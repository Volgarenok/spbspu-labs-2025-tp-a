#include "graph.hpp"
#include <iostream>
#include <algorithm>
#include <functional>

namespace {
  struct DelimiterIn
  {
    char exp;
  };

  std::istream& operator>>(std::istream& in, DelimiterIn&& rhs)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != rhs.exp)) {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  bool is_neighbour_in_src(const std::vector< unsigned >& src, unsigned neighbour)
  {
    return std::find(src.begin(), src.end(), neighbour) != src.end();
  }
}

maslevtsov::Graph::Graph(const Graph& gr1, const Graph& gr2):
  Graph(gr1)
{
  for (auto i = gr2.adjacency_list_.begin(); i != gr2.adjacency_list_.end(); ++i) {
    try {
      if (i->second.empty()) {
        add_vertice(i->first);
      }
      for (auto j = i->second.begin(); j != i->second.end(); ++j) {
        add_edge(i->first, *j);
      }
    } catch (const std::invalid_argument&) {
      continue;
    } catch (...) {
      adjacency_list_.clear();
      throw;
    }
  }
}

maslevtsov::Graph::Graph(const Graph& src, const std::vector< unsigned >& vertices):
  Graph()
{
  for (unsigned vertice: vertices) {
    auto src_it = src.adjacency_list_.find(vertice);
    if (src_it != src.adjacency_list_.end()) {
      std::vector< unsigned > filtered_edges;
      auto is_nghbr_in_vrtcs = std::bind(is_neighbour_in_src, std::ref(vertices), std::placeholders::_1);
      std::copy_if(src_it->second.begin(), src_it->second.end(), std::back_inserter(filtered_edges), is_nghbr_in_vrtcs);
      adjacency_list_[vertice] = filtered_edges;
    } else {
      adjacency_list_.clear();
      throw std::invalid_argument("non-existing vertice");
    }
  }
}

const maslevtsov::Graph::adjacency_list_t& maslevtsov::Graph::get_adj_list() const
{
  return adjacency_list_;
}

void maslevtsov::Graph::add_vertice(unsigned vertice)
{
  if (adjacency_list_.find(vertice) != adjacency_list_.end()) {
    throw std::invalid_argument("vertice already exist");
  }
  adjacency_list_[vertice];
}

void maslevtsov::Graph::add_edge(unsigned vertice1, unsigned vertice2)
{
  auto vertice1_it = adjacency_list_.find(vertice1);
  if (vertice1_it != adjacency_list_.end()) {
    if (std::find(vertice1_it->second.begin(), vertice1_it->second.end(), vertice2) != vertice1_it->second.cend()) {
      throw std::invalid_argument("edge already exist");
    }
  }
  adjacency_list_[vertice1].push_back(vertice2);
  adjacency_list_[vertice2].push_back(vertice1);
}

void maslevtsov::Graph::delete_vertice(unsigned vertice)
{
  if (adjacency_list_.find(vertice) == adjacency_list_.end()) {
    throw std::invalid_argument("non-existing vertice");
  }
  auto neighbours_it = adjacency_list_.find(vertice)->second;
  for (auto i = neighbours_it.begin(); i != neighbours_it.end(); ++i) {
    auto neighbour_it = adjacency_list_.find(*i);
    neighbour_it->second.erase(std::find(neighbour_it->second.begin(), neighbour_it->second.end(), vertice));
  }
  adjacency_list_.erase(adjacency_list_.find(vertice));
}

void maslevtsov::Graph::delete_edge(unsigned vertice1, unsigned vertice2)
{
  auto vertice1_it = adjacency_list_.find(vertice1);
  if (vertice1_it == adjacency_list_.end()) {
    throw std::invalid_argument("non-existing edge");
  }
  if (std::find(vertice1_it->second.begin(), vertice1_it->second.end(), vertice2) == vertice1_it->second.cend()) {
    throw std::invalid_argument("non-existing edge");
  }
  vertice1_it->second.erase(std::find(vertice1_it->second.begin(), vertice1_it->second.end(), vertice2));
  auto vertice2_it = adjacency_list_.find(vertice2);
  vertice2_it->second.erase(std::find(vertice2_it->second.begin(), vertice2_it->second.end(), vertice1));
}

std::istream& maslevtsov::operator>>(std::istream& in, Graph& gr)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  Graph result;
  size_t vertice_count = 0;
  in >> vertice_count;
  for (size_t i = 0; i != vertice_count; ++i) {
    unsigned vertice = 0, neighbour = 0;
    in >> vertice >> DelimiterIn{':'};
    if (!in) {
      return in;
    }
    bool is_neighbour = false;
    while (in.peek() != '\n') {
      in >> neighbour;
      is_neighbour = true;
      try {
        result.add_edge(vertice, neighbour);
      } catch (const std::invalid_argument&) {
        continue;
      } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
    if (!is_neighbour) {
      try {
        result.add_vertice(vertice);
      } catch (const std::invalid_argument&) {
        continue;
      } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
      }
    }
  }
  if (in) {
    try {
      gr = result;
    } catch (...) {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::ostream& maslevtsov::operator<<(std::ostream& out, const Graph& gr)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  if (gr.adjacency_list_.empty()) {
    return out;
  }
  out << gr.adjacency_list_.size() << '\n';
  out << gr.adjacency_list_.cbegin()->first << " :";
  for (auto i = gr.adjacency_list_.cbegin()->second.cbegin(); i != gr.adjacency_list_.cbegin()->second.cend(); ++i) {
    out << ' ' << *i;
  }
  for (auto i = ++gr.adjacency_list_.cbegin(); i != gr.adjacency_list_.cend(); ++i) {
    out << '\n' << i->first << " :";
    if (!i->second.empty()) {
      for (auto j = i->second.cbegin(); j != i->second.cend(); ++j) {
        out << ' ' << *j;
      }
    }
  }
  return out;
}
