#ifndef HTTP_ERORR_HPP
#define HTTP_ERROR_HPP

#include <string_view>
namespace http {

enum class error {
  end_of_stream = 1, // eof
  partial_message,   //开始解析但是报文不完整,构建报文时使用
  need_more, //在本段报文未解析完时，需要更多数据，解析报文时使用
  end_of_chunk, // chunk块结束
  header_limit,
  line_limit,
  body_limit,
  bad_line_ending,
  bad_method,
  bad_target,
  bad_version,
  bad_status,
  bad_reason,
  bad_field,
  bad_value,
  bad_content_length,
  bad_transfer_encoding,
  bad_chunk,
  bad_chunk_extension,
  bad_obs_fold,
  broken_parser, //尝试复用解析完成的parser
  short_read //读取系统文件遇到意外错误而终止,构建相应报文的body时用
};

std::string_view to_string(error e);
}; // namespace http
#endif