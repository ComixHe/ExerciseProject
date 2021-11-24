#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <algorithm>
#include <array>
#include <http_error.hpp>
#include <http_msg.hpp>
#include <sstream>
#include <string>

namespace http {

static std::string blank(" ");
static std::string end_flag("\r\n");
static std::string line_split(":");

template <typename http_msg_type> class http_parser {
  using msg_type = http_msg_type;

  enum class parser_state {
    parser_top = 0,
    parser_header,
    parser_body,
    parser_done
  };

  enum class line_state { line_open = 0, line_close };

  enum class line_location { raw_buffer = 0, cache_buffer };

public:
  http_msg<msg_type> &get_msg() { return msg; }
  std::string get_header_content(http_header<http_msg_type> &header);
  error parser(const char *src, const char *end) {
    if (p_state == parser_state::parser_done)
      return error::broken_parser;
    if (src == nullptr)
      return error::need_more;
    error ec = error::need_more;
    switch (p_state) {
    case parser_state::parser_top: {
      src = parser_top(src, end, ec);
      if (src == nullptr) {
        p_state = parser_state::parser_done; //代表出错，抛弃整个报文
        return ec;
      }
      if (l_state == line_state::line_open)
        return ec;
    }
    case parser_state::parser_header: {
      src = parser_header(src, end, ec);
      if (src == nullptr) {
        p_state = parser_state::parser_done;
        return ec;
      }
      if (l_state == line_state::line_open)
        return ec;
    }
    case parser_state::parser_body: {
      if (chunk_flag)
        src = parser_chunk_body(src, ec);
      else
        src = parser_body(src, ec);
      if (src == nullptr) {
        p_state = parser_state::parser_done;
        return ec;
      }
      if (ec == error::end_of_stream || ec == error::end_of_chunk)
        p_state = parser_state::parser_done;
      return ec;
    }
    case parser_state::parser_done: {
      return ec;
    }
    }
    return ec;
  }

private:
  std::size_t get_buffer_content_length() {
    auto it = parser_buffer.begin();
    while (*it != '\0')
      ++it;
    --it;
    return std::distance(parser_buffer.begin(), it);
  }

  const char *parser_top(const char *start, const char *end, error &ec);

  const char *parser_header(const char *start, const char *end, error &ec) {
    const char *raw_p;
    auto &header = msg.get_http_header();
    do {
      raw_p = parser_line(start, end, ec);
      if (raw_p == nullptr)
        return nullptr; // line_limit
      if (l_state == line_state::line_open)
        return raw_p;
      const char *p = nullptr;
      if (l_location == line_location::raw_buffer)
        p = raw_p;
      else {
        start = parser_buffer.data();
        p = start + get_buffer_content_length();
      }
      const char *tmp = start;
      while (*tmp != ':')
        ++tmp;
      auto key = string_to_field(std::string(start, tmp));
      if (key == field::Unknown) {
        ec = error::bad_field;
        parser_buffer.fill('\0');
        return nullptr;
      }
      ++tmp;
      while (*tmp == ' ')
        ++tmp;
      std::string_view value(tmp, p - 1);
      header.append_attr(key, value);
      ec = error::need_more;
      p_state = parser_state::parser_body;
      l_state = line_state::line_close;
      l_location = line_location::raw_buffer;
      start = raw_p + 1;
    } while (*start != '\r');
    if (*(start + 1) != '\n') {
      ec = error::bad_line_ending;
      return nullptr;
    }
    auto chunk = std::string(header.get_attr(field::Transfer_Encoding));
    std::transform(chunk.begin(), chunk.end(), chunk.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    if (chunk == "chunked")
      chunk_flag = true;
    return raw_p + 2;
  }

  const char *parser_trailer(const char *c, error &ec) {
    auto &header = msg.get_http_header();
    auto p = c;
    while (true) {
      while (*p != ':')
        ++p;
      --p;
      auto key = string_to_field(std::string(c, p));
      if (key == field::Unknown) {
        ec = error::bad_field;
        return nullptr;
      }
      p += 2;
      while (*p == ' ')
        ++p;
      c = p;
      while (*p != '\r')
        ++p;
      --p;
      std::string_view value(c, p);
      header.append_attr(key, value);
      if (*(p + 1) != '\r' || *(p + 2) != '\n') {
        ec = error::bad_line_ending;
        return nullptr;
      }
      p += 3;
      c = p;
      if (*c == '\r' && *(c + 1) == '\n')
        break;
    }
    return c;
  }

  const char *parser_body(const char *c, error &ec) {
    auto &header = msg.get_http_header();
    auto &body = msg.get_http_body();
    auto length = header.get_attr(field::Content_Length);
    if (length == "") {
      ec = error::bad_value;
      return nullptr;
    }
    auto length_num = std::stoi(std::string(length));
    int real_put = body.put_normal_content(std::string_view(c, c + length_num),
                                           length_num);
    if (real_put != length_num)
      return nullptr;
    ec = error::end_of_stream;
    return c + length_num;
  }

  const char *parser_chunk_body(const char *c, error &ec) {
    if (*c == '0') {
      if (*(c + 1) == '\r' && *(c + 2) == '\n') {
        if (*(c + 3) != '\r')
          parser_trailer(c + 3, ec);
        ec = error::end_of_chunk;
        return c + 1;
      }
      ec = error::bad_chunk;
    }
    auto p = c;
    auto &body = msg.get_http_body();
    while (*p != ';' && *p != '\r')
      ++p;
    --p;
    auto chunk_length = std::stoi(std::string(c, p), 0, 16);
    c = ++p;
    while (*p != '\r')
      ++p;
    if (p != c) {
      ++c, --p;
      std::string_view extension(c, p);
      body.put_chunked_content(extension);
      ++p;
    }
    c = p + 2;
    p += chunk_length;
    std::string_view content(c, p);
    body.put_chunked_content(content);
    ec = error::need_more;
    return p;
  }

  const char *parser_line(const char *start, const char *end,
                          error &ec) { // 在parser_buffer没有空间时返回nullptr
    const char *p = parser_line_check(start, end);
    if (p == nullptr) {
      parser_buffer.fill('\0');
      ec = error::line_limit;
      return nullptr;
    }
    if (l_state == line_state::line_open) {
      auto it = parser_buffer.begin();
      while (*it != '\0')
        ++it;
      if (p - start > std::distance(it, parser_buffer.end())) {
        ec = error::line_limit;
        parser_buffer.fill('\0');
        return nullptr;
      }
      std::copy(start, p, it);
      ec = error::need_more;
      return p;
    } else if (l_state == line_state::line_close && parser_buffer[0] != '\0') {
      auto it = parser_buffer.begin();
      if (p - start > std::distance(it, parser_buffer.end())) {
        ec = error::line_limit;
        parser_buffer.fill('\0');
        return nullptr;
      }
      std::copy(start, p, it);
      l_location = line_location::cache_buffer;
    }
    ec = error::need_more;
    l_location = line_location::raw_buffer;
    return p;
  }

  const char *parser_line_check(const char *start, const char *end) {
    for (; start < end; ++start) {
      if (*start == '\r') {
        if (start + 1 > end) {
          l_state = line_state::line_open;
          return start;
        }
        if (*(start + 1) == '\n') {
          l_state = line_state::line_close;
          return start + 1;
        }
      } else if (*start == '\n') {
        auto length = get_buffer_content_length();
        if (parser_buffer[length] == '\r') {
          if (length + 1 >= parser_buffer.size())
            return nullptr;
          parser_buffer[length + 1] = '\n';
          l_state = line_state::line_close;
          return start;
        }
        l_state = line_state::line_open;
        return start;
      }
    }
    l_state = line_state::line_open;
    return start;
  }
  bool chunk_flag{false};
  parser_state p_state{parser_state::parser_top};
  line_state l_state{line_state::line_close};
  line_location l_location{line_location::raw_buffer};
  http_msg<msg_type> msg;
  std::array<char, 8192> parser_buffer = {'\0'}; // url最大长度
};

} // namespace http

#endif