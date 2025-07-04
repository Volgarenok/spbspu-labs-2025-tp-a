#include "commands.hpp"
#include <map>
#include <string>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <functional>
#include <stream-guard.hpp>

namespace
{
  using is_t = std::istream;
  using os_t = std::ostream;
  using polygon_set_t = std::vector< sveshnikov::Polygon >;
  using param_holder_t = std::map< std::string, std::function< void() > >;

  bool isEven(const sveshnikov::Polygon &poly);
  size_t getNumVertexes(const sveshnikov::Polygon &poly);
  std::vector< double > getAreaSet(const polygon_set_t &shapes);
  std::vector< size_t > getVertexesSet(const polygon_set_t &shapes);

  double area_impl(const polygon_set_t &shapes);
  double area_even(const polygon_set_t &shapes);
  double area_odd(const polygon_set_t &shapes);
  double area_mean(const polygon_set_t &shapes);
  double area_num_vertexes(const polygon_set_t &shapes, size_t num);

  void max_area(os_t &out, const polygon_set_t &shapes);
  void max_vertexes(os_t &out, const polygon_set_t &shapes);

  void min_area(os_t &out, const polygon_set_t &shapes);
  void min_vertexes(os_t &out, const polygon_set_t &shapes);

  size_t count_even(const polygon_set_t &shapes);
  size_t count_odd(const polygon_set_t &shapes);
  size_t count_num_vertexes(const polygon_set_t &shapes, size_t num);

  bool isEven(const sveshnikov::Polygon &poly)
  {
    return poly.points.size() % 2 == 0;
  }

  size_t getNumVertexes(const sveshnikov::Polygon &poly)
  {
    return poly.points.size();
  }

  std::vector< double > getAreaSet(const polygon_set_t &shapes)
  {
    using namespace sveshnikov;
    std::vector< double > areas(shapes.size());
    std::transform(shapes.begin(), shapes.end(), areas.begin(), getPolygonArea);
    return areas;
  }

  std::vector< size_t > getVertexesSet(const polygon_set_t &shapes)
  {
    std::vector< size_t > num_vertexes_set(shapes.size());
    std::transform(shapes.begin(), shapes.end(), num_vertexes_set.begin(), getNumVertexes);
    return num_vertexes_set;
  }

  double area_impl(const polygon_set_t &shapes)
  {
    std::vector< double > areas = getAreaSet(shapes);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double area_even(const polygon_set_t &shapes)
  {
    polygon_set_t pts;
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(pts), isEven);
    return area_impl(pts);
  }

  double area_odd(const polygon_set_t &shapes)
  {
    return area_impl(shapes) - area_even(shapes);
  }

  double area_mean(const polygon_set_t &shapes)
  {
    if (shapes.empty())
    {
      throw std::out_of_range("Error: no shapes!");
    }
    return area_impl(shapes) / shapes.size();
  }

  double area_num_vertexes(const polygon_set_t &shapes, size_t num)
  {
    using namespace std::placeholders;
    polygon_set_t pts;
    auto is_num_vert = std::bind(std::equal_to< size_t >(), std::bind(getNumVertexes, _1), num);
    std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(pts), is_num_vert);
    return area_impl(pts);
  }

  void max_area(os_t &out, const polygon_set_t &shapes)
  {
    std::vector< double > areas = getAreaSet(shapes);
    auto it = std::max_element(areas.begin(), areas.end());
    sveshnikov::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << *it << '\n';
  }

  void max_vertexes(os_t &out, const polygon_set_t &shapes)
  {
    std::vector< size_t > num_vertexes = getVertexesSet(shapes);
    auto it = std::max_element(num_vertexes.begin(), num_vertexes.end());
    sveshnikov::StreamGuard guard(out);
    out << *it << '\n';
  }

  void min_area(os_t &out, const polygon_set_t &shapes)
  {
    std::vector< double > areas = getAreaSet(shapes);
    auto it = std::min_element(areas.begin(), areas.end());
    sveshnikov::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << *it << '\n';
  }

  void min_vertexes(os_t &out, const polygon_set_t &shapes)
  {
    std::vector< size_t > num_vertexes = getVertexesSet(shapes);
    auto it = std::min_element(num_vertexes.begin(), num_vertexes.end());
    sveshnikov::StreamGuard guard(out);
    out << *it << '\n';
  }

  size_t count_even(const polygon_set_t &shapes)
  {
    return std::count_if(shapes.begin(), shapes.end(), isEven);
  }

  size_t count_odd(const polygon_set_t &shapes)
  {
    return shapes.size() - count_even(shapes);
  }

  size_t count_num_vertexes(const polygon_set_t &shapes, size_t num)
  {
    using namespace std::placeholders;
    auto is_num_vert = std::bind(std::equal_to< size_t >(), std::bind(getNumVertexes, _1), num);
    return std::count_if(shapes.begin(), shapes.end(), is_num_vert);
  }
}

void sveshnikov::area(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  std::map< std::string, std::function< double() > > params;
  params["EVEN"] = std::bind(area_even, std::cref(shapes));
  params["ODD"] = std::bind(area_odd, std::cref(shapes));
  params["MEAN"] = std::bind(area_mean, std::cref(shapes));

  std::string parm;
  in >> parm;
  double area = 0.0;
  try
  {
    size_t len_str = 0;
    int num_vertexes = std::stoi(parm, &len_str);
    if (len_str != parm.size() || num_vertexes < 3)
    {
      throw std::invalid_argument("Error: the parameter is not a number!");
    }
    area = area_num_vertexes(shapes, num_vertexes);
  }
  catch (const std::invalid_argument &e)
  {
    area = params.at(parm)();
  }

  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << area << '\n';
}

void sveshnikov::max(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  std::map< std::string, std::function< void() > > params;
  params["AREA"] = std::bind(max_area, std::ref(out), std::cref(shapes));
  params["VERTEXES"] = std::bind(max_vertexes, std::ref(out), std::cref(shapes));

  std::string parm;
  in >> parm;
  if (shapes.empty())
  {
    throw std::out_of_range("Error: no shapes!");
  }
  params.at(parm)();
}

void sveshnikov::min(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  std::map< std::string, std::function< void() > > params;
  params["AREA"] = std::bind(min_area, std::ref(out), std::cref(shapes));
  params["VERTEXES"] = std::bind(min_vertexes, std::ref(out), std::cref(shapes));

  std::string parm;
  in >> parm;
  if (shapes.empty())
  {
    throw std::out_of_range("Error: no shapes!");
  }
  params.at(parm)();
}

void sveshnikov::count(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  std::map< std::string, std::function< size_t() > > params;
  params["EVEN"] = std::bind(count_even, std::cref(shapes));
  params["ODD"] = std::bind(count_odd, std::cref(shapes));

  std::string parm;
  in >> parm;
  size_t num_vertexes = 0;
  try
  {
    size_t len_str = 0;
    int n = std::stoi(parm, &len_str);
    if (len_str != parm.size() || n < 3)
    {
      throw std::invalid_argument("Error: the parameter is not a number!");
    }
    num_vertexes = count_num_vertexes(shapes, n);
  }
  catch (const std::invalid_argument &e)
  {
    num_vertexes = params.at(parm)();
  }
  sveshnikov::StreamGuard guard(out);
  out << num_vertexes << '\n';
}

void sveshnikov::maxseq(is_t &in, os_t &out, const polygon_set_t &shapes)
{
  using namespace std::placeholders;
  Polygon poly;
  in >> poly;
  if (!in)
  {
    throw std::logic_error("Error: incorrect polygon!");
  }
  in >> std::ws;
  if (in.peek() != '\n')
  {
    throw std::invalid_argument("Error: extra characters after command!");
  }

  std::vector< int > match_vect(shapes.size());
  auto equal_curr = std::bind(std::equal_to< Polygon >(), _1, poly);
  std::transform(shapes.begin(), shapes.end(), match_vect.begin(), equal_curr);
  if (match_vect.empty())
  {
    sveshnikov::StreamGuard guard(out);
    out << "0\n";
    return;
  }

  std::vector< int > seqLengths(match_vect.size());
  auto bin_op = std::bind(std::multiplies< int >(), _2, std::bind(std::plus< int >(), _1, 1));
  std::partial_sum(match_vect.begin(), match_vect.end(), seqLengths.begin(), bin_op);
  auto max_it = std::max_element(seqLengths.begin(), seqLengths.end());

  sveshnikov::StreamGuard guard(out);
  out << *max_it << '\n';
}

void sveshnikov::rmecho(is_t &in, os_t &out, polygon_set_t &shapes)
{
  using namespace std::placeholders;
  Polygon poly;
  in >> poly;
  if (!in)
  {
    throw std::invalid_argument("Error: incorrect polygon!");
  }
  in >> std::ws;
  if (in.peek() != '\n')
  {
    throw std::invalid_argument("Error: extra characters after command!");
  }
  size_t old_size = shapes.size();

  auto equal1 = std::bind(std::equal_to< Polygon >(), _1, poly);
  auto equal2 = std::bind(std::equal_to< Polygon >(), _2, poly);
  auto is_dublicate = std::bind(std::logical_and< bool >(), equal1, equal2);
  auto last = std::unique(shapes.begin(), shapes.end(), is_dublicate);
  shapes.erase(last, shapes.end());

  sveshnikov::StreamGuard guard(out);
  out << old_size - shapes.size() << '\n';
}
