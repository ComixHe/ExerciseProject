
#include <http_parser.hpp>

namespace http {

template <>
std::string http_parser<http_request_type>::get_header_content(
    http_header<http_request_type> &header) {
  std::string header_content(to_string(header.get_http_verb()));
  header_content += (blank + std::string(header.get_request_path()) + blank +
                     std::string(header.get_http_version()) + end_flag);
  for (auto it = header.cbegin(); it != header.cend(); ++it) {
    header_content +=
        (to_string(it->first) + line_split + blank + it->second + end_flag);
  }
  header_content += end_flag;
  return header_content;
}

template <>
std::string http_parser<http_response_type>::get_header_content(
    http_header<http_response_type> &header) {
  std::string header_content(header.get_http_version());
  header_content += (blank +
                     std::string(std::to_string(
                         static_cast<unsigned int>(header.get_status()))) +
                     blank + std::string(header.get_reason()) + end_flag);
  for (auto it = header.cbegin(); it != header.cend(); ++it) {
    header_content +=
        (to_string(it->first) + line_split + blank + it->second + end_flag);
  }
  header_content += end_flag;
  return header_content;
}

template <>
const char *http_parser<http_request_type>::parser_top(const char *start,
                                                       const char *end,
                                                       error &ec) {
  auto raw_p = parser_line(start, end, ec);
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
  auto &header = msg.get_http_header();
  auto top_line = std::string(start, p + 1);
  std::istringstream stream(top_line);
  std::string word;
  std::getline(stream, word, ' ');
  auto v = string_to_verb(word);
  if (v == verb::Unknown) {
    ec = error::bad_method;
    parser_buffer.fill('\0');
    return nullptr;
  }
  header.set_http_verb(v);
  std::getline(stream, word, ' ');
  header.set_request_path(word);
  std::getline(stream, word, ' ');
  word.resize(word.size() - 2);
  if (word != "HTTP/1.0" || word != "HTTP/1.1") {
    ec = error::bad_version;
    parser_buffer.fill('\0');
    return nullptr;
  }
  header.set_http_version(word);
  ec = error::need_more;
  p_state = parser_state::parser_header;
  l_state = line_state::line_close;
  l_location = line_location::raw_buffer;
  return raw_p + 1;
}

template <>
const char *http_parser<http_response_type>::parser_top(const char *start,
                                                        const char *end,
                                                        error &ec) {
  auto raw_p = parser_line(start, end, ec);
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
  auto &header = msg.get_http_header();
  auto top_line = std::string(start, p + 1);
  std::istringstream stream(top_line);
  std::string word;
  std::getline(stream, word, ' ');
  std::string_view version(word);
  if (version != "HTTP/1.0" && version != "HTTP/1.1") {
    ec = error::bad_version;
    parser_buffer.fill('\0');
    return nullptr;
  }
  header.set_http_version(word);
  std::getline(stream, word, ' ');
  auto s = num_to_status(std::stoi(word));
  if (s == status::Unknown) {
    ec = error::bad_status;
    parser_buffer.fill('\0');
    return nullptr;
  }
  header.set_status(s);
  std::getline(stream, word, ' ');
  word.resize(word.size() - 2);
  auto reason = std::string(get_status_reason(s));
  if (reason != word) {
    ec = error::bad_reason;
    parser_buffer.fill('\0');
    return nullptr;
  }
  header.set_reason(reason);
  ec = error::need_more;
  p_state = parser_state::parser_header;
  l_state = line_state::line_close;
  l_location = line_location::raw_buffer;
  return raw_p + 1;
}

}; // namespace http