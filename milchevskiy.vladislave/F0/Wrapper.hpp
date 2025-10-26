#include "GraphSystem.hpp"

namespace milchevskiy
{
  void createWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void isEmptyWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void searchNodeWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void searchArcWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void insertNodeWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void insertArcWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void deleteNodeWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void printWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void clearWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void isCyclicalWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void shortestPathWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
  void topologicalSortWrapper(GraphSystem& gs, std::istream& in, std::ostream&);
}
