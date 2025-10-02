#ifndef PARSER_HPP
#define PARSER_HPP

#include <iosfwd>
#include <istream>
#include <string>
#include <utility>
#include <map>
#include <limits>

namespace rychkov
{
  bool eol(std::istream& in);
  struct ParserContext
  {
    std::istream& in;
    std::ostream& out;
    std::ostream& err;

    void parse_error();
  };
  struct Parser
  {
    template< class Context, class Proc >
    using call_signature = bool(Proc::*)(Context&);
    template< class Context, class Proc >
    using map_type = std::map< std::string, call_signature< Context, Proc > >;

    template< class Context = ParserContext, class Proc >
    static bool parse(Context context, Proc& processor, const map_type< Context, Proc >& call_map)
    {
      if (call_map.empty() || !context.in)
      {
        return false;
      }
      std::string command;
      if (context.in >> command)
      {
        typename map_type< Context, Proc >::const_iterator call_p = call_map.find(command);
        try
        {
          if ((call_p != call_map.end()) && ((processor.*(call_p->second))(context)))
          {
            return true;
          }
        }
        catch (...)
        {}
        context.parse_error();
      }
      return true;
    }
  };
}

#endif
