#ifndef HTTP_VERB_HPP
#define HTTP_VERB_HPP

#include <iostream>
#include <string_view>

namespace http {

enum class verb : unsigned {
  Unknown = 0,
  Get,
  Delete,
  Head,
  Options,
  Patch,
  Post,
  Put,
  Trace,
  Connect
};

verb string_to_verb(std::string_view s);

std::string to_string(verb v);

std::ostream &operator<<(std::ostream os, verb v);
}; // namespace http

#endif