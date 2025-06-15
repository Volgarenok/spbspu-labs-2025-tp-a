#include "code_parsers.hpp"

bool rychkov::TypeParser::append(CParseContext& context, std::string name)
{
  using append_signature = bool(TypeParser::*)(CParseContext&);
  using append_map = std::map< std::string, append_signature >;
  static const append_map dispatch_map = {
        {"const", &TypeParser::append_const},
        {"volatile", &TypeParser::append_volatile},
        {"signed", &TypeParser::append_signed},
        {"unsigned", &TypeParser::append_unsigned}
      };

  append_map::const_iterator found = dispatch_map.find(name);
  if (found != dispatch_map.cend())
  {
    return (this->*(found->second))(context);
  }

  if (stack_.top().data != &combined_)
  {
    if (can_be_named_param(stack_.top().data))
    {
      parameters_.back() = std::move(name);
      return true;
    }
    log(context, "name \"" + name + "\" cannot be placed here");
    return false;
  }
  if (!var_name_.empty())
  {
    log(context, "name repeated");
    return false;
  }
  var_name_ = std::move(name);
  stack_.top().right_allign = true;
  return true;
}
bool rychkov::TypeParser::append_const(CParseContext& context)
{
  if (stack_.empty())
  {
    combined_ = {{}, typing::Type::Combination};
    combined_.is_const = true;
    stack_.push({&combined_});
    return true;
  }
  if (stack_.top().right_allign)
  {
    log(context, "const cannot be applied here");
    return false;
  }
  if (stack_.top().data->is_const)
  {
    log(context, "const repeated");
    return false;
  }
  stack_.top().data->is_const = true;
  return true;
}
bool rychkov::TypeParser::append_volatile(CParseContext& context)
{
  if (stack_.empty())
  {
    combined_ = {{}, typing::Type::Combination};
    combined_.is_volatile = true;
    stack_.push({&combined_});
    return true;
  }
  if (stack_.top().right_allign)
  {
    log(context, "volatile cannot be applied here");
    return false;
  }
  if (stack_.top().data->is_volatile)
  {
    log(context, "volatile repeated");
    return false;
  }
  stack_.top().data->is_volatile = true;
  return true;
}
bool rychkov::TypeParser::append_signed(CParseContext&)
{
  return true;
}
bool rychkov::TypeParser::append_unsigned(CParseContext&)
{
  return true;
}
