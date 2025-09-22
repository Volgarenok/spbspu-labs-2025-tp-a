#include "DataStorage.hpp"
#include <stdexcept>

nikonov::Text::Text(const str& content, bool isCompressed, const str& encodingId, const str& compressedData):
  originalContent_(content),
  compressedContent_(compressedData),
  encodingId_(encodingId),
  isCompressed_(isCompressed)
{}

const std::string& nikonov::Text::getOriginalContent() const
{
  return originalContent_;
}

const std::string& nikonov::Text::getCompressedContent() const
{
  return compressedContent_;
}

const std::string& nikonov::Text::getEncodingId() const
{
  return encodingId_;
}

bool nikonov::Text::isCompressed() const
{
  return isCompressed_;
}

size_t nikonov::Text::getOriginalSizeBits() const
{
  constexpr int sizeOfByte = 8;
  return originalContent_.size() * sizeOfByte;
}

size_t nikonov::Text::getCompressedSizeBits() const
{
  return compressedContent_.size();
}

nikonov::Encoding::Encoding(const std::unordered_map< char, str >& codeTable, const str& fromTextId):
  codeTable_(codeTable),
  fromTextId_(fromTextId)
{
  buildReverseTable();
}

void nikonov::Encoding::buildReverseTable()
{
  for (const auto& pair : codeTable_) {
    reverseTable_[pair.second] = pair.first;
  }
}

const std::unordered_map< char, std::string >& nikonov::Encoding::getCodeTable() const
{
  return codeTable_;
}

const std::unordered_map< std::string, char >& nikonov::Encoding::getReverseTable() const
{
  return reverseTable_;
}

const std::string& nikonov::Encoding::getFromTextId() const
{
  return fromTextId_;
}

bool nikonov::Storage::addText(const str& id, const str& content)
{
  if (texts_.find(id) != texts_.end()) {
    return false;
  }
  texts_[id] = std::make_unique< Text >(content);
  return true;
}

bool nikonov::Storage::addCompressedText(const str& id, const str& original, const str& compressed, const str& encodingId)
{
  if (texts_.find(id) != texts_.end()) {
    return false;
  }
  texts_[id] = std::make_unique< Text >(original, true, encodingId, compressed);
  return true;
}

nikonov::Text* nikonov::Storage::getText(const str& id)
{
  auto it = texts_.find(id);
  if (it == texts_.end()) {
    return nullptr;
  }
  return it->second.get();
}

bool nikonov::Storage::textExists(const str& id) const
{
  return texts_.find(id) != texts_.end();
}

bool nikonov::Storage::addEncoding(const str& id, const std::unordered_map< char, str >& codes, const str& textId)
{
  if (encodings_.find(id) != encodings_.end()) {
    return false;
  }
  encodings_[id] = std::make_unique< Encoding >(codes, textId);
  return true;
}

nikonov::Encoding* nikonov::Storage::getEncoding(const str& id)
{
  auto it = encodings_.find(id);
  if (it == encodings_.end()) {
    return nullptr;
  }
  return it->second.get();
}

bool nikonov::Storage::encodingExists(const str& id) const
{
  return encodings_.find(id) != encodings_.end();
}
