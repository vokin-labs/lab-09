//
// Created by alesandr on 16.04.2021.
//

#ifndef PAVUK_CRAWLER_H
#define PAVUK_CRAWLER_H
#include "../third-party/beast/root_certificates.hpp"
#include "../third-party/ThreadPool/ThreadPool.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <utility>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/program_options.hpp>
#include <vector>
#include <chrono>
#include <queue>
#include <deque>
#include <gumbo.h>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace po = boost::program_options;

using string = std::string;
using std::cout;

  struct Html{
    string protocol{};
    string host{};
    string html{};
  };

class Crawl{
 public:
  Crawl(int depth_, string path) :
                                      depth(depth_),
                                      path_out_file(std::move(path)){};
  ~Crawl();

  void start(const string& url, const int& num_loaders, const int& num_parsers);

  void print_img_to_file(const string& file_path);
 private:
  int depth;
  string path_out_file;
  std::vector<string> all_img;
  std::deque<std::pair<std::deque<string>, std::deque<Html>>> all_levels;
};
#endif  // PAVUK_CRAWLER_H
