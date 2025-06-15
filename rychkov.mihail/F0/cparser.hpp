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
    static const std::vector< Operator > operators;

    CParser();

    bool append(CParseContext& context, char c);
    bool append(CParseContext& context, entities::Literal literal);
    bool append(CParseContext& context, std::string name);
    bool append(CParseContext& context, const std::vector< rychkov::Operator >& cases);

    void print(std::ostream& out) const;
  private:
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
    void move_down();

    bool parse_semicolon(CParseContext& context);
    bool parse_open_brace(CParseContext& context);
    bool parse_close_brace(CParseContext& context);

    bool parse_unary(CParseContext& context, const Operator& oper);
    bool parse_binary(CParseContext& context, const Operator& oper);
  };
}

#endif
