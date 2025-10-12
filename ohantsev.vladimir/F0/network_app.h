#ifndef NETWORK_H
#define NETWORK_H
#include <string>
#include <iosfwd>
#include <iterator>
#include <unordered_map>
#include "graph.h"
#include "command_handler.h"

namespace ohantsev
{
  class NetworkApp: public CommandHandler
  {
  public:
    using graph_type = Graph< std::string >;
    using map_type = std::unordered_map< std::string, graph_type >;
    using outIter = std::ostream_iterator< std::string >;

    NetworkApp(map_type& networks, std::istream& in, std::ostream& out);
    void operator()() override;
    void input(const std::string& filename);

  private:
    struct DeviceConnectionsPrinter;
    struct ConnectionPrinter;
    struct ConnectionAdder;
    struct NetworkMerger;
    struct NetworkSaver;
    struct ConnectionSaver;
    struct DeviceSaver;
    struct ConnectionCounter;

    map_type& networks_;

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
    template < bool AllowCycles >
    static void topPaths(const map_type& networks, std::istream& in, std::ostream& out);
    static void topPathsWithCycles(const map_type& networks, std::istream& in, std::ostream& out);
    static void topPathsNoCycles(const map_type& networks, std::istream& in, std::ostream& out);
    static void merge(map_type& networks, std::istream& in);
    static void save(const map_type& networks, std::istream& in);
    static std::string getName(const map_type::value_type& pair);
  };

  std::istream& operator>>(std::istream& in, Graph< std::string >& graph);
  void skipGraph(std::istream& in);
  void readConnection(std::istream& in, Graph< std::string >& graph);
  std::ostream& operator<<(std::ostream& out, const Graph< std::string >::Way& way);
}
#endif
