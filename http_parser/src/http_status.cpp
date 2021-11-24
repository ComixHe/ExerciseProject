#include <http_status.hpp>
namespace http {
status num_to_status(unsigned int n) {
  switch (n) {
  case 100:
  case 101:
  case 102:
  case 200:
  case 201:
  case 202:
  case 203:
  case 204:
  case 205:
  case 206:
  case 207:
  case 208:
  case 226:
  case 300:
  case 301:
  case 302:
  case 303:
  case 304:
  case 305:
  case 307:
  case 400:
  case 401:
  case 402:
  case 403:
  case 404:
  case 405:
  case 406:
  case 407:
  case 408:
  case 409:
  case 410:
  case 411:
  case 412:
  case 413:
  case 414:
  case 415:
  case 416:
  case 417:
  case 418:
  case 421:
  case 422:
  case 423:
  case 424:
  case 426:
  case 428:
  case 429:
  case 431:
  case 444:
  case 451:
  case 499:
  case 500:
  case 501:
  case 502:
  case 503:
  case 504:
  case 505:
  case 506:
  case 507:
  case 508:
  case 510:
  case 511:
  case 599:
    return static_cast<status>(n);
    [[fallthrough]];
  default:
    break;
  }
  return status::Unknown;
}

status_categories get_status_categories(unsigned int c) {
  switch (c / 100) {
  case 1:
    return status_categories::Informational;
  case 2:
    return status_categories::Successful;
  case 3:
    return status_categories::Redirection;
  case 4:
    return status_categories::Client_Error;
  case 5:
    return status_categories::Server_Error;
  }
  return status_categories::Unknown;
}

status_categories get_status_categories(status s) {
  return get_status_categories(static_cast<unsigned int>(s));
}

std::string_view get_status_reason(status s) {
  switch (s) {
  case status::Continue:
    return "Continue";
  case status::Switching_Protocols:
    return "Switching Protocols";
  case status::Processing:
    return "Processing";
  case status::OK:
    return "OK";
  case status::Created:
    return "Created";
  case status::Accepted:
    return "Accepted";
  case status::Non_Authoritative_Information:
    return "Non-Authoritative Information";
  case status::No_Content:
    return "No Content";
  case status::Reset_Content:
    return "Reset Content";
  case status::Partial_Content:
    return "Partial Content";
  case status::Multi_Status:
    return "Multi-Status";
  case status::Already_Reported:
    return "Already Reported";
  case status::IM_Used:
    return "IM Used";
  case status::Multiple_Choices:
    return "Multiple Choices";
  case status::Moved_Permanently:
    return "Moved Permanently";
  case status::Found:
    return "Found";
  case status::See_Other:
    return "See Other";
  case status::Not_Modified:
    return "Not Modified";
  case status::Use_Proxy:
    return "Use Proxy";
  case status::Temporary_Redirect:
    return "Temporary Redirect";
  case status::Permanent_Redirect:
    return "Permanent Redirect";
  case status::Bad_Request:
    return "Bad Request";
  case status::Unauthorized:
    return "Unauthorized";
  case status::Payment_Required:
    return "Payment Required";
  case status::Forbidden:
    return "Forbidden";
  case status::Not_Found:
    return "Not Found";
  case status::Method_Not_Allowed:
    return "Method Not Allowed";
  case status::Not_Acceptable:
    return "Not Acceptable";
  case status::Proxy_Authentication_Required:
    return "Proxy Authentication Required";
  case status::Request_Timeout:
    return "Request Timeout";
  case status::Conflict:
    return "Conflict";
  case status::Gone:
    return "Gone";
  case status::Length_Required:
    return "Length Required";
  case status::Precondition_Failed:
    return "Precondition Failed";
  case status::Payload_Too_Large:
    return "Payload Too Large";
  case status::Uri_Too_Long:
    return "URI Too Long";
  case status::Unsupported_Media_Type:
    return "Unsupported Media Type";
  case status::Range_Not_Satisfiable:
    return "Range Not Satisfiable";
  case status::Expectation_Failed:
    return "Expectation Failed";
  case status::Misdirected_Request:
    return "Misdirected Request";
  case status::Unprocessable_Entity:
    return "Unprocessable Entity";
  case status::Locked:
    return "Locked";
  case status::Failed_Dependency:
    return "Failed Dependency";
  case status::Upgrade_Required:
    return "Upgrade Required";
  case status::Precondition_Required:
    return "Precondition Required";
  case status::Too_Many_Requests:
    return "Too Many Requests";
  case status::Request_Header_Fields_Too_Large:
    return "Request Header Fields Too Large";
  case status::Connection_Closed_Without_Response:
    return "Connection Closed Without Response";
  case status::Unavailable_For_Legal_Reasons:
    return "Unavailable For Legal Reasons";
  case status::Client_Closed_Request:
    return "Client Closed Request";
  case status::Internal_Server_Error:
    return "Internal Server Error";
  case status::Not_Implemented:
    return "Not Implemented";
  case status::Bad_Gateway:
    return "Bad Gateway";
  case status::Service_Unavailable:
    return "Service Unavailable";
  case status::Gateway_Timeout:
    return "Gateway Timeout";
  case status::Http_Version_Not_Supported:
    return "HTTP Version Not Supported";
  case status::Variant_Also_Negotiates:
    return "Variant Also Negotiates";
  case status::Insufficient_Storage:
    return "Insufficient Storage";
  case status::Loop_Detected:
    return "Loop Detected";
  case status::Not_Extended:
    return "Not Extended";
  case status::Network_Authentication_Required:
    return "Network Authentication Required";
  case status::Network_Connect_Timeout_Error:
    return "Network Connect Timeout Error";
  default:
    break;
  }
  return "Unknown Status";
}

std::ostream &operator<<(std::ostream &os, status s) {
  return os << get_status_reason(s);
}
}; // namespace http