#ifndef HTTP_MSG_HPP
#define HTTP_MSG_HPP
#include <http_body.hpp>
#include <http_header.hpp>

namespace http {

template <typename header_type, typename body_type = http_body> class http_msg {
  http_header<header_type> hd;
  body_type body;

public:
  using msg_header_type = header_type;
  http_header<header_type> &get_http_header() { return hd; }
  http_body &get_http_body() { return body; }
  void set_http_body(http_body &&new_body) { body = std::move(new_body); }
  void set_http_header(const http_header<header_type> &new_header) {
    hd = new_header;
  }
  void set_http_header(http_header<header_type> &&new_header) {
    hd = std::move(new_header);
  }
};

}; // namespace http
#endif