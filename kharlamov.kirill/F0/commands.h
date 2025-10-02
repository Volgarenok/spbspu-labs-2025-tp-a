#ifndef COMMANDS_H
#define COMMANDS_H

#include <algorithm>
#include <istream>
#include <map>
#include <ostream>
#include <set>
#include <string>


namespace kharlamov
{
  using dictionary = std::map< std::string, std::string >;
  using dictionaries = std::map< std::string, dictionary >;

  void doDownload(std::istream& in, std::ostream& out, dictionaries& dicts);
  void doSave(std::istream& in, std::ostream& out, const dictionaries& dicts);
  void doAdd(std::istream& in, std::ostream& out, dictionaries& dicts);
  void doRemove(std::istream& in, std::ostream& out, dictionaries& dicts);
  void doTranslate(std::istream& in, std::ostream& out, const dictionaries& dicts);
  void doFind(std::istream& in, std::ostream& out, const dictionaries& dicts);
  void doSearch(std::istream& in, std::ostream& out, const dictionaries& dicts);
  void doCount(std::istream& in, std::ostream& out, const dictionaries& dicts);
  void doList(std::istream& in, std::ostream& out, const dictionaries& dicts);
  void doClear(std::istream& in, std::ostream& out, dictionaries& dicts);
  void doCreatedict(std::ostream& out, dictionaries& dicts);
  void doListdicts(std::ostream& out, const dictionaries& dicts);
  void doCopy(std::istream& in, std::ostream& out, dictionaries& dicts);
  void doMerge(std::istream& in, std::ostream& out, dictionaries& dicts);
  void doDiff(std::istream& in, std::ostream& out, dictionaries& dicts);
  void doIntersect(std::istream& in, std::ostream& out, dictionaries& dicts);
  void doGlobalsearch(std::istream& in, std::ostream& out, const dictionaries& dicts);
  void doGlobalcount(std::ostream& out, const dictionaries& dicts);
}

#endif
