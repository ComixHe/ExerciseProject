#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP
#include <http_header_field.hpp>
#include <http_status.hpp>
#include <http_verb.hpp>
#include <string>
#include <vector>

namespace http {
using http_request_type = std::true_type;
using http_response_type = std::false_type;
using const_iterator =
    std::vector<std::pair<field, std::string>>::const_iterator;

class generic_header {
  std::string version;
  std::vector<std::pair<field, std::string>> fields;

public:
  generic_header() = default;
  virtual ~generic_header() = default;
  generic_header(const generic_header &other);
  generic_header(generic_header &&r_other);
  generic_header &operator=(const generic_header &other);
  generic_header &operator=(generic_header &&r_other);

  virtual void append_attr(field f, std::string_view sv);
  virtual std::string_view get_attr(field f);
  virtual void set_http_version(std::string_view sv);
  virtual std::string_view get_http_version();
  virtual const_iterator cbegin();
  virtual const_iterator cend();
};

class request_header : public generic_header {
  verb v;
  std::string path;

public:
  request_header() = default;
  ~request_header() = default;
  request_header(const request_header &other);
  request_header(request_header &&r_othre);
  request_header &operator=(const request_header &other);
  request_header &operator=(request_header &&r_other);

  virtual void set_http_verb(verb new_v);
  virtual verb get_http_verb();
  virtual void set_request_path(std::string_view sv);
  virtual std::string_view get_request_path();
};

class response_header : public generic_header {
  status s;
  std::string reason;

public:
  response_header() = default;
  ~response_header() = default;
  response_header(const response_header &other);
  response_header(response_header &&r_other);
  response_header &operator=(const response_header &other);
  response_header &operator=(response_header &&r_other);

  virtual status get_status();
  virtual void set_status(status new_s);
  virtual std::string_view get_reason();
  virtual void set_reason(std::string_view sv);
  virtual bool is_valid();
};

template <typename header_type> class http_header {};

template <> class http_header<http_request_type> : public request_header {};

template <> class http_header<http_response_type> : public response_header {};
} // namespace http
#endif