#include "graph.hpp"
#include <iostream>

void maslevtsov::Graph::print_adjacency_list(std::ostream& out)
{
  out << edges_.size() << '\n';
  out << edges_.cbegin()->first;
  for (auto i = ++edges_.cbegin(); i != edges_.cend(); ++i) {
    out << '\n' << i->first << " :";
    for (auto j = i->second.cbegin(); j != i->second.cend(); ++j) {
      out << ' ' << *j;
    }
  }
}
