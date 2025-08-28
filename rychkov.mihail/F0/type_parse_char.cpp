#include "type_parser.hpp"

#include <utility>
#include <map>

void rychkov::TypeParser::append(CParseContext& context, char c)
{
  static const std::map< char, void(TypeParser::*)(CParseContext&) > dispatch_map = {
        {'*', &TypeParser::append_asterisk},
        {'[', &TypeParser::append_open_bracket},
        {']', &TypeParser::append_close_bracket},
        {'(', &TypeParser::append_open_parenthesis},
        {')', &TypeParser::append_close_parenthesis},
        {',', &TypeParser::append_comma}
      };

  decltype(dispatch_map)::const_iterator found = dispatch_map.find(c);
  if (found != dispatch_map.cend())
  {
    (this->*(found->second))(context);
    return;
  }
  log(context, "unknown symbol in type declaration");
}
void rychkov::TypeParser::append_asterisk(CParseContext& context)
{
  if (stack_.empty() || stack_.top().right_allign)
  {
    log(context, "pointer cannot be applied here");
    return;
  }
  if (!stack_.top().data->empty())
  {
    move_up();
  }
  stack_.top().data->category = typing::POINTER;
}
void rychkov::TypeParser::append_open_bracket(CParseContext& context)
{
  if (stack_.top().data->empty() || stack_.top().bracket_opened
      || (stack_.top().data->category == typing::FUNCTION))
  {
    log(context, "array cannot be applied here");
    return;
  }
  typing::Type* old = move_up();
  stack_.top().data->category = typing::ARRAY;
  if (old->category == typing::ARRAY)
  {
    stack_.top().data->array_has_length = old->array_has_length;
    stack_.top().data->array_length = old->array_length;
    old->array_has_length = false;
    old->array_length = 0;
    stack_.top().data = old;
    stack_.top().array_must_have_size = true;
  }
  stack_.top().bracket_opened = true;
  stack_.top().right_allign = true;
}
void rychkov::TypeParser::append_close_bracket(CParseContext& context)
{
  if (stack_.empty() || !stack_.top().bracket_opened)
  {
    log(context, "found unpaired ']'");
    return;
  }
  if (stack_.top().array_must_have_size && !stack_.top().data->array_has_length)
  {
    log(context, "array cannot be abstract here");
    return;
  }
  stack_.top().bracket_opened = false;
}
void rychkov::TypeParser::append_open_parenthesis(CParseContext& context)
{
  if (stack_.top().data->empty())
  {
    log(context, "type cannot starts with '('");
    return;
  }
  if (stack_.top().right_allign)
  {
    if ((stack_.top().data->category == typing::ARRAY)
        || (stack_.top().data->category == typing::FUNCTION))
    {
      log(context, "function parentheses cannot be applied here");
      return;
    }
    if (!stack_.top().data->empty())
    {
      move_up();
    }
    stack_.top().data->category = typing::FUNCTION;
    stack_.top().data->function_parameters.emplace_back();
    stack_.push({&stack_.top().data->function_parameters.back()});
    stack_.top().is_function_paremeter = true;
    if (can_be_named_param(stack_.top().data))
    {
      parameters_.emplace_back();
    }
    return;
  }
  typing::Type* old = move_up();
  std::swap(old, stack_.top().data);
  stack_.push({old});
  stack_.top().parenthesis_opened = true;
}
void rychkov::TypeParser::append_close_parenthesis(CParseContext& context)
{
  if (stack_.size() <= 1)
  {
    log(context, "found unpaired ')'");
    return;
  }
  if (stack_.top().is_function_paremeter)
  {
    if (stack_.top().data->empty() && (stack_.top().data->base == nullptr))
    {
      stack_.pop();
      if (stack_.top().data->function_parameters.size() == 1)
      {
        if (can_be_named_param(&stack_.top().data->function_parameters[0]))
        {
          parameters_.clear();
        }
        stack_.top().data->function_parameters.clear();
        return;
      }
      log(context, "function parameter following after ',' cannot be empty");
      return;
    }
    else if (!stack_.top().data->ready())
    {
      log(context, "function parameter started but not finished");
      return;
    }
    stack_.pop();
    return;
  }
  if (stack_.top().bracket_opened)
  {
    log(context, "')' cannot follow '['");
    return;
  }
  if (stack_.top().data->empty())
  {
    decltype(typing::Type::base) temp = std::move(stack_.top().data->base);
    *stack_.top().data = std::move(*temp);

    typing::Type* old = stack_.top().data;
    stack_.pop();
    stack_.top().data = old;
    stack_.top().right_allign = true;
    return;
  }
  stack_.pop();
  stack_.top().right_allign = true;
}
void rychkov::TypeParser::append_comma(CParseContext& context)
{
  if ((stack_.size() <= 1) || !stack_.top().is_function_paremeter || !stack_.top().data->ready())
  {
    log(context, "',' cannot be applied here");
    return;
  }
  remove_combination(*stack_.top().data);
  stack_.pop();
  stack_.top().data->function_parameters.emplace_back();
  stack_.push({&stack_.top().data->function_parameters.back()});
  stack_.top().is_function_paremeter = true;
  if (can_be_named_param(stack_.top().data))
  {
    parameters_.emplace_back();
  }
}
