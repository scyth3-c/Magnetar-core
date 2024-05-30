#include "parameter_proccess.h"


HTTP_QUERY::HTTP_QUERY() = default;
HTTP_QUERY::~HTTP_QUERY() = default;

string HTTP_QUERY::x_www_form_urlencoded(const string &target, const string& type){
    string params{};
    for (size_t ti = target.length() - 0x1; ti > 0x0; ti--){
        if (target[ti] == ' ' || target[ti] == char(13))
            break;
        params.push_back(target[ti]);
    }
    std::reverse(params.begin(), params.end());
    return type + params;
}

string HTTP_QUERY::selectPerType(const string &target, const string &conten_type, bool &init) const {
    if (conten_type == X_WWW_FORM) {
        init = true;
        return x_www_form_urlencoded(target);
    }
    if(conten_type == PLAIN_TEXT) {
        init = true;
        return x_www_form_urlencoded(target, "data=");
    }
    return get_params(target, init);
}

string HTTP_QUERY::route_refactor_params(const string& _target) const {

    const string content_type = findContenType(_target);
    const string content_type_value = trim(content_type);

    if(content_type != STR_ERR) {
        bool init = false;
        string encoded = selectPerType(_target, content_type_value , init);
        return init ? encoded : NOT_PARAMS;
    }
        return NOT_PARAMS;
}

string HTTP_QUERY::route_refactor_params_get(const string& rawresponse) const {
    bool init = false;
    return get_params(rawresponse, init);
}

string HTTP_QUERY::headers_from(const string& response)  {
    string encoded{};
    char fc = 0x0A;
    for(size_t i = 0; i<response.size(); i++) {
        if(response[i] == fc){
            if(response[i+1]==fc)
                break;
            fc = '\0';
            encoded.clear();
        }
            encoded.push_back(response[i]);
    }
    return encoded;
}

std::pair<string, string> HTTP_QUERY::route_refactor(string target){
    const size_t size = target.size();
    std::pair<string, string> route;
    bool init = false;
    char space = 0x20;

    for (const auto &it : target){
        if (it == space || it == '/')
            break;
        route.first.push_back(it);
    }
    for (size_t ui = 0; ui < size; ui++){
        if (init){
            if (target[ui] == '?' || target[ui] == 'H')
                break;
            if (target[ui] != ' '){
                route.second.push_back(target[ui]);
            }
        }
        else if (target[ui + 0x1] == '/'){
            init = true;
        }
    }
    return route;
}


string HTTP_QUERY::findContenType(string text){
    if (text.empty())  { return STR_ERR; }

    const std::string target = "Content-Type: ";

    std::string content_type;
    size_t index = text.find(target);

    if (index != std::string::npos) {
        index += target.length()  - 1;
        while (text[index] != char(0xd)) {
            content_type.push_back(text[index]);
            index++;
        }
        return content_type;
    } else {
        return STR_ERR;
    }
}


string HTTP_QUERY::get_params(const string &target, bool &init) const{
    string params{};
    for (size_t ui = 3; ui < target.size(); ui++){
        if (init){
            if (target[ui] == 'H' || target[ui] == ' ')
                break;
            params.push_back(target[ui]);
        }
        else if (target[ui] == '?' && ui <= max_iterator){
            init = true;
        }
    }
    if (params.empty() || params.length() <= 1 ){
        return NOT_PARAMS;
    }
    return params;
}

string HTTP_QUERY::trim(string target){
    target.erase(std::remove_if(target.begin(), target.end(), ::isspace), target.end());
    return target;
}