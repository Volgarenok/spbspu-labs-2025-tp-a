#include "Commands.hpp"
#include "DataStorage.hpp"
#include "HuffmanCore.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <sstream>

void nikonov::getCommands(std::map< str, std::function< void(Storage&, std::istream&, std::ostream&) > >& commands)
{
  commands["compress_text"] = compressText;
  commands["decompress_text"] = decompressText;
  commands["create_encoding"] = createEncoding;
  commands["apply_encoding"] = applyEncoding;
  commands["compare_encodings"] = compareEncodings;
  commands["show_encoding"] = showEncoding;
  commands["text_stats"] = textStats;
  commands["test_compression"] = testCompression;
  commands["load_text_from_file"] = loadTextFromFile;
  commands["read_text_from_stdin"] = readTextFromStdin;
  commands["upload_text"] = uploadText;
  commands["get_encoding"] = getEncodingFromFile;
  commands["upload_encoding"] = uploadEncoding;
}
void nikonov::compressText(Storage& storage, std::istream& in, std::ostream& out)
{
  str textId, newTextId, newEncodingId;
  in >> textId >> newTextId >> newEncodingId;
  Text* text = storage.getText(textId);
  if (text == nullptr) {
    throw std::runtime_error("ERROR: Invalid text_id.");
  }
  if (text->isCompressed()) {
    throw std::runtime_error("ERROR: Text already compressed.");
  }
  auto codes = HuffmanCore::buildCodes(text->getOriginalContent());
  str compressed = HuffmanCore::compress(text->getOriginalContent(), codes);
  if (!storage.addEncoding(newEncodingId, codes, textId)) {
    throw std::runtime_error("ERROR: Memory overflow.");
  }
  if (!storage.addCompressedText(newTextId, text->getOriginalContent(), compressed, newEncodingId)) {
    throw std::runtime_error("ERROR: Memory overflow.");
  }
  out << "Text compressed successfully. Encoding ID: " << newEncodingId << std::endl;
}

void nikonov::decompressText(Storage& storage, std::istream& in, std::ostream& out)
{
  str compressedTextId, newTextId;
  in >> compressedTextId >> newTextId;
  Text* text = storage.getText(compressedTextId);
  if (text == nullptr) {
    throw std::runtime_error("ERROR: Invalid text_id.");
  }
  if (!text->isCompressed()) {
    throw std::runtime_error("ERROR: Text isn't compressed.");
  }
  Encoding* encoding = storage.getEncoding(text->getEncodingId());
  if (encoding == nullptr) {
    throw std::runtime_error("ERROR: Invalid encoding_id.");
  }
  str decompressed = HuffmanCore::decompress(
    text->getCompressedContent(), 
    encoding->getReverseTable()
  );
  if (!storage.addText(newTextId, decompressed)) {
    throw std::runtime_error("ERROR: Memory overflow.");
  }
  out << "Text decompressed successfully." << std::endl;
}

void nikonov::createEncoding(Storage& storage, std::istream& in, std::ostream& out)
{
  str textId, newEncodingId;
  in >> textId >> newEncodingId;
  Text* text = storage.getText(textId);
  if (text == nullptr) {
    throw std::runtime_error("ERROR: Invalid text_id.");
  }
  if (text->isCompressed()) {
    throw std::runtime_error("ERROR: Text already compressed.");
  }
  auto codes = HuffmanCore::buildCodes(text->getOriginalContent());
  if (!storage.addEncoding(newEncodingId, codes, textId)) {
    throw std::runtime_error("ERROR: Memory overflow.");
  }
  out << "Encoding created successfully. ID: " << newEncodingId << std::endl;
}

void nikonov::applyEncoding(Storage& storage, std::istream& in, std::ostream& out)
{
  str textId, encodingId, newTextId;
  in >> textId >> encodingId >> newTextId;
  Text* text = storage.getText(textId);
  if (text == nullptr) {
    throw std::runtime_error("ERROR: Invalid text_id.");
  }
  if (text->isCompressed()) {
    throw std::runtime_error("ERROR: Text already compressed.");
  }
  Encoding* encoding = storage.getEncoding(encodingId);
  if (encoding == nullptr) {
    throw std::runtime_error("ERROR: Invalid encoding_id.");
  }
  str compressed = HuffmanCore::compress(
    text->getOriginalContent(), 
    encoding->getCodeTable()
  );
  if (!storage.addCompressedText(newTextId, text->getOriginalContent(), compressed, encodingId)) {
    throw std::runtime_error("ERROR: Memory overflow.");
  }
  out << "Encoding applied successfully." << std::endl;
}

void nikonov::compareEncodings(Storage& storage, std::istream& in, std::ostream& out)
{
  str textId, encoding1Id, encoding2Id;
  in >> textId >> encoding1Id >> encoding2Id;
  Text* text = storage.getText(textId);
  if (text == nullptr) {
    throw std::runtime_error("ERROR: Invalid text_id.");
  }
  if (text->isCompressed()) {
    throw std::runtime_error("ERROR: Text already compressed.");
  }
  Encoding* encoding1 = storage.getEncoding(encoding1Id);
  Encoding* encoding2 = storage.getEncoding(encoding2Id);
  if (encoding1 == nullptr || encoding2 == nullptr) {
    throw std::runtime_error("ERROR: Invalid encoding_id.");
  }
  str compressed1 = HuffmanCore::compress(text->getOriginalContent(), encoding1->getCodeTable());
  str compressed2 = HuffmanCore::compress(text->getOriginalContent(), encoding2->getCodeTable());
  double ratio1 = static_cast<double>(compressed1.size()) / static_cast<double>(text->getOriginalSizeBits());
  double ratio2 = static_cast<double>(compressed2.size()) / static_cast<double>(text->getOriginalSizeBits());
  out << "Encoding 1: " << compressed1.size() << " bits, compression ratio: " << ratio1 << std::endl;
  out << "Encoding 2: " << compressed2.size() << " bits, compression ratio: " << ratio2 << std::endl;
}

void nikonov::showEncoding(Storage& storage, std::istream& in, std::ostream& out)
{
  str encodingId;
  in >> encodingId;
  Encoding* encoding = storage.getEncoding(encodingId);
  if (encoding == nullptr) {
    throw std::runtime_error("ERROR: Invalid encoding_id.");
  }
  out << "Character | Code" << std::endl;
  out << "----------|-----" << std::endl;
  for (const auto& pair : encoding->getCodeTable()) {
    out << " '" << pair.first << "'     | " << pair.second << std::endl;
  }
}

void nikonov::textStats(Storage& storage, std::istream& in, std::ostream& out)
{
  str textId;
  in >> textId;
  Text* text = storage.getText(textId);
  if (text == nullptr) {
    throw std::runtime_error("ERROR: Invalid text_id.");
  }
  str content = text->getOriginalContent();
  std::unordered_map< char, int > freq;
  for (char c : content) {
    freq[c]++;
  }
  out << "Total characters: " << content.size() << std::endl;
  out << "Unique characters: " << freq.size() << std::endl;
  if (text->isCompressed()) {
    double ratio = static_cast<double>(text->getCompressedSizeBits()) / static_cast<double>(text->getOriginalSizeBits());
    out << "Compression ratio: " << ratio << std::endl;
  }
}

void nikonov::testCompression(Storage& storage, std::istream& in, std::ostream& out)
{
  str textId;
  in >> textId;
  Text* text = storage.getText(textId);
  if (text == nullptr) {
    throw std::runtime_error("ERROR: Invalid text_id.");
  }
  if (text->isCompressed()) {
    throw std::runtime_error("ERROR: Text already compressed.");
  }
  auto codes = HuffmanCore::buildCodes(text->getOriginalContent());
  str compressed = HuffmanCore::compress(text->getOriginalContent(), codes);
  str decompressed = HuffmanCore::decompress(compressed, Encoding(codes).getReverseTable());
  double ratio = static_cast<double>(compressed.size()) / static_cast<double>(text->getOriginalSizeBits());
  out << "Original size: " << text->getOriginalSizeBits() << " bits" << std::endl;
  out << "Compressed size: " << compressed.size() << " bits" << std::endl;
  out << "Compression ratio: " << ratio << std::endl;
  out << "Decompression successful: " << (decompressed == text->getOriginalContent() ? "Yes" : "No") << std::endl;
}

void nikonov::loadTextFromFile(Storage& storage, std::istream& in, std::ostream& out)
{
  str filename, textId;
  in >> filename >> textId;
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("ERROR: File not found.");
  }
  str content((std::istreambuf_iterator< char >(file)), std::istreambuf_iterator< char >());
  if (content.empty()) {
    throw std::runtime_error("ERROR: empty input.");
  }
  if (!storage.addText(textId, content)) {
    throw std::runtime_error("ERROR: Memory overflow.");
  }
  file.close();
  out << "Text loaded from file successfully." << std::endl;
}

void nikonov::readTextFromStdin(Storage& storage, std::istream& in, std::ostream& out)
{
  str textId;
  in >> textId;
  str content;
  std::getline(in, content);
  if (content.empty()) {
    throw std::runtime_error("ERROR: empty input.");
  }
  if (!storage.addText(textId, content)) {
    throw std::runtime_error("ERROR: Memory overflow.");
  }
  out << "Text read from stdin successfully." << std::endl;
}

void nikonov::uploadText(Storage& storage, std::istream& in, std::ostream& out)
{
  str textId, filename;
  in >> textId >> filename;
  Text* text = storage.getText(textId);
  if (text == nullptr) {
    throw std::runtime_error("ERROR: Invalid text_id.");
  }
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("ERROR: Writing has been denied.");
  }
  file << text->getOriginalContent();
  file.close();
  out << "Text uploaded to file successfully." << std::endl;
}

void nikonov::getEncodingFromFile(Storage& storage, std::istream& in, std::ostream& out)
{
  str filename, encodingId;
  in >> filename >> encodingId;
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("ERROR: File not found.");
  }
  std::unordered_map< char, str > codes;
  str line;
  while (std::getline(file, line)) {
    if (line.empty()) continue;
    size_t space_pos = line.find(' ');
    if (space_pos == str::npos) {
      throw std::runtime_error("ERROR: Invalid file.");
    }
    char character = line[0];
    str code = line.substr(space_pos + 1);
    codes[character] = code;
  }
  if (!storage.addEncoding(encodingId, codes)) {
    throw std::runtime_error("ERROR: Memory overflow.");
  }
  file.close();
  out << "Encoding loaded from file successfully." << std::endl;
}

void nikonov::uploadEncoding(Storage& storage, std::istream& in, std::ostream& out)
{
  str encodingId, filename;
  in >> encodingId >> filename;
  Encoding* encoding = storage.getEncoding(encodingId);
  if (encoding == nullptr) {
    throw std::runtime_error("ERROR: Invalid encoding_id.");
  }
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("ERROR: Writing has been denied.");
  }
  for (const auto& pair : encoding->getCodeTable()) {
    file << pair.first << " " << pair.second << std::endl;
  }
  file.close();
  out << "Encoding uploaded to file successfully." << std::endl;
}
