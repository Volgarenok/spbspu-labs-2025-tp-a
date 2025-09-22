#include "io-utils.h"
#include <algorithm>
#include <map>
#include <list>
#include <iterator>

orlova::DictPairWrapper returnDictPairWrapper(std::pair< std::string, std::list< std::string > > pair)
{
  return orlova::DictPairWrapper{ pair };
}

std::istream& orlova::operator>>(std::istream& in, std::list< std::string >& list)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  size_t size;
  in >> size;
  if (size == 0)
  {
    return in;
  }
  std::list< std::string > tested;
  auto begin = std::istream_iterator< std::string >(in);
  auto end = std::istream_iterator< std::string >();
  std::copy_n(begin, size, std::back_inserter(tested));
  if (in && tested.size() == size)
  {
    list = std::move(tested);
  }
  return in;
}

std::ostream& orlova::operator<<(std::ostream& out, const Dictionary& dict)
{
  std::ostream::sentry s(out);
  if (!s || dict.empty())
  {
    return out;
  }
  auto it = std::ostream_iterator< DictPairWrapper >(out, "\n");
  std::transform(dict.begin(), dict.end(), it, returnDictPairWrapper);
  return out;
}

std::ostream& orlova::operator<<(std::ostream& out, const std::list< std::string >& list)
{
  std::ostream::sentry s(out);
  if (!s || list.empty())
  {
    return out;
  }
  std::list< std::string > it(list);
  std::copy(it.begin(), it.end(), std::ostream_iterator< std::string >(out, " "));
  return out;
}

std::ostream& orlova::operator<<(std::ostream& out, const DictPairWrapper& pair)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  out << pair.p.first;
  if (!pair.p.second.empty())
  {
    out << ' ' << pair.p.second;
  }
  return out;
}
