#ifndef ROUTES_HPP
#define ROUTES_HPP

#include <string>
#include <memory>
#include <vector>
#include <initializer_list>
#include <functional>
#include "request.hpp"

#include "../files/basic_reader.hpp"
#include "../files/cpp_reader.hpp"
#include "../files/mg_reader.hpp"
#include "../files/data_render.hpp"

#include "../json/json.hpp"

using std::string;


template<class...P>
struct Headers_t {
    Headers_t(){}
    Headers_t(std::initializer_list<P...>list): body(list) {}
    vector<string> body;
    string generate(){
        string response{""};
        for (auto &it : body) {
            response += it + "\n";
        }
        return response;
    }
};  
typedef Headers_t<string> HEADERS;


class Query {

   private:
   
    bool nexteable  { false};
    string response {"default"};
    string last     {};
    string headers;

    public:
    Query() {}
    ~Query() {}

    Request body; // property access

    [[nodiscard]] string  getData()    const noexcept;
    bool    getNext()    const noexcept;
    
    void    next()       noexcept;
    void    lock()       noexcept;

    void    setHeaders(HEADERS) noexcept;
    void    setHeaders(string) noexcept;

    //  PARAMS:  CONTEN  OPTIONAL CALLBACK
    
    void    json(string, std::function<void()> opcional=[]()->void{}) noexcept;
    void    html(string, std::function<void()> opcional=[]()->void{}) noexcept;
    void    send(string, std::function<void()> opcional=[]()->void{}) noexcept;
    void    readFile(string,string, std::function<void()> opcional=[]()->void{}) noexcept;
    void    readFileX(string,string, std::function<void()> opcional=[]()->void{}) noexcept;
    void    compose(string,int, std::function<void()> opcional=[]()->void{}) noexcept;
    void    render(string, std::function<dataRender(dataRender&)> opcional=[](dataRender&)->dataRender{}) noexcept;

    // PARAMS:  CONTEN  STATUS OPTIONAL CALLBACK
     void    json(string, int, std::function<void()> opcional=[]()->void{}) noexcept;
     void    html(string, int, std::function<void()> opcional=[]()->void{}) noexcept;
     void    send(string, int, std::function<void()> opcional=[]()->void{}) noexcept;

};

string  Query::getData() const noexcept                   {     return last;        }
bool    Query::getNext() const noexcept                   {     return nexteable;   }
void    Query::next()    noexcept                         {     nexteable = true;   }
void    Query::lock()    noexcept                         {     nexteable = false;  }


void Query::json(string _txt, std::function<void()> callback) noexcept {
    last = utility_t::prepare_basic(_txt, "application/json", headers); 
    callback();
}
void Query::html(string _txt,  std::function<void()> callback) noexcept {
    last = utility_t::prepare_basic(_txt, "text/html", headers); 
    callback();
}
void  Query::send(string _txt,  std::function<void()> callback) noexcept {    
     last = utility_t::prepare_basic(_txt, "text/plain", headers);       
     callback();
}



void Query::json(string _txt, int status, std::function<void()> callback) noexcept {
    last = utility_t::prepare_basic(_txt, "application/json", headers, std::to_string(status)); 
    callback();
}
void Query::html(string _txt, int status,  std::function<void()> callback) noexcept {
    last = utility_t::prepare_basic(_txt, "text/html", headers,  std::to_string(status)); 
    callback();
}
void  Query::send(string _txt, int status,  std::function<void()> callback) noexcept {    
     last = utility_t::prepare_basic(_txt, "text/plain", headers,  std::to_string(status));       
     callback();
}


void  Query::readFile(string path,string type, std::function<void()> callback) noexcept {

    BasicRead reader;
    string body = reader.processing(path);
    last = utility_t::prepare_basic(body, type, headers);
    reader.~BasicRead();
    callback();
}

void  Query::readFileX(string path,string type, std::function<void()> callback) noexcept {
    CppReader reader;
    string body = reader.processing(path);
    last = utility_t::prepare_basic(body, type, headers);
    reader.~CppReader();
    callback();
}

void  Query::compose(string path, int reserve, std::function<void()> callback) noexcept {
    
    MgReader reader;
    string body = reader.processing(path, reserve);
    last = utility_t::prepare_basic(body, "text/html" , headers);
    reader.~MgReader();
    callback();
}

void  Query::render(string path, std::function<dataRender(dataRender& data)> callback) noexcept {

std::unique_ptr<dataRender> tasty_temp = std::make_unique<dataRender>(callback);
string body = tasty_temp->render(path);
last = utility_t::prepare_basic(body, "text/html", headers);
tasty_temp.reset();

}



void Query::setHeaders(string body) noexcept {
 headers += body + "\n";
}
void Query::setHeaders(HEADERS box) noexcept{
headers += box.generate();
}


template <class... P>
struct Core_init_t 
{

    Core_init_t(std::initializer_list<P...> list) : 
                functions(std::move(list)) {}
    Core_init_t() {}


    std::vector<P...> functions;
    Query *remote_control;
    string response{""};


inline size_t size() const noexcept { return functions.size(); } 
inline string execute(string _raw) {
        remote_control = new Query();
        for (size_t i = 0; i < functions.size(); i++) {
            remote_control->lock();
            remote_control->body.setRawParametersData(_raw);
            functions[i](*remote_control);
            if(remote_control->getNext() == true) continue;
            else break;
        }
        response += remote_control->getData();
        delete remote_control;
        return response;
    }
};


typedef Core_init_t<std::function<void(Query&)>> _callbacks;

struct Route
{
    private:
        string route_name{},
               route_type{};

    public:
        Route(string _route) : route_name(_route) {}
        Route() {}
       ~Route() {}

    inline void operator= (string _val) { route_name = _val; }
    inline void setType(string _type) { route_type = _type; }
    inline string getName () const noexcept { return route_name; }
    inline string getType () const noexcept { return route_type; }
};

struct listen_routes {
    listen_routes(string _route, _callbacks __callback, string _type) : callbacks(std::move(__callback)) {
    route = _route;
    route.setType(_type);
    }

    Route route;
    _callbacks callbacks;
};

#endif // !ROUTES_HPP