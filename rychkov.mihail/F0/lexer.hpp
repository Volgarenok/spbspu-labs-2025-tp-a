#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <set>
#include <vector>
#include <variant>

#include "content.hpp"
#include "compare.hpp"
#include "log.hpp"

namespace rychkov
{
  class CParser;
  class Lexer
  {
  public:
    static const std::set< std::vector< Operator >, NameCompare > cases;

    Lexer(CParser* next = nullptr);

    void parse(CParseContext& context, std::string str);
    void append(CParseContext& context, char c);
    void flush(CParseContext& context);

  private:
    using operator_value = const std::vector< Operator >*;
    CParser* next_;
    bool literal_full_;
    std::variant< std::monostate, operator_value, std::string, entities::Literal > buf_;

    void append_new(CParseContext& context, char c);
    void append_name(CParseContext& context, char c);
    void append_literal(CParseContext& context, char c);
    void append_operator(CParseContext& context, char c);
  };
}

#endif
