#include "network_app.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>

ohantsev::NetworkApp::NetworkApp(map_type& networks, std::istream& in, std::ostream& out):
  CommandHandler(in, out),
  networks_(networks)
{
  add("create", std::bind(create, std::ref(networks), std::ref(in)));
  add("delete_network", std::bind(deleteNetwork, std::ref(networks), std::ref(in)));
  add("show_all", std::bind(showAll, std::cref(networks), std::ref(out)));
  add("show_network", std::bind(showNetwork, std::cref(networks), std::ref(in), std::ref(out)));
  add("add", std::bind(addDevice, std::ref(networks), std::ref(in)));
  add("connect", std::bind(connect, std::ref(networks), std::ref(in)));
  add("disconnect", std::bind(disconnect, std::ref(networks), std::ref(in)));
  add("delete_device", std::bind(deleteDevice, std::ref(networks), std::ref(in)));
  add("force_delete_device", std::bind(forceDeleteDevice, std::ref(networks), std::ref(in)));
  add("copy", std::bind(copy, std::ref(networks), std::ref(in)));
  add("remove_loops", std::bind(removeLoops, std::ref(networks), std::ref(in)));
  add("remove_loops_new", std::bind(removeLoopsNew, std::ref(networks), std::ref(in)));
  add("distance", std::bind(distance, std::cref(networks), std::ref(in), std::ref(out)));
  add("top_paths", std::bind(topPathsWithCycles, std::cref(networks), std::ref(in), std::ref(out)));
  add("top_paths_nocycles", std::bind(topPathsNoCycles, std::cref(networks), std::ref(in), std::ref(out)));
  add("merge", std::bind(merge, std::ref(networks), std::ref(in)));
  add("save", std::bind(save, std::cref(networks), std::ref(in)));
}

void ohantsev::NetworkApp::operator()()
{
  try
  {
    out_ << "> ";
    CommandHandler::operator()();
  }
  catch (std::invalid_argument& e)
  {
    if (!in_.eof())
    {
      out_ << e.what() << '\n';
    }
  }
  catch (std::exception& e)
  {
    if (!in_.eof())
    {
      out_ << "Invalid command" << '\n';
    }
  }
  if (in_.fail())
  {
    in_.clear(in_.rdstate() ^ std::ios::failbit);
  }
  in_.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}

void ohantsev::NetworkApp::create(map_type& networks, std::istream& in)
{
  std::string name;
  if (in >> name && !(networks.emplace(name, graph_type{}).second))
  {
    throw std::invalid_argument("Network " + name + " already exists");
  }
}

void ohantsev::NetworkApp::deleteNetwork(map_type& networks, std::istream& in)
{
  std::string name;
  if (in >> name && !networks.erase(name))
  {
    throw std::invalid_argument("Network " + name + " not found");
  }
}

std::string ohantsev::NetworkApp::getName(const map_type::value_type& pair)
{
  return pair.first;
}

void ohantsev::NetworkApp::showAll(const map_type& networks, std::ostream& out)
{
  std::transform(networks.begin(), networks.end(), outIter(out, "\n"), getName);
}

struct ohantsev::NetworkApp::ConnectionPrinter
{
  std::ostream& out;

  void operator()(const graph_type::ConnectionMap::value_type& cntPair) const
  {
    out << " -> " << cntPair.first << " " << cntPair.second << '\n';
  }
};

struct ohantsev::NetworkApp::DeviceConnectionsPrinter
{
  std::ostream& out;

  void operator()(const graph_type::GraphMap::value_type& nodesPair) const
  {
    out << "Device: " << nodesPair.first << '\n';
    std::for_each(nodesPair.second.begin(), nodesPair.second.end(), ConnectionPrinter{ out });
  }
};

void ohantsev::NetworkApp::showNetwork(const map_type& networks, std::istream& in, std::ostream& out)
{
  std::string name;
  if (in >> name)
  {
    auto iter = networks.find(name);
    if (iter == networks.end())
    {
      throw std::invalid_argument("Network " + name + " not found");
    }
    const auto& network = iter->second.watch();
    std::for_each(network.begin(), network.end(), DeviceConnectionsPrinter{ out });
  }
}

void ohantsev::NetworkApp::addDevice(map_type& networks, std::istream& in)
{
  std::string net;
  std::string device;
  if (in >> net >> device)
  {
    auto iter = networks.find(net);
    if (iter == networks.end())
    {
      throw std::invalid_argument("Network " + net + " not found");
    }
    auto& network = iter->second;
    if (!network.insert(device))
    {
      throw std::invalid_argument("Device already exists");
    }
  }
}

void ohantsev::NetworkApp::connect(map_type& networks, std::istream& in)
{
  std::string net;
  std::string from;
  std::string to;
  std::size_t weight;
  if (!(in >> net >> from >> to >> weight))
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto iter = networks.find(net);
  if (iter == networks.end())
  {
    throw std::invalid_argument("Network " + net + " not found");
  }
  auto& network = iter->second;
  if (!network.link(from, to, weight))
  {
    throw std::invalid_argument("Connection between " + from + "and" + to + " already exists");
  }
}

void ohantsev::NetworkApp::disconnect(map_type& networks, std::istream& in)
{
  std::string net;
  std::string from;
  std::string to;
  if (in >> net >> from >> to)
  {
    auto iter = networks.find(net);
    if (iter == networks.end())
    {
      throw std::invalid_argument("Network " + net + " not found");
    }
    auto& network = iter->second;
    if (!network.removeLink(from, to))
    {
      throw std::invalid_argument("Connection between " + from + "and" + to + " not found");
    }
  }
}

void ohantsev::NetworkApp::deleteDevice(map_type& networks, std::istream& in)
{
  std::string net;
  std::string device;
  if (in >> net >> device)
  {
    auto iter = networks.find(net);
    if (iter == networks.end())
    {
      throw std::invalid_argument("Network " + net + " not found");
    }
    auto& network = iter->second;
    if (!network.remove(device))
    {
      if (!network.contains(device))
      {
        throw std::invalid_argument("Device " + device + " not found");
      }
      throw std::invalid_argument("Device " + device +
        " still connected\nUse force_delete_device to whatever remove with connections");
    }
  }
}

void ohantsev::NetworkApp::forceDeleteDevice(map_type& networks, std::istream& in)
{
  std::string net;
  std::string device;
  if (in >> net >> device)
  {
    auto iter = networks.find(net);
    if (iter == networks.end())
    {
      throw std::invalid_argument("Network " + net + " not found");
    }
    auto& network = iter->second;
    if (!network.removeForce(device))
    {
      throw std::invalid_argument("Device " + device + " not found");
    }
  }
}

void ohantsev::NetworkApp::copy(map_type& networks, std::istream& in)
{
  std::string source;
  std::string dest;
  if (in >> source >> dest)
    {
    auto iterSource = networks.find(source);
    if (iterSource == networks.end())
    {
      throw std::invalid_argument("Network " + source + " not found");
    }
    auto iterDest = networks.find(dest);
    if (iterDest != networks.end())
    {
      throw std::invalid_argument("Network " + dest + " already exists");
    }
    networks.emplace(dest, graph_type(iterSource->second));
  }
}

void ohantsev::NetworkApp::removeLoops(map_type& networks, std::istream& in)
{
  std::string net;
  if (in >> net)
  {
    auto iter = networks.find(net);
    if (iter == networks.end())
    {
      throw std::invalid_argument("Network " + net + " not found");
    }
    auto& network = iter->second;
    network.removeCycles();
  }
}

void ohantsev::NetworkApp::removeLoopsNew(map_type& networks, std::istream& in)
{
  std::string net;
  std::string newNet;
  if (in >> net >> newNet)
  {
    auto iter = networks.find(net);
    if (iter == networks.end())
    {
      throw std::invalid_argument("Network " + net + " not found");
    }
    auto iterNew = networks.find(newNet);
    if (iterNew != networks.end())
    {
      throw std::invalid_argument("Network " + newNet + " already exists");
    }
    graph_type networkGraph(iter->second);
    networkGraph.removeCycles();
    networks.emplace(newNet, std::move(networkGraph));
  }
}

void ohantsev::NetworkApp::distance(const map_type& networks, std::istream& in, std::ostream& out)
{
  std::string net;
  std::string from;
  std::string to;
  if (in >> net >> from >> to)
  {
    auto iter = networks.find(net);
    if (iter == networks.end())
    {
      throw std::invalid_argument("Network " + net + " not found");
    }
    auto& network = iter->second;
    if (!network.contains(from))
    {
      throw std::invalid_argument("Device " + from + " not found");
    }
    if (!network.contains(to))
    {
      throw std::invalid_argument("Device " + to + " not found");
    }
    out << iter->second.path(from, to).length_ << '\n';
  }
}

std::ostream& ohantsev::operator<<(std::ostream& out, const Graph< std::string >::Way& way)
{
  out << way.length_ << '\t';
  std::copy(way.steps_.begin(), way.steps_.end() - 1, std::ostream_iterator< std::string >{ out, " -> " });
  return (out << way.steps_.back() << '\n');
}

template< bool AllowCycles >
void ohantsev::NetworkApp::topPaths(const map_type& networks, std::istream& in, std::ostream& out)
{
  std::string net;
  std::string from;
  std::string to;
  std::size_t count;
  if (!(in >> net >> from >> to >> count))
  {
    throw std::invalid_argument("Invalid arguments");
  }
  auto iter = networks.find(net);
  if (iter == networks.end())
  {
    throw std::invalid_argument("Network " + net + " not found");
  }
  auto& network = iter->second;
  if (!network.contains(from))
  {
    throw std::invalid_argument("Device " + from + " not found");
  }
  if (!network.contains(to))
  {
    throw std::invalid_argument("Device " + to + " not found");
  }
  auto ways = network.nPaths< AllowCycles >(from, to, count);
  std::copy(ways.begin(), ways.end(), std::ostream_iterator< graph_type::Way >{ out });
}

void ohantsev::NetworkApp::topPathsWithCycles(const map_type& networks, std::istream& in, std::ostream& out)
{
  topPaths< true >(networks, in, out);
}

void ohantsev::NetworkApp::topPathsNoCycles(const map_type& networks, std::istream& in, std::ostream& out)
{
  topPaths< false >(networks, in, out);
}

struct ohantsev::NetworkApp::ConnectionAdder
{
  graph_type& dest;
  const std::string& from;

  void operator()(const graph_type::ConnectionMap::value_type& cnt) const
  {
    dest.link(from, cnt.first, cnt.second);
  }
};

struct ohantsev::NetworkApp::NetworkMerger
{
  graph_type& dest;

  void operator()(const graph_type::GraphMap::value_type& device) const
  {
    std::for_each(device.second.begin(), device.second.end(), ConnectionAdder{ dest, device.first });
  }
};

void ohantsev::NetworkApp::merge(map_type& networks, std::istream& in)
{
  std::string sourceFirst;
  std::string sourceSecond;
  std::string dest;
  if (in >> sourceFirst >> sourceSecond >> dest)
  {
    auto iterFirst = networks.find(sourceFirst);
    auto iterSecond = networks.find(sourceSecond);
    if (iterFirst == networks.end())
    {
      throw std::invalid_argument("Network " + sourceFirst + " not found");
    }
    if (iterSecond == networks.end())
    {
      throw std::invalid_argument("Network " + sourceSecond + " not found");
    }
    auto iterDest = networks.find(dest);
    if (iterDest != networks.end())
    {
      throw std::invalid_argument("Network " + dest + " already exists");
    }
    graph_type destNet = (iterFirst->second);
    auto& secondNetMap =  iterSecond->second.watch();
    std::for_each(secondNetMap.begin(), secondNetMap.end(), NetworkMerger{ destNet });
    networks.emplace(dest, std::move(destNet));
  }
}

struct ohantsev::NetworkApp::ConnectionSaver
{
  std::ostream& out;
  const std::string& from;

  void operator()(const graph_type::ConnectionMap::value_type& cnt) const
  {
    out << from << " " << cnt.first << " " << cnt.second << "\n";
  }
};

struct ohantsev::NetworkApp::ConnectionCounter
{
  std::size_t operator()(std::size_t sum, const graph_type::GraphMap::value_type& nodePair) const
  {
    return sum + nodePair.second.size();
  }
};

struct ohantsev::NetworkApp::DeviceSaver
{
  std::ostream& out;

  void operator()(const graph_type::GraphMap::value_type& device) const
  {
    std::for_each(device.second.begin(), device.second.end(), ConnectionSaver{ out, device.first });
  }
};

struct ohantsev::NetworkApp::NetworkSaver
{
  std::ostream& out;

  void operator()(const map_type::value_type& network) const
  {
    auto& graph = network.second.watch();
    size_t count = std::accumulate(graph.begin(), graph.end(), 0, ConnectionCounter{});
    out << network.first << " " << count << "\n";
    std::for_each(graph.begin(), graph.end(), DeviceSaver{ out });
    out << "\n";
  }
};

void ohantsev::NetworkApp::save(const map_type& networks, std::istream& in)
{
  std::string filename;
  if (in >> filename)
  {
    std::ofstream fout(filename);
    std::for_each(networks.begin(), networks.end(), NetworkSaver{ fout });
  }
}

void ohantsev::NetworkApp::input(const std::string& filename)
{
  std::ifstream in(filename);
  if (!in.is_open())
  {
    throw std::invalid_argument("File not found");
  }
  while (!in.eof())
  {
    std::string name;
    graph_type graph;
    if (in >> name >> graph)
    {
      networks_.emplace(name, std::move(graph));
    }
    else
    {
      in.clear(in.rdstate() ^ std::ios::failbit);
      skipGraph(in);
    }
  }
}

void ohantsev::skipGraph(std::istream& in)
{
  for (std::string line; std::getline(in, line) && !line.empty(););
}

void ohantsev::readConnection(std::istream& in, Graph< std::string >& graph)
{
  std::string from, to;
  std::size_t weight;
  if (in >> from >> to >> weight)
  {
    graph.link(from, to, weight);
  }
}

std::istream& ohantsev::operator>>(std::istream& in, Graph< std::string >& graph)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  std::size_t size;
  in >> size;
  for (std::size_t i = 0; i < size && in; ++i)
  {
    readConnection(in, graph);
  }
  return in;
}
