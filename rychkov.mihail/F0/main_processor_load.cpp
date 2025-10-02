#include "main_processor.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <utility>
#include <stdexcept>
#include <boost/json.hpp>

namespace rychkov
{
  struct Loader
  {
    CParser& parser;
    Preprocessor preproc{std::unique_ptr< Lexer >{new Lexer{std::unique_ptr< CParser >{new CParser}}}, {}};
    size_t depth = 1;

    static Macro as_macro(const boost::json::value& val);
    void clear_scope();

    void operator()(const boost::json::value& val);
    entities::Variable as_var_data(const boost::json::string& str);
    entities::Expression as_expr(const boost::json::value& val);
    entities::Expression::operand as_operand(const boost::json::value& val);
    entities::Literal as_lit(const boost::json::value& val);
    entities::Variable as_var(const boost::json::value& val);
    entities::Function as_func(const boost::json::value& val);
    entities::Struct as_struct(const boost::json::value& val);
    entities::Enum as_enum(const boost::json::value& val);
    entities::Union as_union(const boost::json::value& val);
    entities::Alias as_alias(const boost::json::value& val);
    entities::Statement as_statement(const boost::json::value& val);
    entities::Declaration as_decl(const boost::json::value& val);
    entities::Declaration::declared as_declared(const boost::json::value& val);
    entities::CastOperation as_cast(const boost::json::value& val);
    entities::Body as_body(const boost::json::value& val);
  };
}

bool rychkov::MainProcessor::load(std::ostream& out, std::ostream& err, std::string filename)
{
  std::ifstream in(filename);
  if (!in)
  {
    err << "failed to open save file on read - \"" << filename << "\"\n";
    return false;
  }
  boost::json::value doc = boost::json::parse(in);
  std::map< std::string, ParseCell > new_parsed;
  size_t ngenerated = 0;
  for (const boost::json::object::value_type& file: doc.as_object())
  {
    std::pair< decltype(new_parsed)::iterator, bool > cell_p = new_parsed.emplace(file.key(),
          ParseCell{{out, err, file.key()}, last_stage_, include_dirs_});
    if (cell_p.second)
    {
      Preprocessor& preproc = cell_p.first->second.preproc;
      out << "<--LOAD: \"" << file.key() << "\"-->\n";
      const boost::json::object& obj = file.value().as_object();

      const boost::json::array& macros = obj.at("macros").as_array();
      std::transform(macros.begin(), macros.end(),
            std::inserter(preproc.macros, preproc.macros.end()), Loader::as_macro);
      const boost::json::array& legacy = obj.at("old_macro").as_array();
      std::transform(legacy.begin(), legacy.end(),
            std::inserter(preproc.legacy_macros, preproc.legacy_macros.end()), Loader::as_macro);

      const boost::json::array& pgm = obj.at("pgm").as_array();
      CParser& parser = *preproc.next->next;
      Loader loader = std::for_each(pgm.begin(), pgm.end(), Loader{parser});
      cell_p.first->second.real_file = obj.at("real").as_bool();
      ngenerated += !cell_p.first->second.real_file;
      cell_p.first->second.cache = boost::json::value_to< std::string >(obj.at("cache"));

      parser.defined_functions = loader.preproc.next->next->defined_functions;
      parser.base_types = loader.preproc.next->next->base_types;
      parser.variables = loader.preproc.next->next->variables;
      parser.structs = loader.preproc.next->next->structs;
      parser.unions = loader.preproc.next->next->unions;
      parser.enums = loader.preproc.next->next->enums;
      parser.aliases = loader.preproc.next->next->aliases;
      parser.defined_functions = loader.preproc.next->next->defined_functions;
    }
  }
  parsed_ = std::move(new_parsed);
  generated_files = ngenerated;
  return true;
}
rychkov::Macro rychkov::Loader::as_macro(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  rychkov::Macro result{boost::json::value_to< std::string >(obj.at("name")),
        boost::json::value_to< std::string >(obj.at("body")), obj.at("fstyle").as_bool()};
  const boost::json::array& params = obj.at("params").as_array();
  result.parameters.reserve(params.size());
  std::transform(params.begin(), params.end(), std::back_inserter(result.parameters),
        boost::json::value_to< std::string >);
  return result;
}
void rychkov::Loader::clear_scope()
{
  CParser::clear_scope(preproc.next->next->base_types, depth);
  CParser::clear_scope(preproc.next->next->variables, depth);
  CParser::clear_scope(preproc.next->next->structs, depth);
  CParser::clear_scope(preproc.next->next->unions, depth);
  CParser::clear_scope(preproc.next->next->enums, depth);
}
void rychkov::Loader::operator()(const boost::json::value& val)
{
  parser.push_back(as_expr(val));
}

rychkov::entities::Expression rychkov::Loader::as_expr(const boost::json::value& val)
{
  if (val.is_array())
  {
    return as_body(val);
  }
  const boost::json::object& obj = val.as_object();
  if (obj.empty())
  {
    return {};
  }
  if (obj.at("obj") == "expr")
  {
    Operator::Type type = static_cast< Operator::Type >(obj.at("size").as_int64());
    if ((type != Operator::MULTIPLE) && ((type > 3) || (type == 0)))
    {
      throw std::invalid_argument{"wrong operator size"};
    }
    Operator oper{type};
    oper.token = boost::json::value_to< std::string >(obj.at("token"));
    oper.right_align = obj.at("rallign").as_bool();
    decltype(Lexer::cases)::const_iterator cases = Lexer::cases.find(oper.token);
    bool found = false;
    entities::Expression result;
    if (cases != Lexer::cases.end())
    {
      for (const Operator& i: *cases)
      {
        if ((i.right_align == oper.right_align) && (i.type == oper.type))
        {
          result.operation = &i;
          found = true;
        }
      }
    }
    else
    {
      found = true;
      if (oper.token == CParser::parentheses.token)
      {
        result.operation = &CParser::parentheses;
      }
      else if (oper.token == CParser::brackets.token)
      {
        result.operation = &CParser::brackets;
      }
      else if (oper.token == CParser::comma.token)
      {
        result.operation = &CParser::comma;
      }
      else if (oper.token == CParser::inline_if.token)
      {
        result.operation = &CParser::inline_if;
      }
      else
      {
        found = false;
      }
    }
    if (!found)
    {
      throw std::invalid_argument{"unknown operator"};
    }
    result.result_type = as_var_data(obj.at("res_t").as_string()).type;
    const boost::json::array& operands = obj.at("operands").as_array();
    result.operands.reserve(operands.size());
    for (const boost::json::value& operand: operands)
    {
      result.operands.push_back(as_expr(operand));
    }
    return result;
  }
  return {nullptr, {as_operand(val)}};
}
rychkov::entities::Expression::operand rychkov::Loader::as_operand(const boost::json::value& val)
{
  if (val.is_array())
  {
    return as_body(val);
  }
  const boost::json::object& obj = val.as_object();
  const boost::json::string& type = obj.at("obj").as_string();
  if (type == "expr")
  {
    return as_expr(val);
  }
  else if (type == "var")
  {
    return as_var(val);
  }
  else if (type == "lit")
  {
    return as_lit(val);
  }
  else if (type == "decl")
  {
    return as_decl(val);
  }
  else if (type == "cast")
  {
    return as_cast(val);
  }
  throw std::invalid_argument{"unknown expression operand"};
}
rychkov::entities::Variable rychkov::Loader::as_var_data(const boost::json::string& str)
{
  std::stringstream temp{std::string{str.begin(), str.end()}};
  std::stringstream err;
  CParseContext context{temp, err, "json-load"};
  preproc.parse(context, temp);
  const TypeParser& type_parser = preproc.next->next->next();
  if (!type_parser.ready())
  {
    throw std::invalid_argument{"incorrect variable"};
  }
  if (context.nerrors > 0)
  {
    throw std::runtime_error{err.str()};
  }
  preproc.next->next->prepare_type();
  entities::Variable result = type_parser.variable();
  preproc.next->next->clear_program();
  return result;
}
rychkov::entities::Variable rychkov::Loader::as_var(const boost::json::value& val)
{
  return as_var_data(val.at("data").as_string());
}
rychkov::entities::Function rychkov::Loader::as_func(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  entities::Variable temp = as_var(obj.at("sign"));
  entities::Function result{std::move(temp.type), std::move(temp.name)};
  const boost::json::array& params = obj.at("parameters").as_array();
  result.parameters.reserve(params.size());
  std::transform(params.begin(), params.end(), std::back_inserter(result.parameters),
        static_cast< std::string(*)(const boost::json::value&) >(boost::json::value_to< std::string >));
  return result;
}
rychkov::entities::Struct rychkov::Loader::as_struct(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  entities::Struct result{boost::json::value_to< std::string >(obj.at("name"))};
  preproc.next->next->base_types.insert({{result.name, typing::STRUCT}, depth});
  const boost::json::array& fields = obj.at("fields").as_array();
  ++depth;
  for (const boost::json::value& field: fields)
  {
    result.fields.insert(as_var(field));
  }
  --depth;
  clear_scope();
  preproc.next->next->structs.emplace(result, depth);
  return result;
}
rychkov::entities::Enum rychkov::Loader::as_enum(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  entities::Enum result{boost::json::value_to< std::string >(obj.at("name"))};
  preproc.next->next->base_types.insert({{result.name, typing::ENUM}, depth});
  const boost::json::array& fields = obj.at("fields").as_array();
  for (const boost::json::value& field: fields)
  {
    result.fields.emplace(boost::json::value_to< std::string >(field.as_array()[0]),
          field.as_array()[1].as_int64());
  }
  preproc.next->next->enums.emplace(result, depth);
  return result;
}
rychkov::entities::Union rychkov::Loader::as_union(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  entities::Union result{boost::json::value_to< std::string >(obj.at("name"))};
  preproc.next->next->base_types.insert({{result.name, typing::STRUCT}, depth});
  const boost::json::array& fields = obj.at("fields").as_array();
  ++depth;
  for (const boost::json::value& field: fields)
  {
    result.fields.insert(as_var(field));
  }
  --depth;
  clear_scope();
  preproc.next->next->unions.emplace(result, depth);
  return result;
}
rychkov::entities::Alias rychkov::Loader::as_alias(const boost::json::value& val)
{
  entities::Variable result = as_var(val.at("sign"));
  return {std::move(result.type), std::move(result.name)};
}
rychkov::entities::Statement rychkov::Loader::as_statement(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  entities::Statement result{static_cast< entities::Statement::Type >(obj.at("type").as_int64())};
  if (result.type >= entities::Statement::TYPE_LAST)
  {
    throw std::invalid_argument{"wrong statement type"};
  }
  const boost::json::array& conditions = obj.at("conditions").as_array();
  result.conditions.reserve(conditions.size());
  ++depth;
  for (const boost::json::value& expr: conditions)
  {
    result.conditions.push_back(as_expr(expr));
  }
  --depth;
  clear_scope();
  return result;
}
rychkov::entities::Literal rychkov::Loader::as_lit(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  entities::Literal::Type type = static_cast< entities::Literal::Type >(obj.at("type").as_int64());
  if (type >= 3)
  {
    throw std::invalid_argument{"wrong literal type"};
  }
  return {boost::json::value_to< std::string >(obj.at("body")),
        boost::json::value_to< std::string >(obj.at("suf")), type,
        as_var_data(obj.at("res_type").as_string()).type};
}
rychkov::entities::Declaration rychkov::Loader::as_decl(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  entities::ScopeType scope = static_cast< entities::ScopeType >(obj.at("scope").as_int64());
  if (scope >= 3)
  {
    throw std::invalid_argument{"wrong scope type"};
  }
  rychkov::entities::Declaration result{as_declared(obj.at("data")), nullptr, scope};
  if (obj.at("value").is_null())
  {
    return result;
  }
  ++depth;
  result.value = as_expr(obj.at("value"));
  --depth;
  clear_scope();
  return result;
}
rychkov::entities::Declaration::declared rychkov::Loader::as_declared(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  const boost::json::string& type = obj.at("obj").as_string();
  if (type == "var")
  {
    entities::Variable result = as_var(val);
    parser.variables.emplace(result, depth);
    return result;
  }
  else if (type == "func")
  {
    entities::Function result = as_func(val);
    parser.variables.insert({{result.type, result.name}, depth});
    return result;
  }
  else if (type == "struct")
  {
    return as_struct(val);
  }
  else if (type == "enum")
  {
    return as_enum(val);
  }
  else if (type == "union")
  {
    return as_union(val);
  }
  else if (type == "alias")
  {
    entities::Alias result = as_alias(val);
    parser.aliases.emplace(result);
    return result;
  }
  else if (type == "stmtnt")
  {
    return as_statement(val);
  }
  throw std::invalid_argument{"unknown declared"};
}
rychkov::entities::CastOperation rychkov::Loader::as_cast(const boost::json::value& val)
{
  const boost::json::object& obj = val.as_object();
  return {as_var_data(obj.at("to").as_string()).type, obj.at("explicit").as_bool(), as_expr(obj.at("value"))};
}
rychkov::entities::Body rychkov::Loader::as_body(const boost::json::value& val)
{
  const boost::json::array& data = val.as_array();
  entities::Body result;
  result.data.reserve(data.size());
  ++depth;
  for (const boost::json::value& expr: data)
  {
    result.data.push_back(as_expr(expr));
  }
  --depth;
  clear_scope();
  return result;
}
