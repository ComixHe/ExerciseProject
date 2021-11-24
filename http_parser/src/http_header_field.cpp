#include <http_header_field.hpp>

namespace http {

field_table::field_table()
    : table({"Unknown",
             "Accept-Charset",
             "Accept-Encoding",
             "Accept-Language",
             "Accept-Patch",
             "Accept-Post",
             "Accept-Range",
             "Accept",
             "Access-Control-Allow-Credentials",
             "Access-Control-Allow-Headers",
             "Access-Control-Allow-Method",
             "Access-Control-Allow-Origin",
             "Access-Control-Expose-Header",
             "Access-Control-Max-Age",
             "Access-Control-Request-Header",
             "Access-Control-Request-Method",
             "Age",
             "Allow",
             "Alt-Svx",
             "Authorization",
             "Cache-Control",
             "Clear-Site-Data",
             "Connection",
             "Content-Disposition",
             "Content-Encoding",
             "Content-Language",
             "Content-Length",
             "Content-Location",
             "Content-Range",
             "Content-Type",
             "Cookie",
             "Data",
             "Digest",
             "DNT",
             "Etag",
             "Expect",
             "Expires",
             "Forwarded",
             "From",
             "Host",
             "If-Match",
             "If-Modified-Since",
             "If-None-Match",
             "If-Range",
             "If-Unmodified-Since",
             "Keep-Alive",
             "Last-Modified",
             "Link",
             "Location",
             "Origin",
             "Proxy-Authenticate",
             "Proxy-Authorization",
             "Range",
             "Referer",
             "Retry-After",
             "Save-Data",
             "Sec-Fetch-Dest",
             "Sec-Fetch-Mode",
             "Sec-Fetch-Site",
             "Sec-Fetch-User",
             "Server-Timing",
             "Server",
             "Set-Cookie",
             "Strict-Transport-Security",
             "Te",
             "Timing-Allow-Origin",
             "Tk",
             "Trailer",
             "Transfer-Encoding",
             "Upgrade-Insecure-Requests",
             "Upgrade",
             "User-Agent",
             "Vary",
             "Via",
             "Want-Digest",
             "Waring",
             "WWW-Authenticate"}) {
  map.reserve(table.size());
  for (unsigned short i = 0; i < 77; ++i)
    map.emplace(std::make_pair(table[i], static_cast<field>(i)));
}

std::string to_string(field f) {
  const auto &v = map_table.get_table();
  if (static_cast<unsigned>(f) > v.size())
    return v.begin()[0];
  return v.begin()[static_cast<unsigned>(f)];
}

field string_to_field(std::string s) {
  const auto &map = map_table.get_map();
  auto result = map.find(s);
  if (result != map.end())
    return result->second;
  return field::Unknown;
}

std::ostream &operator<<(std::ostream &os, field f) {
  os << to_string(f);
  return os;
};

}; // namespace http