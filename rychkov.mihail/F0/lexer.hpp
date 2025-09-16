#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <set>
#include <map>
#include <vector>
#include <memory>
#include <boost/variant2.hpp>

#include "content.hpp"
#include "compare.hpp"
#include "log.hpp"
#include "cparser.hpp"

namespace rychkov
{
  class Lexer
  {
  public:
    static const std::set< std::vector< Operator >, NameCompare > cases;
    std::unique_ptr< CParser > next;

    Lexer(std::unique_ptr< CParser > cparser = nullptr);

    void append_name(CParseContext& context, std::string name);
    void append_number(CParseContext& context, std::string name);
    void append_string_literal(CParseContext& context, std::string name);
    void append_char_literal(CParseContext& context, std::string name);
    void append(CParseContext& context, char c);
    void flush(CParseContext& context);

  private:
    using operator_value = const std::vector< Operator >*;

    const std::map< std::string, CParser::TypeKeyword > type_keywords_;
    const std::map< std::string, void(CParser::*)(CParseContext&) > keywords_;

    boost::variant2::variant< boost::variant2::monostate, operator_value, entities::Literal > buf_;

    void append_new(CParseContext& context, char c);
    void append_operator(CParseContext& context, char c);
  };
}

#endif
