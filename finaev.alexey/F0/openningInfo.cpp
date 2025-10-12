#include "openningInfo.hpp"
#include <delimiter.hpp>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <numeric>

namespace
{
  struct Accumulator
  {
    std::vector< std::string > vect;
    std::string current;

    Accumulator process(char c)
    {
      Accumulator res = *this;
      if (c == ' ')
      {
        if (!res.current.empty())
        {
          res.vect.push_back(res.current);
          res.current.clear();
        }
      }
      else
      {
        res.current += c;
      }
      return res;
    }
  };

  struct AccumulatorProc
  {
    Accumulator operator()(Accumulator a, char c) const
    {
      return a.process(c);
    }
  };

  std::vector< std::string > parsStr(std::string s)
  {
    Accumulator temp;
    AccumulatorProc proc;
    Accumulator res = std::accumulate(s.begin(), s.end(), temp, proc);
    if (!res.current.empty())
    {
      res.vect.push_back(res.current);
    }
    return res.vect;
  }
}

std::istream& finaev::operator>>(std::istream& in, DebutMoves& m)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'"'};
  std::vector< std::string > temp;
  std::string input;
  std::getline(in, input, '"');
  temp = parsStr(input);
  if (in)
  {
    m.moves = std::move(temp);
  }
  return in;
}

bool finaev::DebutMoves::containsSequence(const std::vector< std::string >& sequence) const
{
  if (sequence.empty())
  {
    return false;
  }
  auto it = std::search( moves.begin(), moves.end(), sequence.begin(), sequence.end());
  return it != moves.end();
}
