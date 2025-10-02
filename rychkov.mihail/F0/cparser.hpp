#ifndef CPARSER_HPP
#define CPARSER_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <set>
#include <map>
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
    enum TypeKeyword
    {
      CONST,
      VOLATILE,
      SIGNED,
      UNSIGNED,
      LONG
    };
    using value_type = entities::Expression;

    static const Operator parentheses;
    static const Operator brackets;
    static const Operator comma;
    static const Operator inline_if;

    std::set< entities::Alias, NameCompare > aliases;
    std::multiset< std::pair< entities::Variable, size_t >, NameCompare > variables;
    std::multiset< entities::Variable, NameCompare > defined_functions;

    std::set< std::pair< entities::Struct, size_t >, NameCompare > structs;
    std::set< std::pair< entities::Union, size_t >, NameCompare > unions;
    std::set< std::pair< entities::Enum, size_t >, NameCompare > enums;
    std::set< std::pair< typing::Type, size_t >, NameCompare > base_types = {
          {{"int", typing::BASIC}, 0},
          {{"char", typing::BASIC}, 0},
          {{"float", typing::BASIC}, 0},
          {{"double", typing::BASIC}, 0},
          {{"void", typing::BASIC}, 0}
        };

    CParser();
    CParser(const CParser&) = delete;
    CParser(CParser&&) = default;

    template< class T >
    static void clear_scope(T& pair_set, size_t scope);

    std::vector< entities::Expression >::const_iterator begin() const;
    std::vector< entities::Expression >::const_iterator end() const;
    const TypeParser& next() const;
    void prepare_type();
    void clear_program();
    void push_back(entities::Expression expr);

    void append(CParseContext& context, char c);
    void append(CParseContext& context, entities::Literal literal);
    void append(CParseContext& context, std::string name);
    void append(CParseContext& context, const std::vector< rychkov::Operator >& cases);
    void append(CParseContext& context, TypeKeyword keyword);

    void parse_typedef(CParseContext& context);
    void parse_struct(CParseContext& context);
    void parse_union(CParseContext& context);
    void parse_enum(CParseContext& context);
    void parse_return(CParseContext& context);
    void parse_if(CParseContext& context);
    void parse_else(CParseContext& context);
    void parse_do(CParseContext& context);
    void parse_while(CParseContext& context);
    void parse_for(CParseContext& context);

  private:
    static constexpr int min_priority = -1;

    const std::map< TypeKeyword, void(TypeParser::*)(CParseContext&) > type_keywords = {
          {CONST, &TypeParser::append_const},
          {VOLATILE, &TypeParser::append_volatile},
          {SIGNED, &TypeParser::append_signed},
          {UNSIGNED, &TypeParser::append_unsigned},
          {LONG, &TypeParser::append_long}
        };

    std::vector< entities::Expression > program_;
    std::stack< entities::Expression* > stack_;
    TypeParser type_parser_;

    bool global_scope() const noexcept;

    bool flush_type_parser(CParseContext& context);
    bool append_empty(CParseContext& context);
    entities::Expression* move_up();
    void move_down();
    void move_up_down();
    void fold(CParseContext& context, const Operator* reference = nullptr);
    void clear_scope();
    void calculate_type(CParseContext& context, entities::Expression& expr);
    void require_type(CParseContext& context, entities::Expression::operand& expr, const typing::Type& type);
    void require_type(CParseContext& context, entities::Expression& expr, const typing::Type& type);
    std::pair< const entities::Variable*, typing::MatchType > find_overload(const std::string& name,
        const std::vector< typing::Type >& args);

    void parse_semicolon(CParseContext& context);
    void parse_open_brace(CParseContext& context);
    void parse_close_brace(CParseContext& context);
    void parse_open_parenthesis(CParseContext& context);
    void parse_close_parenthesis(CParseContext& context);
    void parse_open_bracket(CParseContext& context);
    void parse_close_bracket(CParseContext& context);
    void parse_comma(CParseContext& context);
    void parse_question_mark(CParseContext& context);
    void parse_colon(CParseContext& context);

    void check_statement_placement(CParseContext& context);

    bool parse_unary(CParseContext& context, const Operator& oper);
    bool parse_binary(CParseContext& context, const Operator& oper);
  };
}

template< class T >
void rychkov::CParser::clear_scope(T& pair_set, size_t scope)
{
  typename T::iterator pos = pair_set.begin();
  while (pos != pair_set.end())
  {
    if (pos->second <= scope)
    {
      pos = pair_set.upper_bound(entities::get_name(pos->first));
      continue;
    }
    typename T::iterator to = pair_set.lower_bound({pos->first, scope});
    if (pos == to)
    {
      ++pos;
      continue;
    }
    pos = pair_set.erase(pos, to);
  }
}

#endif
