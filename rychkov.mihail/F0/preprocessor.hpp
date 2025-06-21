#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <iosfwd>
#include <cstddef>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <parser.hpp>
#include "log.hpp"
#include "content.hpp"
#include "compare.hpp"

namespace rychkov
{
  class Lexer;
  class Preprocessor
  {
  public:
    Preprocessor();
    Preprocessor(Lexer* next, std::vector< std::string > search_dirs);

    void parse(CParseContext& context, std::istream& in, bool need_flush = true);
    void parse(CParseContext& context, std::string line);
    void flush(CParseContext& context);
    void flush(CParseContext& context, char c);
    void flush_buf(CParseContext& context);

  private:
    enum IfStage
    {
      IF_BODY,
      WAIT_ELSE,
      ELSE_BODY,
      SKIP_ELSE
    };
    struct CommandContext
    {
      std::istream& in;
      std::ostream& err;
      CParseContext& parse_context;

      void parse_error();
    };
    static Parser::map_type< CommandContext, Preprocessor > call_map;

    Lexer* next_;
    bool multiline_comment_ = false;
    bool directive_ = false;
    bool empty_line_ = true;
    const Macro* expansion_ = nullptr;
    size_t parentheses_depth_ = 0;

    std::stack< IfStage > conditional_pairs_;
    std::vector< std::string > expansion_list_;
    std::string buf_;
    std::vector< std::string > include_dirs_;
    std::set< Macro, NameCompare > macros_;

    static std::string get_name(std::istream& in);
    static void remove_whitespaces(std::string& str);
    void append(CParseContext& context, char c);
    void expanse_macro(CParseContext& context);

    bool include(CommandContext&);
    bool define(CommandContext&);
    bool pragma(CommandContext&);
    bool undef(CommandContext&);
    bool ifdef(CommandContext&);
    bool ifndef(CommandContext&);
    bool else_cmd(CommandContext&);
    bool endif(CommandContext&);
  };
}

#endif
