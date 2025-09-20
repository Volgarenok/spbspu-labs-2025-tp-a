#include "commands.hpp"
#include <fstream>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
#include <iterator>
#include "graph.hpp"

namespace {
  using distances_t = std::unordered_map< unsigned, size_t >;
  using parents_t = std::unordered_map< unsigned, unsigned >;
  using nodes_queue_t = std::queue< unsigned >;
  using node_neighbour_pair_t = std::pair< const unsigned, std::vector< unsigned > >;
  using node_distance_pair_t = std::pair< const unsigned, size_t >;

  struct PathProcessor
  {
    unsigned current_node_;
    distances_t& distances_;
    parents_t& parents_;
    nodes_queue_t& nodes_queue_;

    PathProcessor(unsigned current_node, distances_t& distances, parents_t& parents, nodes_queue_t& nodes_queue):
      current_node_(current_node),
      distances_(distances),
      parents_(parents),
      nodes_queue_(nodes_queue)
    {}

    int operator()(unsigned neighbor)
    {
      if (distances_.find(neighbor) == distances_.end()) {
        distances_[neighbor] = distances_[current_node_] + 1;
        parents_[neighbor] = current_node_;
        nodes_queue_.push(neighbor);
      }
      return 0;
    }
  };

  void get_bfs_from(const maslevtsov::Graph& graph, unsigned start, distances_t& distances, parents_t& parents)
  {
    distances_t distances_result;
    parents_t parents_result;
    const std::unordered_map< unsigned, std::vector< unsigned > >& adj_list = graph.get_adj_list();
    nodes_queue_t to_visit;
    distances_result[start] = 0;
    to_visit.push(start);
    while (!to_visit.empty()) {
      unsigned current_node = to_visit.front();
      to_visit.pop();
      auto adj_list_it = adj_list.find(current_node);
      if (adj_list_it->second.empty()) {
        continue;
      }
      std::vector< int > dump(adj_list_it->second.size());
      std::transform(adj_list_it->second.begin(), adj_list_it->second.end(), dump.begin(),
        PathProcessor(current_node, distances_result, parents_result, to_visit));
    }
    distances = std::move(distances_result);
    parents = std::move(parents_result);
  }

  bool compare_distances(const node_distance_pair_t& left, const node_distance_pair_t& right)
  {
    return left.second < right.second;
  }

  size_t get_max_distance(const maslevtsov::Graph& graph, const node_neighbour_pair_t& node_neighbour_pair)
  {
    distances_t distances;
    parents_t parents;
    get_bfs_from(graph, node_neighbour_pair.first, distances, parents);
    auto max_el_it = std::max_element(distances.begin(), distances.end(), compare_distances);
    return max_el_it != distances.end() ? max_el_it->second : 0;
  }

  unsigned get_first_component_element(const std::pair< const unsigned, unsigned >& node_parent_pair)
  {
    return node_parent_pair.first;
  }

  unsigned get_second_component_element(const std::pair< const unsigned, unsigned >& node_parent_pair)
  {
    return node_parent_pair.second;
  }

  int get_component(const maslevtsov::Graph& graph, const node_neighbour_pair_t& node_neighbour_pair,
    std::vector< std::vector< unsigned > >& components)
  {
    distances_t distances;
    parents_t parents;
    get_bfs_from(graph, node_neighbour_pair.first, distances, parents);
    std::vector< unsigned > component;
    if (parents.empty()) {
      return 0;
    }
    std::transform(parents.begin(), parents.end(), std::back_inserter(component), get_first_component_element);
    std::transform(parents.begin(), parents.end(), std::back_inserter(component), get_second_component_element);
    std::sort(component.begin(), component.end());
    auto first_to_erase = std::unique(component.begin(), component.end());
    component.erase(first_to_erase, component.end());
    if (std::find(components.begin(), components.end(), component) == components.end()) {
      components.push_back(component);
    }
    return 0;
  }
}

bool maslevtsov::check_graphs_format(std::istream& in)
{
  std::string graph_name;
  while ((in >> graph_name) && (!in.eof())) {
    Graph gr;
    in >> gr;
  }
  if (in.eof()) {
    return true;
  }
  return false;
}

void maslevtsov::print_help_manual(std::ostream& out)
{
  out << "UNDIRECTED UNWEIGHT GRAPH\n\n";
  out << "SYNOPSIS:\n";
  out << "program [OPTION]... [FILE]...\n";
  out << "Launching the program with loading graphs from the [FILE]\n";
  out << "Options: --help - help output\n";
  out << "         --check - check graphs foramt in [FILE]\n\n";
  out << "GRAPHS FORMAT:\n";
  out << "<graph_name>\n<vertices_number>\n<vertice1>: <vertice2>...\n<vertice2>: <vertice1>...\n...\n";
  out << "Vertices of the graph are numbered with an unsigned integer, starting from zero.\n\n";
  out << "USER COMMANDS:\n";
  out << "1. save [FILE] - save all current graphs to [FILE];\n";
  out << "2. open [FILE] - read graphs from [FILE];\n";
  out << "3. add-graph <graph_name> <vertices_number> <edges_number>\n   <vertice1> <vertice2>\n   ...\n";
  out << "   create a graph named <graph_name> with edges: <vertice1> <vertice2>...;\n";
  out << "4. add-vertice <graph_name> <vertice>\n";
  out << "   add vertice to <graph_name>;\n";
  out << "5. add-edge <graph_name> <vertice1> <vertice2>\n";
  out << "   add edge to <graph_name>;\n";
  out << "6. print-graph <graph_name> - print adjacency list for <graph_name>;\n";
  out << "7. copy <graph_name> <new_graph_name> - copy <graph_name> to <new_graph_name>;\n";
  out << "8. delete-graph <graph_name> - delete <graph_name>;\n";
  out << "9. delete-vertice <graph_name> <vertice> - delete <vertice> from <graph_name>;\n";
  out << "10. delete-edge <graph_name> <vertice1> <vertice2> - delete edge from <graph_name>;\n";
  out << "11. union <new_graph_name> <graph_name1> <graph_name2>\n";
  out << "    create <new_graph_name> based on two existing <graph_name1> and <graph_name2>;\n";
  out << "12. subgraph <new_graph_name> <graph_name> <vertices_number> <vertice1>...\n";
  out << "    create <new_graph_name> by selecting the vertices of <graph_name>, preserving the edges;\n";
  out << "13. bfs <graph_name> <start_node>\n";
  out << "    breadth-first traversal of <graph_name> starting at <start_node>,\n";
  out << "    printing all distances from <start_node> to each node reachable from <start_node>;\n";
  out << "14. path <graph_name> <start_node> <goal_node>\n";
  out << "    find the shortest chain of nodes from <start_node> to <goal_node>,\n";
  out << "    calculate the minimum distance in <graph_name>;\n";
  out << "15. width <graph_name> - find width of <graph_name>;\n";
  out << "16. components <graph_name> - find connected components in <graph_name>.";
}

void maslevtsov::save_graphs(const graphs_t& graphs, std::istream& in)
{
  std::string filename;
  in >> filename;
  std::ofstream save_file(filename);
  if (!save_file) {
    throw std::invalid_argument("invalid filename");
  }
  for (auto i = graphs.cbegin(); i != graphs.cend(); ++i) {
    save_file << i->first << '\n';
    save_file << i->second << '\n';
  }
}

void maslevtsov::open_graphs(graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string filename;
  in >> filename;
  std::ifstream open_file(filename);
  if (!open_file) {
    throw std::invalid_argument("invalid filename");
  }
  out << "Do you want to save current graphs? y/n: ";
  std::string save_info;
  in >> save_info;
  if (save_info != "y" && save_info != "n") {
    throw std::invalid_argument("invalid save info");
  }
  if (save_info == "y") {
    out << "Enter filename: ";
    save_graphs(graphs, in);
  }
  graphs_t new_graphs;
  std::string graph_name;
  while ((open_file >> graph_name) && (!open_file.eof())) {
    Graph gr;
    open_file >> gr;
    new_graphs[graph_name] = gr;
  }
  if (!open_file.eof()) {
    throw std::invalid_argument("invalid graphs format");
  }
  graphs = new_graphs;
}

void maslevtsov::add_graph(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  in >> graph_name;
  if (graphs.find(graph_name) != graphs.end()) {
    throw std::invalid_argument("graph already exist");
  }
  Graph gr;
  size_t edge_count = 0;
  in >> edge_count;
  for (size_t i = 0; i != edge_count; ++i) {
    unsigned vertice1 = 0, vertice2 = 0;
    in >> vertice1 >> vertice2;
    gr.add_edge(vertice1, vertice2);
  }
  graphs[graph_name] = gr;
}

void maslevtsov::add_vertice(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  unsigned vertice = 0;
  in >> graph_name >> vertice;
  graphs.at(graph_name).add_vertice(vertice);
}

void maslevtsov::add_edge(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  unsigned vertice1 = 0, vertice2 = 0;
  in >> graph_name >> vertice1 >> vertice2;
  graphs.at(graph_name).add_edge(vertice1, vertice2);
}

void maslevtsov::print_graph(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  in >> graph_name;
  out << graphs.at(graph_name);
  out << '\n';
}

void maslevtsov::copy_graph(graphs_t& graphs, std::istream& in)
{
  std::string gr_name, new_gr_name;
  in >> gr_name >> new_gr_name;
  if (graphs.find(new_gr_name) != graphs.end()) {
    throw std::invalid_argument("invalid graph names");
  }
  Graph new_gr(graphs.at(gr_name));
  graphs[new_gr_name] = new_gr;
}

void maslevtsov::delete_graph(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  in >> graph_name;
  if (graphs.find(graph_name) == graphs.end()) {
    throw std::invalid_argument("invalid graph names");
  }
  graphs.erase(graphs.find(graph_name));
}

void maslevtsov::delete_vertice(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  unsigned vertice = 0;
  in >> graph_name >> vertice;
  graphs.at(graph_name).delete_vertice(vertice);
}

void maslevtsov::delete_edge(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  unsigned vertice1 = 0, vertice2 = 0;
  in >> graph_name >> vertice1 >> vertice2;
  graphs.at(graph_name).delete_edge(vertice1, vertice2);
}

void maslevtsov::union_graphs(graphs_t& graphs, std::istream& in)
{
  std::string new_gr_name, gr_name1, gr_name2;
  in >> new_gr_name >> gr_name1 >> gr_name2;
  if (graphs.find(new_gr_name) != graphs.end()) {
    throw std::invalid_argument("invalid graph names");
  }
  Graph new_gr(graphs.at(gr_name1), graphs.at(gr_name2));
  graphs[new_gr_name] = new_gr;
}

void maslevtsov::create_subgraph(graphs_t& graphs, std::istream& in)
{
  std::string new_gr_name, gr_name;
  size_t vertice_count = 0;
  in >> new_gr_name >> gr_name >> vertice_count;
  if (graphs.find(new_gr_name) != graphs.end()) {
    throw std::invalid_argument("invalid graph names");
  }
  std::vector< unsigned > vertices(vertice_count);
  std::copy_n(std::istream_iterator< unsigned >(in), vertice_count, vertices.begin());
  Graph new_gr(graphs.at(gr_name), vertices);
  graphs[new_gr_name] = new_gr;
}

void maslevtsov::traverse_breadth_first(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  unsigned start_node = 0;
  in >> graph_name >> start_node;
  auto gr_it = graphs.find(graph_name);
  if (gr_it == graphs.cend() || gr_it->second.get_adj_list().find(start_node) == gr_it->second.get_adj_list().cend()) {
    throw std::invalid_argument("non-existing graph");
  }
  distances_t distances;
  parents_t parents;
  get_bfs_from(gr_it->second, start_node, distances, parents);
  for (auto i = distances.cbegin(); i != distances.cend(); ++i) {
    out << start_node << '-' << i->first << " : " << i->second << '\n';
  }
}

void maslevtsov::get_min_path(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  unsigned start_node = 0, goal_node = 0;
  in >> graph_name >> start_node >> goal_node;
  auto gr_it = graphs.find(graph_name);
  if (gr_it == graphs.cend() || gr_it->second.get_adj_list().find(start_node) == gr_it->second.get_adj_list().cend()) {
    throw std::invalid_argument("non-existing graph");
  }
  distances_t distances;
  parents_t parents;
  get_bfs_from(gr_it->second, start_node, distances, parents);
  if (distances.find(goal_node) == distances.end()) {
    throw std::invalid_argument("non-existing path");
  }
  std::stack< unsigned > restored_path;
  unsigned current_node = goal_node;
  while (current_node != start_node) {
    restored_path.push(current_node);
    current_node = parents[current_node];
  }
  out << current_node << '-' << restored_path.top();
  restored_path.pop();
  while (!restored_path.empty()) {
    out << '-' << restored_path.top();
    restored_path.pop();
  }
  out << ' ' << distances[goal_node] << '\n';
}

void maslevtsov::get_graph_width(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  in >> graph_name;
  auto gr_it = graphs.find(graph_name);
  if (gr_it == graphs.cend()) {
    throw std::invalid_argument("non-existing graph");
  }
  const std::unordered_map< unsigned, std::vector< unsigned > >& adj_list = gr_it->second.get_adj_list();
  std::vector< size_t > all_distances;
  using namespace std::placeholders;
  auto distance_collector = std::bind(get_max_distance, std::ref(gr_it->second), _1);
  std::transform(adj_list.begin(), adj_list.end(), std::back_inserter(all_distances), distance_collector);
  out << *std::max_element(all_distances.begin(), all_distances.end()) << '\n';
}

void maslevtsov::get_graph_components(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  in >> graph_name;
  auto gr_it = graphs.find(graph_name);
  if (gr_it == graphs.cend()) {
    throw std::invalid_argument("non-existing graph");
  }
  const std::unordered_map< unsigned, std::vector< unsigned > >& adj_list = gr_it->second.get_adj_list();
  std::vector< std::vector< unsigned > > all_components;
  std::vector< int > dump;
  using namespace std::placeholders;
  auto component_collector = std::bind(get_component, std::ref(gr_it->second), _1, std::ref(all_components));
  std::transform(adj_list.begin(), adj_list.end(), std::back_inserter(dump), component_collector);
  for (auto i = all_components.begin(); i != all_components.end(); ++i) {
    out << *i->begin();
    for (auto j = ++i->begin(); j != i->end(); ++j) {
      out << '-' << *j;
    }
    out << '\n';
  }
}
