#ifndef CPARSER_HPP
#define CPARSER_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <set>
#include <vector>
#include <utility>

#include "type_parser.hpp"
#include "content.hpp"
#include "compare.hpp"
#include "log.hpp"

namespace rychkov
{
  class CParser
  {
  public:
    static const std::set< Operator, rychkov::NameCompare > operators;

    CParser();

    bool append(CParseContext& context, char c);
    bool append(CParseContext& context, entities::Literal literal);
    bool append(CParseContext& context, std::string name);
    bool append(CParseContext& context, const std::vector< rychkov::Operator >& cases);

    void print(std::ostream& out) const;
  private:
    static constexpr int min_priority = -1;
    static const Operator parentheses;
    static const Operator brackets;
    static const Operator comma;

    std::vector< entities::Expression > program_;
    std::set< std::pair< typing::Type, size_t >, NameCompare > base_types_;
    std::set< entities::Alias > aliases_;
    std::set< std::pair< entities::Variable, size_t >, NameCompare > variables_;
    std::set< entities::Variable, NameCompare > defined_functions_;
    std::set< std::pair< entities::Struct, size_t >, NameCompare > structs_;
    std::set< std::pair< entities::Union, size_t > > unions_;
    std::set< std::pair< entities::Enum, size_t > > enums_;
    std::stack< entities::Expression* > stack_;
    TypeParser type_parser_;

    bool global_scope() const noexcept;

    bool flush_type_parser(CParseContext& context);
    bool append_empty(CParseContext& context);
    entities::Expression* move_up();
    void move_down(CParseContext& context);
    void fold(CParseContext& context, const Operator* reference = nullptr);
    void calculate_type(CParseContext& context, entities::Expression& expr);
    void require_type(CParseContext& context, entities::Expression::operand& expr, const typing::Type& type);

    bool parse_semicolon(CParseContext& context);
    bool parse_open_brace(CParseContext& context);
    bool parse_close_brace(CParseContext& context);
    bool parse_open_parenthesis(CParseContext& context);
    bool parse_close_parenthesis(CParseContext& context);
    bool parse_open_bracket(CParseContext& context);
    bool parse_close_bracket(CParseContext& context);
    bool parse_comma(CParseContext& context);

    bool parse_unary(CParseContext& context, const Operator& oper);
    bool parse_binary(CParseContext& context, const Operator& oper);
  };
}

#endif
