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

#define STR_ERR "-1"

#include <algorithm>
#include <memory>
#include <string>
#include <cctype>

#include "../../HTTP/request/request.hpp"


using std::string;
using std::make_shared;
using std::shared_ptr;

class HTTP_QUERY
{
private:
     size_t max_iterator{50};
     shared_ptr<string> route = nullptr;

public:
    HTTP_QUERY();
    ~HTTP_QUERY();
    
   [[nodiscard]] string route_refactor_params(string);
   [[nodiscard]] string route_refactor_params_get(string) const;
   [[nodiscard]] string get_params(string &, bool &) const;
   [[nodiscard]] static string x_www_form_urlencoded(string &, const string& type = "");
   [[nodiscard]] static string findContenType(string);
   [[nodiscard]] string selectPerType(string &, string&, bool&) const;
   [[nodiscard]] static std::pair<string, string> route_refactor(string);
   [[nodiscard]] static string trim(string);
};


#endif //PARAM_PROCESSING_HPP