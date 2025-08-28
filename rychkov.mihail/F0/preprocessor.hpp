#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <memory>

#include "log.hpp"
#include "content.hpp"
#include "compare.hpp"
#include "lexer.hpp"

namespace rychkov
{
  class Preprocessor
  {
  public:
    std::vector< std::string > include_paths;
    std::unique_ptr< Lexer > next;
    std::set< Macro, NameCompare > macros;
    std::multiset< Macro, NameCompare > legacy_macros;

    Preprocessor();
    Preprocessor(std::unique_ptr< Lexer > lexer, std::vector< std::string > search_dirs);

    static std::string get_name(std::istream& in);
    void parse(CParseContext& context, std::istream& in, bool need_flush = true);
    void append(CParseContext& context, char c);
    void flush(CParseContext& context);
    void flush(CParseContext& context, char c);

  private:
    enum State
    {
      SINGLE_LINE_COMMENT,
      MULTI_LINE_COMMENT,
      STRING_LITERAL,
      CHAR_LITERAL,
      NAME,
      NUMBER,
      MACRO_PARAMETERS,
      DIRECTIVE,
      NO_STATE
    };
    enum IfStage
    {
      IF_BODY,
      WAIT_ELSE,
      ELSE_BODY,
      SKIP_ELSE
    };

    const std::map< std::string, void(Preprocessor::*)(std::istream&, CParseContext&) > directives_ = {
          {"include", &rychkov::Preprocessor::include},
          {"define", &rychkov::Preprocessor::define},
          {"pragma", &rychkov::Preprocessor::pragma},
          {"undef", &rychkov::Preprocessor::undef},
          {"ifdef", &rychkov::Preprocessor::ifdef},
          {"ifndef", &rychkov::Preprocessor::ifndef},
          {"else", &rychkov::Preprocessor::else_cmd},
          {"endif", &rychkov::Preprocessor::endif}
        };

    char prev_ = '\0';
    bool screened_ = false;
    bool empty_line_ = true;
    State state_ = NO_STATE;
    State prev_state_ = NO_STATE;

    const Macro* expansion_ = nullptr;
    size_t parentheses_depth_ = 0;
    std::vector< std::string > expansion_list_;

    std::string buf_;
    std::stack< IfStage > conditional_pairs_;

    static void remove_whitespaces(std::string& str);
    bool skip_all() const noexcept;
    void flush_buf(CParseContext& context);
    void expanse_macro(CParseContext& context);

    void include(std::istream& in, CParseContext& context);
    void define(std::istream& in, CParseContext& context);
    void pragma(std::istream& in, CParseContext& context);
    void undef(std::istream& in, CParseContext& context);
    void ifdef(std::istream& in, CParseContext& context);
    void ifndef(std::istream& in, CParseContext& context);
    void else_cmd(std::istream& in, CParseContext& context);
    void endif(std::istream& in, CParseContext& context);
  };
}

#endif
