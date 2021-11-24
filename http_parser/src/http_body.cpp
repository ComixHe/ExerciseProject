#include <algorithm>
#include <cstddef>
#include <cstring>
#include <http_body.hpp>
#include <string_view>

namespace http {
std::string_view http_body::get_content() {
  return std::string_view(body_buffer);
}

std::size_t http_body::put_normal_content(std::string_view new_content,
                                          std::size_t content_length) {
  body_buffer.resize(content_length);
  std::copy(new_content.begin(), new_content.end(), body_buffer.begin());
  return body_buffer.size();
}

std::size_t http_body::put_chunked_content(std::string_view new_content) {
  body_buffer += new_content;
  body_buffer += "\n";
  return body_buffer.size();
}
}; // namespace http