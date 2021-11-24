#include <http_error.hpp>

namespace http {
std::string_view to_string(error e) {
  switch (e) {
  case error::end_of_stream:
    return "end of stream";
  case error::partial_message:
    return "partial message";
  case error::need_more:
    return "need more";
  case error::end_of_chunk:
    return "end of chunk";
  case error::header_limit:
    return "header limit exceeded";
  case error::body_limit:
    return "body limit exceeded";
  case error::bad_line_ending:
    return "bad line ending";
  case error::bad_method:
    return "bad method";
  case error::bad_target:
    return "bad target";
  case error::bad_version:
    return "bad version";
  case error::bad_status:
    return "bad status";
  case error::bad_reason:
    return "bad reason";
  case error::bad_field:
    return "bad field";
  case error::bad_value:
    return "bad value";
  case error::bad_content_length:
    return "bad Content-Length";
  case error::bad_transfer_encoding:
    return "bad Transfer-Encoding";
  case error::bad_chunk:
    return "bad chunk";
  case error::bad_chunk_extension:
    return "bad chunk extension";
  case error::bad_obs_fold:
    return "bad obs-fold";
  case error::broken_parser:
    return "broken parser";
  case error::short_read:
    return "unexpected eof in body";
  default:
    return "unknown http error";
  }
}
}; // namespace http