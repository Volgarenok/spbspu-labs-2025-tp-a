#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include <parser.hpp>
#include "log.hpp"

namespace rychkov
{
  class Lexer;
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
}

#endif
