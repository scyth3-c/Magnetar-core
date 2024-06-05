#pragma once
#ifndef ROUTES_DEMAND_HPP
#define ROUTES_DEMAND_HPP

#include <string>

#include <utility>
#include <vector>
#include <initializer_list>
#include <functional>
#include <future>


#include "../request/request.hpp"

#include "../../processing/files/basic_render.h"
#include "../../processing/files/cpp_reader.h"
#include "../../processing/files/mg_reader.h"
#include "../../processing/files/data_render.h"
#include "../json/json.hpp"

using std::string;


struct Route {
private:
    string route_name{},
           route_type{};
    bool lock;
    int timeline;

public:
    [[maybe_unused]] explicit Route(string _route) : route_name(std::move(_route)), lock(false), timeline(0) {}
    Route() = default;

    inline Route& operator = (string _val) { route_name = std::move(_val); return *this; }
    inline void setType(string _type) { route_type = std::move(_type); }
    [[nodiscard]] inline string getName () const noexcept { return route_name; }
    [[nodiscard]] inline string getType () const noexcept { return route_type; }
};


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
    long timekey    {0};

    string response {"default"},
           last,
           headers, guardMsg{};

    static dataRender *_cache;

    public:
    Query()  = default;
    ~Query() = default;

    Request body; // property access
    Request query;

    [[nodiscard]] string  getData()    const noexcept;
    [[nodiscard]] bool    getNext()    const noexcept;

    [[maybe_unused]] void    next()       noexcept;

    void    lock()        noexcept;
    long    getTimeKey()  const noexcept;
    string  getGuardMsg() const noexcept;

    [[maybe_unused]] void    setHeaders(HEADERS) noexcept;
    [[maybe_unused]] void    setHeaders(const string&) noexcept;
    [[maybe_unused]] void    guard(const long&, string custom_msg="") noexcept;


    //  PARAMS:  CONTEN  OPTIONAL CALLBACK

    [[maybe_unused]] void    json(const string&, const std::function<void()>& callback=[]()->void{}) noexcept;
    [[maybe_unused]] void    html(const string&, const std::function<void()>& callback=[]()->void{}) noexcept;
    [[maybe_unused]] void    send(const string&, const std::function<void()>& callback=[]()->void{}) noexcept;
    [[maybe_unused]] void    readFile(const string&,const string&, const std::function<void()>& callback=[]()->void{}) noexcept;
    [[maybe_unused]] void    readFileX(const string&,const string&, const std::function<void()>& callback=[]()->void{}) noexcept;
    [[maybe_unused]] void    compose(const string&,int, const std::function<void()>& callback=[]()->void{}) noexcept;
    [[maybe_unused]] void    render(const string&, const std::function<dataRender(dataRender&)>& callback=[](dataRender&)->dataRender{ return *_cache; }) noexcept;

    // PARAMS:  CONTEN  STATUS OPTIONAL CALLBACK
    [[maybe_unused]] void    json(const string&, int, const std::function<void()>& callback=[]()->void{}) noexcept;
    [[maybe_unused]] void    html(const string&, int, const std::function<void()>& callback=[]()->void{}) noexcept;
    [[maybe_unused]] void    send(const string&, int, const std::function<void()>& callback=[]()->void{}) noexcept;

};


template <class... P>
struct Core_init_t  {

    [[maybe_unused]] Core_init_t(std::initializer_list<P...> list) :
                functions(std::move(list)) {}
    [[maybe_unused]] Core_init_t() = default;

    std::vector<P...> functions;
    Query *remote_control{};

    [[nodiscard]] [[maybe_unused]] inline size_t size() const noexcept { return functions.size(); }

     std::pair<string, long> execute(string _raw, string headers, std::unique_ptr<string> &guardmsg) {
        remote_control = new Query();

        string response{};
        long timekey{0};

        remote_control->body.clear_parameters();
        remote_control->body.setRawParametersData(std::move(_raw));
        remote_control->body.setRawHeadersData(std::move(headers));

        // middlewares execution
        for (size_t i = 0; i < functions.size(); i++) {
            remote_control->lock();

            functions[i](*remote_control);
            if(remote_control->getNext())
                continue;
            else
                break;
        }

        response += remote_control->getData();
        timekey = remote_control->getTimeKey();
        if(timekey > 0)
            guardmsg = std::make_unique<string>(remote_control->getGuardMsg());

        delete remote_control;
        return {response, timekey};
    }
};

typedef Core_init_t<std::function<void(Query&)>> xcallargs;

struct listen_routes {
    listen_routes(string _route, xcallargs callargs, string _type) : callbacks(std::move(callargs)), timekey(0) {
    route = std::move(_route);
    route.setType(std::move(_type));
    }

    Route route;
    xcallargs callbacks;
    long timekey;
    std::chrono::time_point<std::chrono::system_clock> time_point{};
    std::unique_ptr<string> guardRouteMsg = nullptr;
};

#endif /*ROUTES_DEMAND_HPP */