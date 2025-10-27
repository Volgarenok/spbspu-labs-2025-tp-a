#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace zholobov {

  namespace {

    bool skipWsUntilNewLine(std::istream& input)
    {
      char c;
      while (input.get(c)) {
        if (c == '\n') {
          return true;
        }
        if (!std::isspace(c)) {
          input.putback(c);
          return false;
        }
      }
      return false;
    }

    struct DictionaryElementPrinter {
      DictionaryElementPrinter(std::ostream& out):
        out(out)
      {}
      void operator()(const Dictionary::value_type& elem)
      {
        out << elem.first;
        if (!elem.second.empty()) {
          out << " " << elem.second;
        }
        out << "\n";
      }
      std::ostream& out;
    };

  }

  std::istream& operator>>(std::istream& in, Words& words)
  {
    words.clear();
    std::istream::sentry s(in, true);
    if (!s) {
      return in;
    }
    if (skipWsUntilNewLine(in)) {
      return in;
    }
    Word word;
    while (in >> word) {
      words.push_back(word);
      if (skipWsUntilNewLine(in)) {
        return in;
      }
    }
    return in;
  }

  std::istream& operator>>(std::istream& in, Dictionary& dict)
  {
    dict.clear();
    std::istream::sentry s(in, true);
    if (!s) {
      return in;
    }

    Word word;
    Words translations;
    while (in >> word >> translations) {
      dict.emplace(word, translations);
    }

    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Words& words)
  {
    std::ostream::sentry s(out);
    if (!s) {
      return out;
    }
    if (!words.empty()) {
      std::copy(words.begin(), std::prev(words.end()),
          std::ostream_iterator< Word >(out, " "));
      out << *words.rbegin();
    }
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const Dictionary& dict)
  {
    std::ostream::sentry s(out);
    if (!s) {
      return out;
    }

    DictionaryElementPrinter printer(out);
    if (!dict.empty()) {
      std::for_each(dict.begin(), dict.end(), printer);
    }

    return out;
  }
}
