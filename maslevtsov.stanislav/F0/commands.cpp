#include "commands.hpp"

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
