#ifndef TYPE_PARSER_HPP
#define TYPE_PARSER_HPP

#include <stack>
#include <set>
#include <string>
#include <cstddef>
#include "log.hpp"
#include "content.hpp"

namespace rychkov
{
  class TypeParser
  {
  public:
    bool empty() const noexcept;
    bool ready() const noexcept;
    bool is_function() const noexcept;

    void prepare();
    typing::Type type() const;
    entities::Variable variable() const;
    entities::Function function() const;
    void clear();

    bool append(CParseContext& context, char c);
    bool append(CParseContext& context, std::string name);
    bool append(CParseContext& context, size_t numeric_literal);
    bool append(CParseContext& context, typing::Type base_type);

  private:
    struct ParseCell
    {
      typing::Type* data;
      bool right_allign = false;
      bool bracket_opened = false;
      bool parenthesis_opened = false;
      bool array_must_have_size = false;
      bool is_function_paremeter = false;
    };
    typing::Type combined_;
    std::stack< ParseCell > stack_;
    std::string var_name_;
    std::vector< std::string > parameters_;

    bool append_const(CParseContext& context);
    bool append_volatile(CParseContext& context);
    bool append_signed(CParseContext& context);
    bool append_unsigned(CParseContext& context);

    bool append_asterisk(CParseContext& context);
    bool append_open_bracket(CParseContext& context);
    bool append_close_bracket(CParseContext& context);
    bool append_open_parenthesis(CParseContext& context);
    bool append_close_parenthesis(CParseContext& context);
    bool append_comma(CParseContext& context);

    static void remove_combination(typing::Type& type);
    typing::Type* move_up();
    bool can_be_named_param(typing::Type* type_p);
  };
}

#endif
