#include "HashTable.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <list>
#include <numeric>

namespace
{

  bool isPrimeRecursive(size_t n, size_t i)
  {
    if (i * i > n)
    {
      return false;
    }
    if (n % i == 0 || n % (i + 2) == 0)
    {
      return true;
    }
    return isPrimeRecursive(n, i + 6);
  }

  bool isPrime(size_t n)
  {
    if (n <= 1)
    {
      return false;
    }
    if (n <= 3)
    {
      return true;
    }
    if (n % 2 == 0 || n % 3 == 0)
    {
      return false;
    }
    return !isPrimeRecursive(n, 5);
  }

}

namespace crossref
{

  HashTable::RehashHelper::RehashHelper(HashTable *t):
    table(t)
  {}
  HashTable::EmplaceHelper::EmplaceHelper(decltype(entries) &e):
    entries(e)
  {}

  void HashTable::RehashHelper::operator()(const HashEntry &entry) const
  {
    if (entry.isActive && !entry.word.empty())
    {
      struct InsertLine
      {
        HashTable *table;
        std::string word;
        InsertLine(HashTable *t, const std::string &w):
          table(t),
          word(w)
        {}
        void operator()(int line) const
        {
          table->insert(word, line);
        }
      };

      std::for_each(entry.lines.begin(), entry.lines.end(), InsertLine(table, entry.word));
    }
  }

  void HashTable::EmplaceHelper::operator()(const HashEntry &entry) const
  {
    if (entry.isActive && !entry.word.empty())
    {
      entries.emplace_back(entry.word, std::vector< int >(entry.lines.begin(), entry.lines.end()));
    }
  }

  void HashTable::ClearHelper::operator()(HashEntry &entry) const
  {
    entry.word.clear();
    entry.lines.clear();
    entry.isActive = false;
  }

  HashTable::HashTable(size_t size):
    table(size),
    itemCount(0)
  {}

  struct HashAccumulator
  {
    size_t operator()(size_t hashValue, unsigned char c) const
    {
      return (hashValue * 31) + c;
    }
  };

  size_t HashTable::hash(const std::string &key) const
  {
    size_t init = 0;
    return std::accumulate(key.begin(), key.end(), init, HashAccumulator()) % table.size();
  }

  size_t HashTable::findPosition(const std::string &key) const
  {
    return findPosition(key, hash(key), 0);
  }

  size_t HashTable::findPosition(const std::string &key, size_t current, size_t i) const
  {
    if (!table[current].isActive || table[current].word == key)
    {
      return current;
    }
    if (i >= table.size())
    {
      return current;
    }

    size_t next_i = i + 1;
    size_t next = (current + next_i * next_i) % table.size();
    return findPosition(key, next, next_i);
  }

  size_t HashTable::nextPrime(size_t n) const
  {
    if (n % 2 == 0)
    {
      n++;
    }
    if (isPrime(n))
    {
      return n;
    }
    return nextPrime(n + 2);
  }

  void HashTable::rehash()
  {
    std::vector< HashEntry > oldTable = std::move(table);
    table = std::vector< HashEntry >(nextPrime(oldTable.size() * 2));
    itemCount = 0;

    std::for_each(oldTable.begin(), oldTable.end(), RehashHelper(this));
  }

  void HashTable::insert(const std::string &key, int line)
  {
    if (itemCount >= table.size() * 0.75)
    {
      rehash();
    }

    size_t pos = findPosition(key);

    if (!table[pos].isActive)
    {
      table[pos].word = key;
      table[pos].isActive = true;
      itemCount++;
    }

    if (std::find(table[pos].lines.begin(), table[pos].lines.end(), line) == table[pos].lines.end())
    {
      table[pos].lines.push_back(line);
      table[pos].lines.sort();
    }
  }

  void HashTable::remove(const std::string &key)
  {
    size_t pos = findPosition(key);

    if (table[pos].isActive && table[pos].word == key)
    {
      table[pos].isActive = false;
      table[pos].lines.clear();
      itemCount--;
    }
  }

  std::vector< int > HashTable::find(const std::string &key) const
  {
    size_t pos = findPosition(key);

    if (table[pos].isActive && table[pos].word == key)
    {
      return std::vector< int >(table[pos].lines.begin(), table[pos].lines.end());
    }
    return {};
  }

  struct HashTable::EntryComparator
  {
    using type_entry = std::pair< std::string, std::vector< int > >;
    bool operator()(const type_entry &a, const type_entry &b) const
    {
      return a.first < b.first;
    }
  };

  std::vector< std::pair< std::string, std::vector< int > > > HashTable::getSortedEntries() const
  {
    std::vector< std::pair< std::string, std::vector< int > > > entries;

    std::for_each(table.begin(), table.end(), EmplaceHelper(entries));

    std::sort(entries.begin(), entries.end(), EntryComparator());
    return entries;
  }

  void HashTable::clear()
  {
    std::for_each(table.begin(), table.end(), ClearHelper());
    itemCount = 0;
  }

  bool HashTable::isEmpty() const
  {
    return itemCount == 0;
  }

  size_t HashTable::size() const
  {
    return itemCount;
  }

  size_t HashTable::capacity() const
  {
    return table.size();
  }

}
