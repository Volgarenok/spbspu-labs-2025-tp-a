#ifndef NETWORK_H
#define NETWORK_H
#include <string>
#include <iosfwd>
#include <unordered_map>
#include <command_handler.h>
#include "graph.h"

namespace ohantsev
{
  class NetworkApp: public CommandHandler< std::unordered_map< std::string, Graph< std::string > > >
  {
  public:
    using graph_type = Graph< std::string >;
    using map_type = std::unordered_map< std::string, graph_type >;
    NetworkApp(map_type& networks, std::istream& in, std::ostream& out);
    void operator()() override;
    void input(std::string filename);

  private:
    static void create(map_type& networks, std::istream& in);
    static void deleteNetwork(map_type& networks, std::istream& in);
    static void showAll(const map_type& networks, std::ostream& out);
    static void showNetwork(const map_type& networks, std::istream& in, std::ostream& out);
    static void addDevice(map_type& networks, std::istream& in);
    static void connect(map_type& networks, std::istream& in);
    static void disconnect(map_type& networks, std::istream& in);
    static void deleteDevice(map_type& networks, std::istream& in);
    static void forceDeleteDevice(map_type& networks, std::istream& in);
    static void copy(map_type& networks, std::istream& in);
    static void removeLoops(map_type& networks, std::istream& in);
    static void removeLoopsNew(map_type& networks, std::istream& in);
    static void distance(const map_type& networks, std::istream& in, std::ostream& out);
    static void topPaths(const map_type& networks, std::istream& in, std::ostream& out);
    static void topPathsNoCycles(const map_type& networks, std::istream& in, std::ostream& out);
    static void merge(map_type& networks, std::istream& in);
    static void save(const map_type& networks, std::istream& in);

    static void printWay(const graph_type::Way& way, std::ostream& out);
    static void printWays(const std::vector< graph_type::Way >& ways, std::ostream& out);
  };

  std::istream& operator>>(std::istream& in, Graph< std::string >& graph);
}
#endif
