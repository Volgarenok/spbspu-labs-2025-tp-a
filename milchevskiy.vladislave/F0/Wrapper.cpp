#include "Wrapper.hpp"
#include <iostream>
#include <string>

namespace milchevskiy
{
  void createWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string name;
    in >> name;
    gs.create(name);
  }

  void isEmptyWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string name;
    in >> name;
    gs.isEmpty(name);
  }

  void searchNodeWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName, nodeName;
    in >> graphName >> nodeName;
    gs.searchNode(graphName, nodeName);
  }

  void searchArcWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName, node1, node2;
    in >> graphName >> node1 >> node2;
    gs.searchArc(graphName, node1, node2);
  }

  void insertNodeWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName, nodeName;
    in >> graphName >> nodeName;
    gs.insertNode(graphName, nodeName);
  }

  void insertArcWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName, node1, node2;
    in >> graphName >> node1 >> node2;
    gs.insertArc(graphName, node1, node2);
  }

  void deleteNodeWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName, nodeName;
    in >> graphName >> nodeName;
    gs.deleteNode(graphName, nodeName);
  }

  void printWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName;
    in >> graphName;
    gs.print(graphName);
  }

  void clearWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName;
    in >> graphName;
    gs.clear(graphName);
  }

  void isCyclicalWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName, newGraphName;
    in >> graphName >> newGraphName;
    gs.isCyclical(graphName, newGraphName);
  }

  void shortestPathWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName, node1, node2, resultGraphName;
    in >> graphName >> node1 >> node2 >> resultGraphName;
    gs.shortestPath(graphName, node1, node2, resultGraphName);
  }

  void topologicalSortWrapper(GraphSystem& gs, std::istream& in, std::ostream&)
  {
    std::string graphName, newGraphName;
    in >> graphName >> newGraphName;
    gs.topologicalSort(graphName, newGraphName);
  }
}
