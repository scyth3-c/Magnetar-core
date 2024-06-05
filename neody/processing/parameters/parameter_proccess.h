#ifndef PARAM_PROCESSING_HPP
#define PARAM_PROCESSING_HPP

constexpr const char* GET_TYPE = "GET";
constexpr const char* POST_TYPE = "POST";
constexpr const char* PUT_TYPE  = "PUT";
constexpr const char* DELETE_TYPE = "DELETE";
constexpr const char* PATCH_TYPE = "PATCH";

constexpr const char* COPY_TYPE = "COPY";
constexpr const char* HEAD_TYPE = "HEAD";
constexpr const char* OPTIONS_TYPE = "OPTIONS";
constexpr const char* LINK_TYPE = "LINK";
constexpr const char* UNLINK_TYPE = "UNLINK";
constexpr const char* PURGE_TYPE = "PURGE";

constexpr const char* X_WWW_FORM = "application/x-www-form-urlencoded";
constexpr const char* PLAIN_TEXT = "text/plain";
constexpr const char* RAW_ERROR = "data=null";
constexpr const char* RAW_TARGET = "data=";

#define STR_ERR "-1"
#include <algorithm>
#include <memory>
#include <string>

#include "../../http/request/request.hpp"

using std::string;
using std::make_shared;
using std::shared_ptr;

class HTTP_QUERY
{
     size_t max_iterator{50};
public:
    HTTP_QUERY();
    ~HTTP_QUERY();
    
   [[nodiscard]] static string route_refactor_params(const string&);
   [[nodiscard]] static string route_refactor_params_get(const string&);
   [[nodiscard]] static string headers_from(const string&);
   [[nodiscard]] static string get_params(const string &);
   [[nodiscard]] static string x_www_form_urlencoded(const string &);
   [[nodiscard]] static string raw_form_encoded(const string &);
   [[nodiscard]] static string findContenType(const string&);
   [[nodiscard]] static string selectPerType(const string &,const string&, bool&);
   [[nodiscard]] static std::pair<string, string> route_refactor(const string&);
   [[nodiscard]] static string trim(string);
};

#endif //PARAM_PROCESSING_HPP