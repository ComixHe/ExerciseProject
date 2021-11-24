#include <cstring>
#include <http_parser.hpp>
#include <iostream>

int main() {
  const char *msg = "HTTP/1.1 200 OK\r\n\
Server: nginx\r\n\
Data: Tue, 09 Nov 2021 03:07:46 GMT\r\n\
Content-Type: text/html; charset=utf-8\r\n\
Content-Length: 218\r\n\
Connection: close\r\n\
\r\n\
<html>\n\
<head>\n\
<title>我的第一个 HTML 页面</title>\n\
</head>\n\
<body>\n\
<p>body 元素的内容会显示在浏览器中。</p>\n\
<p>title 元素的内容会显示在浏览器的标题栏中。</p>\n\
</body>\n\
</html>";
  http::http_parser<http::http_response_type> parser;
  parser.parser(msg, msg + strlen(msg));
  std::cout << std::string(msg) << std::endl;
  std::cout << "-----------------------------------" << std::endl;
  auto &p_msg = parser.get_msg();
  std::cout << parser.get_header_content(p_msg.get_http_header());
  std::cout << p_msg.get_http_body().get_content();
  return 0;
}