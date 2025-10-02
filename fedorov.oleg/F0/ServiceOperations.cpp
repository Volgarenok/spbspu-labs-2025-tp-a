#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>
#include <set>

#include "TextProcessor.hpp"
#include "ValidationUtils.hpp"

namespace crossref
{

  using namespace validation;

  void TextProcessor::findCommonLines(const std::string &new_text_id,
                                      const std::string &text_id1,
                                      const std::string &text_id2)
  {
    auto it1 = texts.find(text_id1);
    auto it2 = texts.find(text_id2);

    checkIdNotFound(texts, text_id1, "<TEXT NOT FOUND>");
    checkIdNotFound(texts, text_id2, "<TEXT NOT FOUND>");
    checkIdExists(texts, new_text_id, "<TEXT ID EXISTS>");

    std::set< std::string > set1(it1->second.begin(), it1->second.end());
    std::set< std::string > set2(it2->second.begin(), it2->second.end());

    std::vector< std::string > common;
    std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(common));

    if (common.empty())
    {
      throw std::runtime_error("<NO COMMON LINES>");
    }

    texts[new_text_id] = common;
  }

  void TextProcessor::clearAll()
  {
    texts.clear();
    dicts.clear();
    dictWordOrder.clear();
  }

}
