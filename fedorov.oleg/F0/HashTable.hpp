#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <string>
#include <list>
#include <utility>

namespace crossref
{

  class HashTable
  {
    struct RehashHelper;
    struct EmplaceHelper;
    struct ClearHelper;

  public:
    explicit HashTable(size_t size = 101);
    void insert(const std::string &key, int line);
    void remove(const std::string &key);
    std::vector< int > find(const std::string &key) const;
    std::vector< std::pair< std::string, std::vector< int > > > getSortedEntries() const;
    void clear();
    bool isEmpty() const;
    size_t size() const;
    size_t capacity() const;

  private:
    struct EntryComparator;
    struct HashEntry
    {
      std::string word;
      std::list< int > lines;
      bool isActive;
    };

    std::vector< HashEntry > table;
    size_t itemCount;

    size_t findPosition(const std::string &key) const;
    size_t findPosition(const std::string &key, size_t current, size_t i) const;
    size_t nextPrime(size_t n) const;

    void rehash();
    size_t hash(const std::string &key) const;

    friend struct RehashHelper;
    friend struct EmplaceHelper;
    friend struct ClearHelper;
  };

  struct HashTable::RehashHelper
  {
    HashTable *table;
    RehashHelper(HashTable *t);
    void operator()(const HashEntry &entry) const;
  };

  struct HashTable::EmplaceHelper
  {
    std::vector< std::pair< std::string, std::vector< int > > > &entries;
    EmplaceHelper(decltype(entries) &e);
    void operator()(const HashEntry &entry) const;
  };

  struct HashTable::ClearHelper
  {
    void operator()(HashEntry &entry) const;
  };

}
#endif
