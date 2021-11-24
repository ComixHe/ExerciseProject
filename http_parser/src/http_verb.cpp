#include <http_verb.hpp>
namespace http {
verb string_to_verb(std::string_view s) {
  if (s.size() < 3)
    return verb::Unknown;
  auto prefix = s[0];
  s.remove_prefix(1);
  switch (prefix) {
  case 'C':
    if (s == "ONNECT")
      return verb::Connect;
    break;
  case 'G':
    if (s == "ET")
      return verb::Get;
    break;
  case 'D':
    if (s == "ELETE")
      return verb::Delete;
    break;
  case 'H':
    if (s == "EAD")
      return verb::Head;
    break;
  case 'O':
    if (s == "PTIONS")
      return verb::Options;
    break;
  case 'T':
    if (s == "RACE")
      return verb::Trace;
    break;
  case 'P':
    if (s == "UT")
      return verb::Put;
    else if (s == "OST")
      return verb::Post;
    else if (s == "ATCH")
      return verb::Patch;
    break;
  default:
    break;
  }
  return verb::Unknown;
}

std::string to_string(verb v) {
  switch (v) {
  case verb::Delete:
    return "DELETE";
  case verb::Head:
    return "HEAD";
  case verb::Options:
    return "OPTIONS";
  case verb::Patch:
    return "PATCH";
  case verb::Post:
    return "POST";
  case verb::Put:
    return "PUT";
  case verb::Trace:
    return "TRACE";
  case verb::Get:
    return "GET";
  case verb::Connect:
    return "CONNECT";
  default:
    return "UNKNOWN";
  }
}

std::ostream &operator<<(std::ostream os, verb v) { return os << to_string(v); }

}; // namespace http