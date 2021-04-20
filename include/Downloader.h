//
// Created by alesandr on 19.04.2021.
//

#ifndef PAVUK_DOWNLOADER_H
#define PAVUK_DOWNLOADER_H
#include <Crawler.hpp>

class Downloader{
 public:
  Downloader(const int& num_workers) : loaders(num_workers){};

  Html load_html_list(const string& url);

  static Html loading_https(string& host, const string& target);

  static Html loading_http(const string& host, const string& target);

 private:

  ThreadPool loaders;
};
#endif  // PAVUK_DOWNLOADER_H
