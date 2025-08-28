#include "commands.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>

void abramov::getCommands(std::map< std::string, std::function< void() > > &commands, DictionaryCollection &collect)
{
  commands["adddict"] = std::bind(addDict, std::ref(collect), std::ref(std::cin));
  commands["addtodict"] = std::bind(addToDict, std::ref(collect), std::ref(std::cin));
  commands["rmdict"] = std::bind(deleteDict, std::ref(collect), std::ref(std::cin));
  commands["rmfromdict"] = std::bind(deleteFromDict, std::ref(collect), std::ref(std::cin));
  commands["set"] = std::bind(setDicts, std::ref(collect), std::ref(std::cin));
  commands["intersection"] = std::bind(intersectDicts, std::ref(collect), std::ref(std::cin));
  commands["union"] = std::bind(unionDicts, std::ref(collect), std::ref(std::cin));
  commands["difference"] = std::bind(diffDicts, std::ref(collect), std::ref(std::cin));
  commands["merge"] = std::bind(mergeDicts, std::ref(collect), std::ref(std::cin));
  commands["print"] = std::bind(printDict, std::cref(collect), std::ref(std::cin), std::ref(std::cout));
}

void abramov::addDict(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There are no enough arguments\n");
  }
  collect.addDict(name);
}

void abramov::addToDict(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  std::string word;
  std::string trans;
  in >> name >> word >> trans;
  if (name.empty() || word.empty() || trans.empty())
  {
    throw std::logic_error("There are no enough arguments\n");
  }
  Dictionary &dict = collect.findDict(name);
  dict.addWord(word, trans);
}

void abramov::deleteDict(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There are no enough arguments\n");
  }
  collect.deleteDict(name);
}

void abramov::deleteFromDict(DictionaryCollection &collect, std::istream &in)
{
  std::string name;
  std::string word;
  in >> name >> word;
  if (name.empty() || word.empty())
  {
    throw std::logic_error("There is no enough arguments\n");
  }
  Dictionary &dict = collect.findDict(name);
  dict.deleteWord(word);
}

void abramov::setDicts(DictionaryCollection &collect, std::istream &in)
{
  struct SetOperation
  {
    Dictionary operator()(Dictionary &res, const Dictionary &other) const
    {
      return res.setWithDict(other);
    }
  };

  processDicts(collect, in, SetOperation());
}

void abramov::intersectDicts(DictionaryCollection &collect, std::istream &in)
{
  struct IntersectOperation
  {
    Dictionary operator()(Dictionary &res, const Dictionary &other) const
    {
      return res.intersectWithDict(other);
    }
  };

  processDicts(collect, in, IntersectOperation());
}

void abramov::unionDicts(DictionaryCollection &collect, std::istream &in)
{
  struct UnionOperation
  {
    Dictionary operator()(Dictionary &res, const Dictionary &other) const
    {
      return res.unionWithDict(other);
    }
  };

  processDicts(collect, in, UnionOperation());
}

void abramov::diffDicts(DictionaryCollection &collect, std::istream &in)
{
  struct DiffOperation
  {
    Dictionary operator()(Dictionary &res, const Dictionary &other) const
    {
      return res.diffDict(other);
    }
  };

  processDicts(collect, in, DiffOperation());
}

void abramov::mergeDicts(DictionaryCollection &collect, std::istream &in)
{
  struct MergeOperation
  {
    Dictionary operator()(Dictionary &res, const Dictionary &other) const
    {
      return res.mergeDict(other);
    }
  };

  processDicts(collect, in, MergeOperation(), true);
}

void abramov::printDict(const DictionaryCollection &collect, std::istream &in, std::ostream &out)
{
  std::string name;
  in >> name;
  if (name.empty())
  {
    throw std::logic_error("There are no enough arguments\n");
  }
  const Dictionary &dict = collect.cfindDict(name);
  if (dict.empty())
  {
    throw std::logic_error("Dictionary is empty\n");
  }
  dict.print(out);
}

template< class Func >
void abramov::processDicts(DictionaryCollection &collect, std::istream &in, Func func, bool del)
{
  std::string new_name;
  in >> new_name;
  char bracket = '\0';
  in >> bracket;
  if (bracket != '(')
  {
    throw std::logic_error("Expected '('\n");
  }
  Dictionary res{};
  std::string name;
  std::vector< std::string > to_del;
  bool first = true;
  while (in >> name && name != ")")
  {
    if (first)
    {
      res = collect.findDict(name);
      first = false;
    }
    else
    {
      res = func(res, collect.cfindDict(name));
    }
    if (del)
    {
      to_del.push_back(name);
    }
  }
  if (name != ")")
  {
    throw std::logic_error("Expected ')'\n");
  }
  if (res.empty())
  {
    throw std::logic_error("There are no words for dictionary\n");
  }
  collect.addCompleteDict(new_name, res);
  if (del)
  {
    for (const auto &dict_name : to_del)
    {
      collect.deleteDict(dict_name);
    }
  }
}
