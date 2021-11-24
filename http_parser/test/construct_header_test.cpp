#include <http_header.hpp>
#include <http_parser.hpp>

int main() {
  http::http_header<http::http_request_type> header;
  header.set_http_verb(http::verb::Get);
  header.set_request_path("/");
  header.set_http_version("HTTP/1.1");
  header.append_attr(http::field::Host, "mirrors.163.com");
  header.append_attr(http::field::Connection, "keep-alive");
  header.append_attr(http::field::DNT, "1");
  header.append_attr(http::field::Upgrade_Insecure_Requests, "1");
  header.append_attr(http::field::User_Agent,
                     "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
                     "(KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36");
  header.append_attr(http::field::Accept_Encoding, "gzip, deflate");
  header.append_attr(http::field::Accept_Language, "zh-CN,zh;q=0.9,en;q=0.8");
  http::http_parser parser;
  auto content = parser.get_header_content(header);
  std::cout << content;
  return 0;
}