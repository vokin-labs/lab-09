//
// Created by alesandr on 19.04.2021.
//

#include "Downloader.h"

Html Downloader::loading_https(string& host, const string& target) {
  try
  {
    const string port = "443"; // https - 443, http - 80
    const int version = 11;
    boost::asio::io_context ioc;
    ssl::context ctx{ ssl::context::sslv23_client };
    load_root_certificates(ctx);
    tcp::resolver resolver{ ioc };
    ssl::stream<tcp::socket> stream{ ioc, ctx };
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
    {
      boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
      throw boost::system::system_error{ ec };
    }
    auto const results = resolver.resolve(host, port);
    boost::asio::connect(stream.next_layer(), results.begin(), results.end());
    stream.handshake(ssl::stream_base::client);
    http::request<http::string_body> req{ http::verb::get, target, version };
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    http::write(stream, req);
    boost::beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(stream, buffer, res);

    string string_body = boost::beast::buffers_to_string(res.body().data());
    boost::system::error_code ec;
    stream.shutdown(ec);
    if (ec == boost::asio::error::eof)
    {
      ec.assign(0, ec.category());
    }
    if (ec) {
      throw boost::system::system_error{ec};
    }
    return {"https", host, string_body};
    // If we get here then the connection is closed gracefully
  }
  catch (std::exception const& e)
  {
    std::cerr << "Error on https: " << e.what() << std::endl;
    return Html{};
  }
}

Html Downloader::loading_http(const string& host, const string& target) {
  try
  {
    const string  port = "80"; // https - 443, http - 80
    const int version = 11;
    boost::asio::io_context ioc;
    tcp::socket socket{ ioc };
    tcp::resolver resolver{ ioc };
    auto const results = resolver.resolve(host, port);
    boost::asio::connect(socket, results.begin(), results.end());
    http::request<http::string_body> req{ http::verb::get, target, version };
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    http::write(socket, req);
    boost::beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(socket, buffer, res);
    string string_body = boost::beast::buffers_to_string(res.body().data());
    boost::system::error_code ec;
    socket.shutdown(tcp::socket::shutdown_both, ec);
    if (ec == boost::asio::error::eof)
    {
      ec.assign(0, ec.category());
    }
    if (ec)
      throw boost::system::system_error{ ec };
    return {"http", host, string_body};
  }
  catch (std::exception const& e)
  {
    std::cerr << "Error on http: " << e.what() << std::endl;
    return Html{};
  }
}

Html Downloader::load_html_list(const string& url_) {
  auto res = loaders.enqueue([&](const string& url){
      string host = url.substr(url.find(':')+3).substr(0,url.substr(url.find(':')+3).find('/'));
      if (url.find(':') == 5){
        string target = (url == string{"https://" + host + url.substr(url.find(':')+3).substr(url.substr(url.find(':')+3).find('/'))}) ? "/" : url.substr(url.find(':')+3).substr(url.substr(url.find(':')+3).find('/'));
        return loading_https(host, target);
      } else if (url.find(':') == 4) {
        string target = (url == string{"http://" + host + url.substr(url.find(':')+3).substr(url.substr(url.find(':')+3).find('/'))}) ? "/" : url.substr(url.find(':')+3).substr(url.substr(url.find(':')+3).find('/'));
        return loading_http(host, target);
      } else
        return Html{};
  }, url_);
  return res.get();
}
