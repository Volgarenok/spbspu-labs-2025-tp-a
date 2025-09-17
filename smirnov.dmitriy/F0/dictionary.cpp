#include "dictionary.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>


void smirnov::Dictionary::insertWord(const std::string& word) {
    data[word]++;
}

void smirnov::Dictionary::insertFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string word;
    while (file >> word) {
        insertWord(word);
    }
}

bool smirnov::Dictionary::contains(const std::string& word) const {
    return data.find(word) != data.cend();
}

int smirnov::Dictionary::getFrequency(const std::string& word) const {
    auto it = data.find(word);
    if (it != data.end()) return it->second;
    return -1;
}

std::string smirnov::Dictionary::mostFrequent() const {
    if (data.empty()) return "";
    return std::max_element(
        data.begin(), data.end(),
        [](auto &a, auto &b){ return a.second < b.second; }
    )->first;
}

void smirnov::Dictionary::deleteWord(const std::string& word) {
    data.erase(word);
}

void smirnov::Dictionary::clear() {
    data.clear();
}

bool smirnov::Dictionary::empty() const {
    return data.empty();
}

size_t smirnov::Dictionary::size() const {
    return data.size();
}

std::vector<std::pair<std::string,int>> smirnov::Dictionary::getSortedByWord() const {
    std::vector<std::pair<std::string,int>> vec(data.begin(), data.end());
    return vec;
}

std::vector<std::pair<std::string,int>> smirnov::Dictionary::getSortedByFrequency() const {
    std::vector<std::pair<std::string,int>> vec(data.begin(), data.end());
    std::sort(vec.begin(), vec.end(),
        [](auto &a, auto &b){ return a.second > b.second; });
    return vec;
}

double smirnov::Dictionary::getRelativeFrequency(const std::string& word) const {
    if (data.empty() || !contains(word)) return -1.0;
    int total = 0;
    for (auto &p : data) total += p.second;
    return static_cast<double>(data.at(word)) / total;
}

std::vector<std::pair<std::string,double>> smirnov::Dictionary::getTopRelative(int N) const {
    std::vector<std::pair<std::string,double>> vec;
    if (data.empty()) return vec;
    int total = 0;
    for (auto &p : data) total += p.second;
    for (auto &p : data)
        vec.push_back({p.first, static_cast<double>(p.second) / total});
    std::sort(vec.begin(), vec.end(),
        [](auto &a, auto &b){ return a.second > b.second; });
    if ((int)vec.size() < N) return {};
    vec.resize(N);
    return vec;
}

std::vector<std::pair<std::string,double>> smirnov::Dictionary::getBottomRelative(int N) const {
    std::vector<std::pair<std::string,double>> vec;
    if (data.empty()) return vec;
    int total = 0;
    for (auto &p : data) total += p.second;
    for (auto &p : data)
        vec.push_back({p.first, static_cast<double>(p.second) / total});
    std::sort(vec.begin(), vec.end(),
        [](auto &a, auto &b){ return a.second < b.second; });
    if ((int)vec.size() < N) return {};
    vec.resize(N);
    return vec;
}

std::vector<std::pair<std::string,double>> smirnov::Dictionary::getRangeRelative(double min, double max) const {
    std::vector<std::pair<std::string,double>> result;
    if (min > max || data.empty()) return result;
    int total = 0;
    for (auto &p : data) total += p.second;
    for (auto &p : data) {
        double rel = static_cast<double>(p.second) / total;
        if (rel >= min && rel <= max)
            result.push_back({p.first, rel});
    }
    return result;
}

std::vector<std::string> smirnov::Dictionary::medianFrequency() const {
    std::vector<std::string> result;
    if (data.empty()) return result;
    std::vector<int> freqs;
    for (auto &p : data) freqs.push_back(p.second);
    std::sort(freqs.begin(), freqs.end());
    size_t n = freqs.size();
    int median;
    if (n % 2 == 1)
        median = freqs[n/2];
    else
        median = (freqs[n/2 - 1] + freqs[n/2]) / 2;
    for (auto &p : data)
        if (p.second == median)
            result.push_back(p.first);
    return result;
}

