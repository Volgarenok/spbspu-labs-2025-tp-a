#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <istream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
namespace kiselev
{
  using Dict = std::map< std::string, std::vector< std::string > >;
  using Dicts = std::map< std::string, Dict >;
  void doNewDict(std::istream&, std::ostream&, Dicts&);
  void doDeleteDict(std::istream&, std::ostream&, Dicts&);
  void doAddElement(std::istream&, std::ostream&, Dicts&);
  void doDeleteElement(std::istream&, std::ostream&, Dicts&);
  void doListDict(std::ostream&, const Dicts&);
  void doPrintDict(std::istream&, std::ostream&, const Dicts&);
  void doTranslateWord(std::istream&, std::ostream&, const Dicts&);
  void doUnionDict(std::istream&, std::ostream&, Dicts&);
  void doSaveDict(std::istream&, std::ostream&, const Dicts&);
  void doCountWord(std::istream&, std::ostream&, const Dicts&);
  void doSearchLetter(std::istream&, std::ostream&, const Dicts&);
  void doLoadDict(std::istream&, std::ostream&, Dicts&);
  void doClearDict(std::istream&, std::ostream&, Dicts&);
  void doIntersectDict(std::istream&, std::ostream&, Dicts&);
  void doComplementDict(std::istream&, std::ostream&, Dicts&);
}
#endif
