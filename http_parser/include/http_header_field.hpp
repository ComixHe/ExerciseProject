#ifndef HTTP_HEADER_FIELD_HPP
#define HTTP_HEADER_FIELD_HPP

#include <array>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace http {
enum class field : unsigned short {
  Unknown = 0,
  Accept_Charset,
  Accept_Encoding,
  Accept_Language,
  Accept_Patch,
  Accept_Post,
  Accept_Range,
  Accept,
  Access_Control_Allow_Credentials,
  Access_Control_Allow_Headers,
  Access_Control_Allow_Method,
  Access_Control_Allow_Origin,
  Access_Control_Expose_Header,
  Access_Control_Max_Age,
  Access_Control_Request_Header,
  Access_Control_Request_Method,
  Age,
  Allow,
  Alt_Svx,
  Authorization,
  Cache_Control,
  Clear_Site_Data,
  Connection,
  Content_Disposition,
  Content_Encoding,
  Content_Language,
  Content_Length,
  Content_Location,
  Content_Range,
  Content_Type,
  Cookie,
  Data,
  Digest,
  DNT,
  Etag,
  Expect,
  Expires,
  Forwarded,
  From,
  Host,
  If_Match,
  If_Modified_Since,
  If_None_Match,
  If_Range,
  If_Unmodified_Since,
  Keep_Alive,
  Last_Modified,
  Link,
  Location,
  Origin,
  Proxy_Authenticate,
  Proxy_Authorization,
  Range,
  Referer,
  Retry_After,
  Save_Data,
  Sec_Fetch_Dest,
  Sec_Fetch_Mode,
  Sec_Fetch_Site,
  Sec_Fetch_User,
  Server_Timing,
  Server,
  Set_Cookie,
  Strict_Transport_Security,
  Te,
  Timing_Allow_Origin,
  Tk,
  Trailer,
  Transfer_Encoding,
  Upgrade_Insecure_Requests,
  Upgrade,
  User_Agent,
  Vary,
  Via,
  Want_Digest,
  Waring,
  WWW_Authenticate
};

class field_table {
  using array_type = std::array<std::string, 77>;

  array_type table;
  std::unordered_map<std::string, field> map;

public:
  field_table();
  std::size_t size() const { return table.size(); }

  auto begin() const { return table.begin(); }

  auto end() const { return table.end(); }

  const auto &get_map() const { return map; }

  const auto &get_table() const { return table; }
};

static const field_table map_table;

std::string to_string(field f);

field string_to_field(std::string s);
}; // namespace http
#endif