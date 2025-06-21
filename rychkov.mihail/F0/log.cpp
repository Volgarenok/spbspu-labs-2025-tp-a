#include "log.hpp"

#include <iostream>

void rychkov::log(CParseContext& context, std::string message)
{
  start_log(context) << message;
  finish_log(context);
}
std::ostream& rychkov::start_log(CParseContext& context)
{
  context.err << (context.macro_expansion ? "In expansion of macro \"" : "In file \"") << context.file;
  context.err << "\" (" << context.line + 1 << ':' << context.symbol + 1 << ")\n";
  context.err << "\tError: ";
  return context.err;
}
void rychkov::finish_log(CParseContext& context)
{
  context.err << "\n  ";
  context.err << context.last_line << "\n  " << std::string(context.symbol, '-') << "^\n";
  for (const CParseContext* file = context.base; file != nullptr; file = file->base)
  {
    context.err << (file->macro_expansion ? "Required from expansion of macro \"" : "Included from \"");
    context.err << file->file << "\" (" << file->line + 1 << ':' << file->symbol + 1 << ")\n  ";
    context.err << file->last_line << "\n  " << std::string(file->symbol, '-') << "^\n";
  }
  context.nerrors++;
}
