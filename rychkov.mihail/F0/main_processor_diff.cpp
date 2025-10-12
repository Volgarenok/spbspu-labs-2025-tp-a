#include "main_processor.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
#include "compare.hpp"
#include "print_content.hpp"

namespace rychkov
{
  struct DiffVisitor
  {
    std::set< std::string > files;
    std::multimap< entities::Variable, std::vector< std::string >, NameCompare > appearances;
    std::multimap< Macro, std::vector< std::string >, NameCompare > macros;
    std::string current_file;

    void operator()(const std::pair< const std::string, ParseCell >& cell)
    {
      current_file = cell.first;
      if (!files.empty() && (files.find(current_file) == files.end()))
      {
        return;
      }
      const Preprocessor& preproc = cell.second.preproc;
      const CParser& parser = *preproc.next->next;
      *this = std::for_each(parser.begin(), parser.end(), std::move(*this));
      *this = std::for_each(preproc.macros.begin(), preproc.macros.end(), std::move(*this));
      *this = std::for_each(preproc.legacy_macros.begin(), preproc.legacy_macros.end(), std::move(*this));
    }
    void operator()(const entities::Expression& expr)
    {
      if (entities::is_decl(expr))
      {
        const entities::Declaration& decl = boost::variant2::get< entities::Declaration >(expr.operands[0]);
        entities::Variable declared;
        if (boost::variant2::holds_alternative< entities::Variable >(decl.data))
        {
          operator()(boost::variant2::get< entities::Variable >(decl.data));
        }
        else if (boost::variant2::holds_alternative< entities::Function >(decl.data))
        {
          const entities::Function& func = boost::variant2::get< entities::Function >(decl.data);
          operator()({func.type, func.name});
        }
      }
    }
    void operator()(const entities::Variable& var)
    {
      using Iter = decltype(appearances)::iterator;
      std::pair< Iter, Iter > range = appearances.equal_range(var);
      for (; range.first != range.second; ++range.first)
      {
        if (var.name != range.first->first.name)
        {}
        else if (!typing::exact_cv(range.first->first.type, var.type))
        {}
        else if (range.first->second.empty() || (range.first->second.back() != current_file))
        {
          range.first->second.push_back(current_file);
          break;
        }
      }
      if (range.first == range.second)
      {
        appearances.emplace(var, std::vector< std::string >{current_file});
      }
    }
    void operator()(const Macro& macro)
    {
      using Iter = decltype(macros)::iterator;
      std::pair< Iter, Iter > range = macros.equal_range(macro);
      for (; range.first != range.second; ++range.first)
      {
        const std::vector< std::string >& parameters = range.first->first.parameters;
        if (macro.name != range.first->first.name)
        {}
        else if (macro.func_style != range.first->first.func_style)
        {}
        else if (!std::equal(parameters.begin(), parameters.end(),
              macro.parameters.begin(), macro.parameters.end()))
        {}
        else if (macro.body != range.first->first.body)
        {}
        else if (range.first->second.empty() || (range.first->second.back() != current_file))
        {
          range.first->second.push_back(current_file);
          break;
        }
      }
      if (range.first == range.second)
      {
        macros.emplace(macro, std::vector< std::string >{current_file});
      }
    }
  };
}

bool rychkov::MainProcessor::intersections(ParserContext& context)
{
  if ((last_stage_ != CPARSER) || !eol(context.in))
  {
    return false;
  }
  DiffVisitor visitor = std::for_each(parsed_.begin(), parsed_.end(), DiffVisitor{});
  bool empty = true;
  for (const decltype(visitor.appearances)::value_type& list: visitor.appearances)
  {
    if (list.second.size() >= 2)
    {
      empty = false;
      ContentPrinter{context.out, 0}(list.first);
      std::for_each(list.second.begin(), list.second.end(), ContentPrinter{context.out, 1});
    }
  }
  for (const decltype(visitor.macros)::value_type& list: visitor.macros)
  {
    if (list.second.size() >= 2)
    {
      empty = false;
      ContentPrinter{context.out, 0}(list.first);
      std::for_each(list.second.begin(), list.second.end(), ContentPrinter{context.out, 1});
    }
  }
  if (empty)
  {
    context.out << "no identical symbols in file\n";
  }
  return true;
}
bool rychkov::MainProcessor::diff(ParserContext& context)
{
  std::set< std::string > files;
  if (last_stage_ != CPARSER)
  {
    return false;
  }
  while (!eol(context.in))
  {
    std::string name;
    context.in >> name;
    files.insert(name);
  }
  DiffVisitor visitor = std::for_each(parsed_.begin(), parsed_.end(), DiffVisitor{std::move(files)});
  bool empty = true;
  using MacroIter = decltype(visitor.macros)::const_iterator;
  MacroIter i = visitor.macros.begin();
  while (i != visitor.macros.end())
  {
    MacroIter to = visitor.macros.upper_bound(i->first.name);
    if (std::distance(i, to) > 1)
    {
      empty = false;
      ContentPrinter{context.out, 0}(i->first.name);
      std::set< std::string > files_list;
      for (; i != to; ++i)
      {
        files_list.insert(i->second.begin(), i->second.end());
      }
      std::for_each(files_list.begin(), files_list.end(), ContentPrinter{context.out, 1});
    }
    else
    {
      ++i;
    }
  }
  using VarIter = decltype(visitor.appearances)::const_iterator;
  VarIter j = visitor.appearances.begin();
  while (j != visitor.appearances.end())
  {
    VarIter to = visitor.appearances.upper_bound(j->first.name);
    if (std::distance(j, to) > 1)
    {
      empty = false;
      ContentPrinter{context.out, 0}(j->first.name);
      std::set< std::string > files_list;
      for (; j != to; ++j)
      {
        files_list.insert(j->second.begin(), j->second.end());
      }
      std::for_each(files_list.begin(), files_list.end(), ContentPrinter{context.out, 1});
    }
    else
    {
      ++j;
    }
  }
  if (empty)
  {
    context.out << "no diff symbols in file\n";
  }
  return true;
}
