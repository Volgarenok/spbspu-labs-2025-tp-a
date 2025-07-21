#ifndef CAVE_COMPONENTS_HPP
#define CAVE_COMPONENTS_HPP
#include <map>
#include <iostream>
#include <string>
#include <list>

namespace zakirov
{
  class CaveComponent
  {
    public:
      char getCell() const noexcept;
      char setCell(char cell) noexcept;
      int getDist();
      int setDist(int dist);
      void visitCell();
      bool isVisited();
      bool isWall() const noexcept;
      bool isPath() const noexcept;
      bool isEntry() const noexcept;
      bool isExit() const noexcept;
    private:
      char cell_;
      int dist_;
      bool visit_ = false;
  };

  class CaveLayer
  {
  public:
    CaveLayer & getPath();
    bool isScopes();
    std::pair< int, int > getCords() const noexcept;
    void setCords(std::pair< int, int > cords) noexcept;
    char getName() const noexcept;
    void setName(char name) noexcept;
    void addEdge(int key1, int key2);
    void addEdges(int key);
    void addCell(int key, CaveComponent & cell);
    CaveComponent & getCell(int key);
  private:
    std::map< int, std::pair< CaveComponent, std::list< int > > > cave_;
    char name_;
    std::pair< int, int > cords_;
    std::pair< int, int > scopes_;
  };

  std::istream & operator>>(std::istream & in, CaveComponent & comp_c);
  std::istream & operator>>(std::istream & in, CaveLayer & layer_c);
}

#endif
