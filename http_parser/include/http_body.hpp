#ifndef HTTP_BODY_HPP
#define HTTP_BODY_HPP
#include <cstddef>
#include <memory>
#include <string_view>

namespace http {

class http_body {
  std::string body_buffer{""};

public:
  http_body() = default;
  http_body(http_body &&) = default;
  http_body(const http_body &) = default;
  std::string_view get_content();
  std::size_t put_normal_content(std::string_view content,
                                 std::size_t content_length);
  std::size_t put_chunked_content(std::string_view content);
};

}; // namespace http

#endif