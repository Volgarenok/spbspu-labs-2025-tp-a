#ifndef CODE_PARSERS_HPP
#define CODE_PARSERS_HPP

#include <iosfwd>
#include <stack>
#include <set>
#include <string>
#include <utility>
#include <parser.hpp>
#include "content.hpp"
#include "compare.hpp"

namespace rychkov
{
  struct CParseContext
  {
    std::ostream& out;
    std::ostream& err;
    std::string file;
    CParseContext* base = nullptr;
    size_t line = 0, symbol = 0;
    std::string last_line;
    size_t nerrors = 0;
  };

  class TypeParser
  {
  public:
    bool empty() const noexcept;
    bool ready() const noexcept;
    bool is_function() const noexcept;

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

    typing::Type* move_up();
    bool can_be_named_param(typing::Type* type_p);
  };

  class Lexer;
  class CParser;
  class Preprocessor
  {
  public:
    Preprocessor();
    Preprocessor(Lexer* next, std::vector< std::string > search_dirs);

    void parse(CParseContext& context, std::istream& in);
    void parse(CParseContext& context, std::string line);
    void flush(CParseContext& context);
    void flush(CParseContext& context, char c);

  private:
    struct CommandContext
    {
      std::istream& in;
      std::ostream& err;
      CParseContext& parse_context;

      void parse_error();
    };
    static Parser::map_type< CommandContext, Preprocessor > call_map;

    Lexer* next_;
    bool multiline_comment = false;
    std::string last_line;
    std::vector< std::string > include_dirs_;

    void append(CParseContext& context, char c);

    bool include(CommandContext&);
    bool define(CommandContext&);
    bool pragma(CommandContext&);
    bool undef(CommandContext&);
  };

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
    std::set< std::pair< typing::Type, size_t >, NameCompare > base_types_; // data and declare-depth
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
  void log(CParseContext& context, std::string message);
}

#endif
