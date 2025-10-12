#ifndef DATA_STORAGE_HPP
#define DATA_STORAGE_HPP
#include <string>
#include <unordered_map>
#include <memory>

namespace nikonov {
  using str = std::string;
  class Text {
  public:
    Text(const str& content, bool isCompressed = false, const str& encodingId = "", const str& compressedData = "");
    const str& getOriginalContent() const;
    const str& getCompressedContent() const;
    const str& getEncodingId() const;
    bool isCompressed() const;
    size_t getOriginalSizeBits() const;
    size_t getCompressedSizeBits() const;
  private:
    str originalContent_;
    str compressedContent_;
    str encodingId_;
    bool isCompressed_;
  };

  class Encoding {
  public:
    Encoding(const std::unordered_map< char, str >& codeTable, const str& fromTextId = "");
    const std::unordered_map< char, str >& getCodeTable() const;
    const std::unordered_map< str, char >& getReverseTable() const;
    const str& getFromTextId() const;
  private:
    std::unordered_map< char, str > codeTable_;
    std::unordered_map< str, char > reverseTable_;
    str fromTextId_;
    void buildReverseTable();
  };

  class Storage {
  public:
    bool addText(const str& id, const str& content);
    bool addCompressedText(const str& id, const str& original, const str& compressed, const str& encodingId);
    Text* getText(const str& id);
    bool textExists(const str& id) const;
    bool addEncoding(const str& id, const std::unordered_map< char, str >& codes, const str& textId = "");
    Encoding* getEncoding(const str& id);
    bool encodingExists(const str& id) const;
  private:
    std::unordered_map< str, std::unique_ptr< Text > > texts_;
    std::unordered_map< str, std::unique_ptr< Encoding > > encodings_;
  };
}
#endif
