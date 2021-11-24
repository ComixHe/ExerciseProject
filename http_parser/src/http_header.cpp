#include <http_header.hpp>
#include <http_header_field.hpp>
#include <http_verb.hpp>
#include <iostream>
#include <string_view>
#include <utility>

namespace http {

std::string_view generic_header::get_http_version() { return version; }

void generic_header::set_http_version(std::string_view sv) { version = sv; }

void generic_header::append_attr(field f, std::string_view sv) {
  fields.emplace_back(std::make_pair(f, sv));
}

std::string_view generic_header::get_attr(field f) {
  for (auto it = fields.cbegin(); it != fields.cend(); ++it) {
    if (it->first == f)
      return it->second;
  }
  return "";
}

const_iterator generic_header::cbegin() { return fields.cbegin(); }

const_iterator generic_header::cend() { return fields.cend(); }

generic_header::generic_header(const generic_header &other)
    : version(other.version), fields(other.fields) {}

generic_header::generic_header(generic_header &&r_other)
    : version(std::move(r_other.version)), fields(std::move(r_other.fields)) {}

generic_header &generic_header::operator=(const generic_header &other) {
  if (&other == this)
    return *this;
  version = other.version;
  fields = other.fields;
  return *this;
}

generic_header &generic_header::operator=(generic_header &&r_other) {
  if (&r_other == this)
    return *this;
  version = std::move(r_other.version);
  fields = std::move(r_other.fields);
  return *this;
}

void request_header::set_http_verb(verb new_v) { v = new_v; }

verb request_header::get_http_verb() { return v; }

void request_header::set_request_path(std::string_view sv) { path = sv; }

std::string_view request_header::get_request_path() { return path; }

request_header::request_header(const request_header &other)
    : generic_header(other), v(other.v), path(other.path){};

request_header::request_header(request_header &&r_othre)
    : generic_header(std::move(r_othre)), v(std::move(r_othre.v)),
      path(std::move(r_othre.path)){};

request_header &request_header::operator=(const request_header &other) {
  if (&other == this)
    return *this;
  generic_header::operator=(other);
  v = other.v;
  path = other.path;
  return *this;
};

request_header &request_header::operator=(request_header &&r_other) {
  if (&r_other == this)
    return *this;
  generic_header::operator=(std::move(r_other));
  v = r_other.v;
  path = std::move(r_other.path);
  return *this;
};

status response_header::get_status() { return s; }

void response_header::set_status(status new_s) { s = new_s; }

bool response_header::is_valid() {
  if (get_status_reason(s) != reason)
    return false;
  return true;
}
std::string_view response_header::get_reason() { return reason; }

void response_header::set_reason(std::string_view sv) { reason = sv; }

response_header::response_header(const response_header &other)
    : generic_header(other), s(other.s), reason(other.reason){};

response_header::response_header(response_header &&r_other)
    : generic_header(std::move(r_other)), s(std::move(r_other.s)),
      reason(std::move(r_other.reason)){};

response_header &response_header::operator=(const response_header &other) {
  if (&other == this)
    return *this;
  generic_header::operator=(other);
  s = other.s;
  reason = other.reason;
  return *this;
};

response_header &response_header::operator=(response_header &&r_other) {
  if (&r_other == this)
    return *this;
  generic_header::operator=(std::move(r_other));
  s = r_other.s;
  reason = std::move(r_other.reason);
  return *this;
};

}; // namespace http