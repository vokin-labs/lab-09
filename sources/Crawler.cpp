//
// Created by alesandr on 15.04.2021.
//
#include <Crawler.hpp>
#include <Downloader.h>
#include <Parser.h>

void Crawl::start(const string& url, const int& num_loaders, const int& num_parsers) {
  Downloader downloader(num_loaders);
  Parser parser(num_parsers,all_img);
  std::deque<string> first_all_links;
  std::deque<Html> first_htmls;
  first_all_links.push_back(url);
  all_levels.emplace_back(first_all_links, first_htmls);
  for (int i = 0; i < depth; ++i){
    std::deque<string> next_all_links;
    std::deque<Html> next_htmls;
    all_levels.emplace_back(next_all_links, next_htmls);
    while(!all_levels[i].first.empty() || !all_levels[i].second.empty()){
      if(!all_levels[i].first.empty()) {
        all_levels[i].second.push_back(downloader.load_html_list(all_levels[i].first.front()));
        all_levels[i].first.pop_front();
      }
      if(!all_levels[i].second.empty()){
        for(auto & a : parser.parse(all_levels[i].second.front(), (i + 1 == depth))){
          all_levels[i+1].first.push_back(a);
        }
        all_levels[i].second.pop_front();
      }
    }
  }
}



Crawl::~Crawl() {
  print_img_to_file(path_out_file);
}
void Crawl::print_img_to_file(const string& file_path) {
  std::ofstream ofs{file_path};
  for (const auto& img : all_img){
    ofs << img << std::endl;
  }
  ofs.close();
}
