#include <algorithm>
#include <iterator>
#include "dictionary.h"

namespace
{
  using namespace fedorova;

  std::pair< std::string, std::list< std::string > > returnDictPair(DictPairWrapper pair)
  {
    return pair.p;
  }
  std::pair< std::string, fedorova::Dictionary > returnDictSetPair(DictSetPairWrapper pair)
  {
    return pair.p;
  }
  DictPairWrapper returnDictPairWrapper(std::pair< std::string, std::list< std::string > > pair)
  {
    return DictPairWrapper{ pair };
  }
  DictSetPairWrapper returnDictSetPairWrapper(std::pair< std::string, Dictionary > pair)
  {
    return DictSetPairWrapper{ pair };
  }
  ContentPairWrapper returnContentPairWrapper(std::pair< char, std::list< std::string > > pair)
  {
    return ContentPairWrapper{ pair };
  }
}

std::istream& fedorova::operator>>(std::istream& is, std::list< std::string >& list)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  size_t size;
  is >> size;
  if (size == 0)
  {
    return is;
  }

  std::list< std::string > tested;
  auto begin = std::istream_iterator< std::string >(is);
  auto end = std::istream_iterator< std::string >();
  std::copy_n(begin, size, std::back_inserter(tested));
  if (is.get() != '\n')
  {
    if (is.eof())
    {
      is.clear(is.rdstate() ^ std::ios::eofbit);
    }
    is.setstate(std::ios::failbit);
  }

  if (is && tested.size() == size)
  {
    list = tested;
  }

  return is;
}

std::istream& fedorova::operator>>(std::istream& is, Dictionary& dict)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  size_t size;
  is >> size;
  if (!is)
  {
    is.setstate(std::ios::failbit);
    return is;
  }

  std::list< DictPairWrapper > tested_list;
  auto begin = std::istream_iterator< DictPairWrapper >(is);
  auto end = std::istream_iterator< DictPairWrapper >();
  std::copy_n(begin, size, std::back_inserter(tested_list));
  if (is && tested_list.size() == size)
  {
    std::transform(tested_list.begin(), tested_list.end(), std::inserter(dict, dict.end()), returnDictPair);
  }

  return is;
}

std::istream& fedorova::operator>>(std::istream& is, DictSet& set)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  DictSet tested;
  std::list< DictSetPairWrapper > tested_list;
  auto begin = std::istream_iterator< DictSetPairWrapper >(is);
  auto end = std::istream_iterator< DictSetPairWrapper >();
  std::transform(begin, end, std::inserter(tested, tested.end()), returnDictSetPair);
  if (is.eof())
  {
    set = tested;
  }

  return is;
}

std::istream& fedorova::operator>>(std::istream& is, DictPairWrapper& pair)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  DictPairWrapper tested;
  is >> tested.p.first >> tested.p.second;
  if (is)
  {
    pair = tested;
  }
  return is;
}

std::istream& fedorova::operator>>(std::istream& is, DictSetPairWrapper& pair)
{
  std::istream::sentry sentry(is);
  if (!sentry)
  {
    return is;
  }

  DictSetPairWrapper tested;
  is >> tested.p.first >> tested.p.second;
  if (is)
  {
    pair = tested;
  }

  return is;
}


