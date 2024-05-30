#pragma once
#ifndef ROUTES_DEMAND_HPP
#define ROUTES_DEMAND_HPP

#include <string>

#include <utility>
#include <vector>
#include <initializer_list>
#include <functional>

#include "../request/request.hpp"

#include "../../processing/files/basic_render.h"
#include "../../processing/files/cpp_reader.h"
#include "../../processing/files/mg_reader.h"
#include "../../processing/files/data_render.h"
#include "../json/json.hpp"

using std::string;

template<class...P>
struct Headers_t {
    Headers_t()= default;
   [[maybe_unused]] Headers_t(std::initializer_list<P...>list): body(list) {}
    vector<string> body;
    string generate(){
        string response;
        for (auto &it : body) {
            response += it + "\n";
        }
        return response;
    }
};  
typedef Headers_t<string> HEADERS;


class Query {
    bool nexteable  { false};
    string response {"default"};
    string last;
    string headers;
    static dataRender *_cache;

    public:
    Query() = default;
    ~Query() = default;

    Request body; // property access
    Request query;

    [[nodiscard]] string  getData()    const noexcept;
    [[nodiscard]] bool    getNext()    const noexcept;

    [[maybe_unused]] void    next()       noexcept;
    void    lock()       noexcept;

    [[maybe_unused]] void    setHeaders(HEADERS) noexcept;
    [[maybe_unused]] void    setHeaders(const string&) noexcept;

    //  PARAMS:  CONTEN  OPTIONAL CALLBACK

    [[maybe_unused]] void    json(const string&, const std::function<void()>& opcional=[]()->void{}) noexcept;
    [[maybe_unused]] void    html(const string&, const std::function<void()>& opcional=[]()->void{}) noexcept;
    [[maybe_unused]] void    send(const string&, const std::function<void()>& opcional=[]()->void{}) noexcept;
    [[maybe_unused]] void    readFile(const string&,const string&, const std::function<void()>& opcional=[]()->void{}) noexcept;
    [[maybe_unused]] void    readFileX(const string&,const string&, const std::function<void()>& opcional=[]()->void{}) noexcept;
    [[maybe_unused]] void    compose(const string&,int, const std::function<void()>& opcional=[]()->void{}) noexcept;
    [[maybe_unused]] void    render(const string&, const std::function<dataRender(dataRender&)>& opcional=[](dataRender&)->dataRender{ return *_cache; }) noexcept;

    // PARAMS:  CONTEN  STATUS OPTIONAL CALLBACK
    [[maybe_unused]] void    json(const string&, int, const std::function<void()>& opcional=[]()->void{}) noexcept;
    [[maybe_unused]] void    html(const string&, int, const std::function<void()>& opcional=[]()->void{}) noexcept;
    [[maybe_unused]] void    send(const string&, int, const std::function<void()>& opcional=[]()->void{}) noexcept;

};

template <class... P>
struct Core_init_t  {

    [[maybe_unused]] Core_init_t(std::initializer_list<P...> list) :
                functions(std::move(list)) {}
    [[maybe_unused]] Core_init_t() = default;

    std::vector<P...> functions;
    Query *remote_control{};

    [[nodiscard]] [[maybe_unused]] inline size_t size() const noexcept { return functions.size(); }
     string execute(string _raw, string headers) {
        remote_control = new Query();
        string response{};
        for (size_t i = 0; i < functions.size(); i++) {
            remote_control->lock();
            remote_control->body.clear_parameters();
            remote_control->body.setRawParametersData(std::move(_raw));
            remote_control->body.setRawHeadersData(std::move(headers));
            functions[i](*remote_control);
            if(remote_control->getNext()) continue;
            else break;
        }
        response += remote_control->getData();
        delete remote_control;
        return response;
    }
};
typedef Core_init_t<std::function<void(Query&)>> xcallargs;

struct Route {
    private:
        string route_name{},
               route_type{};

    public:
       [[maybe_unused]] explicit Route(string _route) : route_name(std::move(_route)) {}
        Route() = default;

    inline Route& operator = (string _val) { route_name = std::move(_val); return *this; }
    inline void setType(string _type) { route_type = std::move(_type); }
    [[nodiscard]] inline string getName () const noexcept { return route_name; }
    [[nodiscard]] inline string getType () const noexcept { return route_type; }
};

struct listen_routes {
    listen_routes(string _route, xcallargs callargs, string _type) : callbacks(std::move(callargs)) {
    route = std::move(_route);
    route.setType(std::move(_type));
    }

    Route route;
    xcallargs callbacks;
};

#endif /*ROUTES_DEMAND_HPP */