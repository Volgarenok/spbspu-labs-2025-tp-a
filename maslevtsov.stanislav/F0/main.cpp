#include <iostream>
#include <fstream>
#include <cstring>
#include <functional>
#include <limits>
#include "commands.hpp"

int main(int argc, char** argv)
{
  using namespace maslevtsov;

  if (argc < 2 || argc > 4) {
    std::cerr << "Error: invalid parameters number\n";
    return 1;
  }
  std::ifstream fin(argv[argc == 4 ? 3 : (argc == 3 ? 2 : 1)]);
  if (!fin) {
    std::cerr << "Error: invalid data file\n";
    return 1;
  }
  if (std::strcmp(argv[1], "--check") == 0 || (argc > 3 && std::strcmp(argv[2], "--check") == 0)) {
    std::ifstream fin_to_check(argv[argc == 4 ? 3 : (argc == 3 ? 2 : 1)]);
    if (!check_graphs_format(fin_to_check)) {
      std::cerr << "Error: invalid graphs format\n";
      return 1;
    }
    std::cout << "Valid graphs format\n";
  }
  if (std::strcmp(argv[1], "--help") == 0 || (argc > 3 && std::strcmp(argv[2], "--help") == 0)) {
    print_help_manual(std::cout);
    std::cout << '\n';
  }

  graphs_t graphs;
  std::string graph_name;
  while ((fin >> graph_name) && (!fin.eof())) {
    Graph gr;
    fin >> gr;
    graphs[graph_name] = gr;
  }
  if (fin.fail() && !fin.eof()) {
    std::cerr << "Error: invalid graphs format\n";
    return 1;
  }

  std::unordered_map< std::string, std::function< void() > > commands;
  commands["save-graphs"] = std::bind(save_graphs, std::cref(graphs), std::ref(std::cin));
  commands["open-graphs"] = std::bind(open_graphs, std::ref(graphs), std::ref(std::cin), std::ref(std::cout));

  commands["add-graph"] = std::bind(add_graph, std::ref(graphs), std::ref(std::cin));
  commands["add-vertice"] = std::bind(add_vertice, std::ref(graphs), std::ref(std::cin));
  commands["add-edge"] = std::bind(add_edge, std::ref(graphs), std::ref(std::cin));
  commands["print-graph"] = std::bind(print_graph, std::cref(graphs), std::ref(std::cin), std::ref(std::cout));
  commands["copy-graph"] = std::bind(copy_graph, std::ref(graphs), std::ref(std::cin));
  commands["delete-graph"] = std::bind(delete_graph, std::ref(graphs), std::ref(std::cin));
  commands["delete-vertice"] = std::bind(delete_vertice, std::ref(graphs), std::ref(std::cin));
  commands["delete-edge"] = std::bind(delete_edge, std::ref(graphs), std::ref(std::cin));
  commands["union-graphs"] = std::bind(union_graphs, std::ref(graphs), std::ref(std::cin));
  commands["create-subgraph"] = std::bind(create_subgraph, std::ref(graphs), std::ref(std::cin));

  commands["bfs"] = std::bind(traverse_breadth_first, std::cref(graphs), std::ref(std::cin), std::ref(std::cout));
  commands["path"] = std::bind(get_min_path, std::cref(graphs), std::ref(std::cin), std::ref(std::cout));
  commands["width"] = std::bind(get_graph_width, std::cref(graphs), std::ref(std::cin), std::ref(std::cout));
  commands["components"] = std::bind(get_graph_components, std::cref(graphs), std::ref(std::cin), std::ref(std::cout));

  std::string command;
  while ((std::cin >> command) && !std::cin.eof()) {
    try {
      commands.at(command)();
    } catch (...) {
      std::cout << "Error: invalid command\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
