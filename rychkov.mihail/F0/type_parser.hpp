#ifndef TYPE_PARSER_HPP
#define TYPE_PARSER_HPP

#include <string>
#include <stack>
#include "log.hpp"
#include "content.hpp"

namespace rychkov
{
  class TypeParser
  {
  public:
    bool empty() const noexcept;
    bool ready() const noexcept;
    bool is_type() const noexcept;
    bool is_function() const noexcept;

    void prepare();
    typing::Type type() const;
    entities::Variable variable() const;
    entities::Function function() const;
    std::string name() const;
    void clear();

    void append(CParseContext& context, char c);
    void append(CParseContext& context, std::string name);
    void append(CParseContext& context, unsigned long long numeric_literal);
    void append(CParseContext& context, typing::Type base_type);

    void append_const(CParseContext& context);
    void append_volatile(CParseContext& context);
    void append_signed(CParseContext& context);
    void append_unsigned(CParseContext& context);
    void append_long(CParseContext& context);

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

    void append_asterisk(CParseContext& context);
    void append_open_bracket(CParseContext& context);
    void append_close_bracket(CParseContext& context);
    void append_open_parenthesis(CParseContext& context);
    void append_close_parenthesis(CParseContext& context);
    void append_comma(CParseContext& context);

    static void remove_combination(typing::Type& type);
    typing::Type* move_up();
    bool can_be_named_param(typing::Type* type_p);
  };
}

#endif
