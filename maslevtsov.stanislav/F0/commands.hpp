#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "graph.hpp"

namespace maslevtsov {
  using graphs_t = std::unordered_map< std::string, Graph >;

  bool check_graphs_format(const std::istream& in);
  void print_help_manual(std::ostream& out);

  void save_graphs(graphs_t& graphs, std::istream& in, std::ostream& out);
  void open_graphs(graphs_t& graphs, std::istream& in, std::ostream& out);

  void add_graph(graphs_t& graphs, std::istream& in);
  void add_vertice(graphs_t& graphs, std::istream& in);
  void add_edge(graphs_t& graphs, std::istream& in);
  void print_graph(graphs_t& graphs, std::istream& in);
  void copy_graph(graphs_t& graphs, std::istream& in);
  void delete_graph(graphs_t& graphs, std::istream& in);
  void delete_vertice(graphs_t& graphs, std::istream& in);
  void delete_edge(graphs_t& graphs, std::istream& in);
  void union_graphs(graphs_t& graphs, std::istream& in);
  void create_subgraph(graphs_t& graphs, std::istream& in);

  void traverse_breadth_first(graphs_t& graphs, std::ostream& out, std::istream& in);
  void get_min_path(graphs_t& graphs, std::ostream& out, std::istream& in);
  void get_graph_width(graphs_t& graphs, std::ostream& out, std::istream& in);
  void get_graph_components(graphs_t& graphs, std::ostream& out, std::istream& in);
}

#endif
