#include "dictionary.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iterator>

namespace {

struct PrefixChecker {
  const std::string &prefix;
  PrefixChecker(const std::string &p) : prefix(p) {}

  bool operator()(const std::pair<std::string, std::string> &entry) const {
    if (entry.first.size() < prefix.size()) return false;
    return std::equal(prefix.begin(), prefix.end(), entry.first.begin());
  }

  bool operator()(const std::string &word) const {
    if (word.size() < prefix.size()) return false;
    return std::equal(prefix.begin(), prefix.end(), word.begin());
  }
};

struct GetFirst {
  const std::string &operator()(const std::pair<std::string, std::string> &p) const {
    return p.first;
  }
};

struct TranslationFragmentChecker {
  const std::string &fragment;
  TranslationFragmentChecker(const std::string &f) : fragment(f) {}

  bool operator()(const std::pair<std::string, std::string> &entry) const {
    return entry.second.find(fragment) != std::string::npos;
  }

  bool operator()(const std::string &word) const {
    return false;
  }
};

struct KeyInRangeChecker {
  const std::string &start;
  const std::string &end;
  KeyInRangeChecker(const std::string &s,
                    const std::string &e): start(s), end(e) {}
  bool operator()(const std::pair<std::string, std::string> &p) const
  {
    return p.first >= start && p.first <= end;
  }
};

struct NotInOtherChecker {
  const Dictionary &other;
  explicit NotInOtherChecker(const Dictionary &o): other(o) {}
  bool operator()(const std::pair<std::string, std::string> &p) const
  {
    std::string dummy;
    return !other.findWord(p.first, dummy);
  }
};

struct KeyExistsInOther {
  const Dictionary &other;
  explicit KeyExistsInOther(const Dictionary &o): other(o) {}
  bool operator()(const std::pair<std::string, std::string> &p) const
  {
    std::string tmp;
    return other.findWord(p.first, tmp);
  }
};

struct KeyExtractor {
  std::string operator()(const std::pair<std::string, std::string> &p) const
  {
    return p.first;
  }
};

struct SwapPair {
  std::pair<std::string, std::string> operator()(const std::pair<std::string, std::string> &p) const
  {
    return std::make_pair(p.second, p.first);
  }
};

struct IdentityPair {
  const std::pair<std::string, std::string> &operator()(const std::pair<std::string, std::string> &p) const
  {
    return p;
  }
};

}

void Dictionary::addWord(const std::string &word,
                         const std::string &translation)
{
  data_.insert(std::make_pair(word, translation));
}

void Dictionary::removeWord(const std::string &word)
{
  data_.erase(word);
}

void Dictionary::editWord(const std::string &word,
                          const std::string &newTranslation)
{
  std::unordered_map<std::string, std::string>::iterator it = data_.find(word);
  if (it != data_.end()) {
    it->second = newTranslation;
  }
}

bool Dictionary::findWord(const std::string &word,
                          std::string &translation) const
{
  std::unordered_map<std::string, std::string>::const_iterator it = data_.find(word);
  if (it != data_.end()) {
    translation = it->second;
    return true;
  }
  return false;
}

std::vector<std::string> Dictionary::listWordsWithPrefix(const std::string &prefix) const
{
  std::vector<std::string> result;
  PrefixChecker checker(prefix);

  std::vector<std::pair<std::string, std::string>> allWords = getAllWords();

  result.reserve(allWords.size());
  std::transform(allWords.begin(), allWords.end(), std::back_inserter(result), GetFirst());

  struct NotPrefixChecker {
    const PrefixChecker &checker;
    NotPrefixChecker(const PrefixChecker &c) : checker(c) {}
    bool operator()(const std::string &word) const {
      return !checker(word);
    }
  };

  result.erase(std::remove_if(result.begin(), result.end(), NotPrefixChecker(checker)), result.end());

  return result;
}

std::vector<std::pair<std::string, std::string>> Dictionary::getAllWords() const
{
  return std::vector<std::pair<std::string, std::string>>(data_.begin(), data_.end());
}

std::size_t Dictionary::getWordCount() const
{
  return data_.size();
}

void Dictionary::clear()
{
  data_.clear();
}

const std::unordered_map<std::string, std::string> &Dictionary::getData() const
{
  return data_;
}

void Dictionary::mergeFrom(const Dictionary &other)
{
  std::vector<std::pair<std::string, std::string>> buffer(other.data_.begin(), other.data_.end());
  std::transform(buffer.begin(), buffer.end(), buffer.begin(), IdentityPair());
  std::copy(buffer.begin(), buffer.end(),
            std::inserter(data_, data_.end()));
}

Dictionary Dictionary::intersectWith(const Dictionary &other) const
{
  Dictionary result;
  KeyExistsInOther checker(other);
  std::vector<std::pair<std::string, std::string>> temp;
  std::copy_if(data_.begin(), data_.end(), std::back_inserter(temp), checker);
  std::copy(temp.begin(), temp.end(),
            std::inserter(result.data_, result.data_.end()));
  return result;
}

Dictionary Dictionary::diffFrom(const Dictionary &other) const
{
  Dictionary result;
  NotInOtherChecker checker(other);
  std::vector<std::pair<std::string, std::string>> temp;
  std::copy_if(data_.begin(), data_.end(), std::back_inserter(temp), checker);
  std::copy(temp.begin(), temp.end(),
            std::inserter(result.data_, result.data_.end()));
  return result;
}

Dictionary Dictionary::extractRange(const std::string &start, const std::string &end) const
{
  Dictionary result;
  KeyInRangeChecker checker(start, end);
  std::vector<std::pair<std::string, std::string>> temp;
  std::copy_if(data_.begin(), data_.end(), std::back_inserter(temp), checker);
  std::copy(temp.begin(), temp.end(),
            std::inserter(result.data_, result.data_.end()));
  return result;
}

std::vector<std::string> Dictionary::getWordsByTranslationFragment(const std::string &fragment) const
{
  std::vector<std::pair<std::string, std::string>> allWords = getAllWords();
  TranslationFragmentChecker checker(fragment);

  struct CheckAndKeepFirst {
    const TranslationFragmentChecker &checker;
    explicit CheckAndKeepFirst(const TranslationFragmentChecker &chk) : checker(chk) {}

    bool operator()(const std::pair<std::string, std::string> &entry) const {
      return checker(entry);
    }
  };

  struct ExtractFirst {
    std::string operator()(const std::pair<std::string, std::string> &entry) const {
      return entry.first;
    }
  };

  std::vector<std::pair<std::string, std::string>> filtered;
  std::copy_if(allWords.begin(), allWords.end(), std::back_inserter(filtered), CheckAndKeepFirst(checker));

  std::vector<std::string> result;
  std::transform(filtered.begin(), filtered.end(), std::back_inserter(result), ExtractFirst());

  return result;
}


std::pair<std::string, std::string> Dictionary::getRandomPair() const
{
  std::vector<std::pair<std::string, std::string>> all = getAllWords();
  if (all.empty()) {
    return std::pair<std::string, std::string>("", "");
  }
  std::size_t idx = std::rand() % all.size();
  return all[idx];
}

void Dictionary::swapTranslations()
{
  std::vector<std::pair<std::string, std::string>> temp;
  std::transform(data_.begin(), data_.end(),
                 std::back_inserter(temp), SwapPair());
  data_.clear();
  std::copy(temp.begin(), temp.end(),
            std::inserter(data_, data_.end()));
}
