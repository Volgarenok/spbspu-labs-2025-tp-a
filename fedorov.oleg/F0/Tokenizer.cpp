#include "Tokenizer.hpp"

namespace crossref
{

  namespace
  {
    struct TokenAdder
    {
      std::vector< std::string > &tokens;
      std::string &current;

      TokenAdder(std::vector< std::string > &t, std::string &c):
        tokens(t),
        current(c)
      {}

      void operator()(char c) const
      {
        if (std::isspace(c))
        {
          if (!current.empty())
          {
            tokens.push_back(current);
            current.clear();
          }
        }
        else
        {
          current += c;
        }
      }
    };
  }

  std::vector< std::string > Tokenizer::tokenize(const std::string &input)
  {
    std::vector< std::string > tokens;
    std::string current;

    std::for_each(input.begin(), input.end(), TokenAdder(tokens, current));

    if (!current.empty())
    {
      tokens.push_back(current);
    }

    return tokens;
  }

}
