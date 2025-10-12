#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "graph.hpp"

namespace maslevtsov {
  using graphs_t = std::unordered_map< std::string, Graph >;

  bool check_graphs_format(std::istream& in);
  void print_help_manual(std::ostream& out);

  void save_graphs(const graphs_t& graphs, std::istream& in);
  void open_graphs(graphs_t& graphs, std::istream& in, std::ostream& out);

  void add_graph(graphs_t& graphs, std::istream& in);
  void add_vertice(graphs_t& graphs, std::istream& in);
  void add_edge(graphs_t& graphs, std::istream& in);
  void print_graph(const graphs_t& graphs, std::istream& in, std::ostream& out);
  void copy_graph(graphs_t& graphs, std::istream& in);
  void delete_graph(graphs_t& graphs, std::istream& in);
  void delete_vertice(graphs_t& graphs, std::istream& in);
  void delete_edge(graphs_t& graphs, std::istream& in);
  void union_graphs(graphs_t& graphs, std::istream& in);
  void create_subgraph(graphs_t& graphs, std::istream& in);

  void traverse_breadth_first(const graphs_t& graphs, std::istream& in, std::ostream& out);
  void get_min_path(const graphs_t& graphs, std::istream& in, std::ostream& out);
  void get_graph_width(const graphs_t& graphs, std::istream& in, std::ostream& out);
  void get_graph_components(const graphs_t& graphs, std::istream& in, std::ostream& out);
}

#endif
