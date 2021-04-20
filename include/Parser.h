//
// Created by alesandr on 19.04.2021.
//

#ifndef PAVUK_PARSER_H
#define PAVUK_PARSER_H
#include <Crawler.hpp>

class Parser{
 public:
  Parser(const int& num_workers,
         std::vector<string>& ref_all_img_)
      : parsers(num_workers),
        ref_all_img(ref_all_img_) {};

  void search_for_links(GumboNode* node, const Html& html);

  void find_links(const Html& html);

  void search_for_img(GumboNode* node, const Html& html);

  void find_img(const Html& html);

  std::deque<string> parse(const Html& html, bool flag);

 private:

  ThreadPool parsers;
  std::deque<string> all_links;
  std::vector<string>& ref_all_img;
};
#endif  // PAVUK_PARSER_H
