#include "parser.hpp"

#include <iostream>
#include <cctype>
#include <iterator>
#include <algorithm>
#include <fmtguard.hpp>

bool rychkov::eol(std::istream& in)
{
  if (in.eof())
  {
    return true;
  }
  if (!std::isspace(in.peek()))
  {
    return false;
  }
  struct remove_empty
  {
    std::istream& in;
    bool& bad;
    bool operator()(char c)
    {
      if (c == '\n')
      {
        bad = false;
        return true;
      }
      char next = in.peek();
      if (in.eof() || (next == '\n'))
      {
        bad = false;
        return true;
      }
      if (!std::isspace(next))
      {
        bad = true;
        return true;
      }
      return false;
    }
  };
  using Iter = std::istream_iterator< char >;
  fmtguard guard{in};
  bool bad = false;
  Iter trash = std::find_if(Iter{in >> std::noskipws}, Iter{}, remove_empty{in, bad});
  return !bad;
}
void rychkov::ParserContext::parse_error()
{
  out << "<INVALID COMMAND>\n";
  in.clear(in.rdstate() & ~std::ios::failbit);
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
}
