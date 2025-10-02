#include "shape-utils.hpp"
#include <limits>
#include <iterator>
#include <algorithm>
#include "rectangle.hpp"
#include "concave.hpp"
#include "complexquad.hpp"

namespace
{
  bool read_n(std::istream & in, double * out, std::size_t count)
  {
    if (!std::copy_n(std::istream_iterator< double >(in), count, out))
    {
      in.clear(in.rdstate() ^ std::ios::failbit);
      in.ignore(std::numeric_limits <std::streamsize >::max(), '\n');
      return false;
    }
    return true;
  }
}

savintsev::Shape * savintsev::createShape(std::istream & in, std::string name)
{
  std::string title;
  in >> title;
  if (name == "rectangle")
  {
    double n[4];
    return read_n(in, n, 4) ? new Rectangle({n[0], n[1]}, {n[2], n[3]}, title) : nullptr;
  }
  if (name == "complexquad")
  {
    double n[8];
    return read_n(in, n, 8) ? new Complexquad({n[0], n[1]}, {n[2], n[3]}, {n[4], n[5]}, {n[6], n[7]}, title) : nullptr;
  }
  if (name == "concave")
  {
    double n[8];
    return read_n(in, n, 8) ? new Concave({n[0], n[1]}, {n[2], n[3]}, {n[4], n[5]}, {n[6], n[7]}, title) : nullptr;
  }
  return nullptr;
}
