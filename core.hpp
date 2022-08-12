#ifndef MAGNETAR_HPP
#define MAGNETAR_HPP

#include "sockets.hpp"
#include "sockets_utils.hpp"
#include "routes.hpp"
#include "request.hpp"
#include "processing.hpp"

#include <thread>
#include <chrono>
#include <mutex>
#include <memory>

#include <string>
#include <vector>


using std::string,
    std::make_shared;

template <class T>
class Magnetar
{
private:

    std::shared_ptr<T>           control = nullptr;
    std::shared_ptr<HTTP_QUERY>  qProcess = nullptr;
    std::vector<listen_routes>   routes;
    std::vector<string>          connections;
    std::mutex                   lock;

    string Response    {};
    string sender_data {};

    bool stop_loop{false};

    unsigned int PORT          {0};
    unsigned int sesions       {1};
    unsigned int milis_clock   {60};

    int _domain     = AF_INET,
        type        = SOCK_STREAM,
        protocol    = 0,
        route_seer  = 0,
        max_iterator= 50;

public:
    Magnetar(unsigned int port);
    Magnetar();
    ~Magnetar() {}


    inline void _wait           (int milis)const              {std::this_thread::sleep_for(std::chrono::milliseconds(milis)); }
    inline void setMaxSesions   (unsigned int _max)noexcept   { sesions = _max; }
    inline void setClockPerNext (unsigned int milis)noexcept  { milis_clock = milis; }
    inline auto getConnections  ()noexcept                    { return connections; }

    inline void stopp           ()noexcept                    { stop_loop = true; }
    inline void cleanConnections()noexcept                    { connections.clear(); }
   
    int http_response     (string, _callbacks, string optional);
    int get      (string, _callbacks);
    int post     (string, _callbacks);
    int put      (string, _callbacks);
    int deleteX  (string, _callbacks);
    int patch    (string, _callbacks);
    int head     (string, _callbacks);
    int options  (string, _callbacks);
    int link     (string, _callbacks);
    int unlink   (string, _callbacks);
    int purge    (string, _callbacks);

    int setPort (unsigned int) noexcept ;

    void config                 (int,int,int);
    void setSenderData          (string _data);
    string oneTrigger           (string data = "default", int _port = 0, int flags = 0);
    string getResponse          ();
    void listenSesions          ();
    void listen                 ();
};

//constructors
template <class T>
Magnetar<T>::Magnetar(unsigned int port) {
    if (port >= 100)
        PORT = std::move(port);
    connections = std::vector<string>(sesions);
}
template <class T>
Magnetar<T>::Magnetar(){connections = std::vector<string>(sesions);}
//end constructor


// FUNCTIONS

template <class T>
int Magnetar<T>::http_response(string _xRoute, _callbacks _funcs, string optional_type){
    try {
        routes.push_back({std::move(_xRoute), std::move(_funcs), std::move(optional_type)});
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}

/*
  YES, this is BECAUSE  when using magnetar just use .get() etc
*/
template <class T>
int Magnetar<T>::get(string _xRoute, _callbacks _funcs){
    return http_response (_xRoute, _funcs, GET_TYPE);
}
template <class T>
int Magnetar<T>::post(string _xRoute, _callbacks _funcs){
    return http_response (_xRoute, _funcs, POST_TYPE);
}
template <class T>
int Magnetar<T>::put(string _xRoute, _callbacks _funcs){
    return http_response (_xRoute, _funcs, PUT_TYPE);
}
template <class T>
int Magnetar<T>::deleteX(string _xRoute, _callbacks _funcs){
    return http_response(_xRoute, _funcs, DELETE_TYPE);
}
template <class T>
int Magnetar<T>::patch(string _xRoute, _callbacks _funcs){
    return http_response(_xRoute, _funcs, PATCH_TYPE);
}
template <class T>
int Magnetar<T>::head(string _xRoute, _callbacks _funcs){
    return http_response(_xRoute, _funcs, HEAD_TYPE);
}
template <class T>
int Magnetar<T>::options(string _xRoute, _callbacks _funcs){
    return http_response(_xRoute, _funcs, OPTIONS_TYPE);
}
template <class T>
int Magnetar<T>::link(string _xRoute, _callbacks _funcs){
    return http_response(_xRoute, _funcs, LINK_TYPE);
}
template <class T>
int Magnetar<T>::unlink(string _xRoute, _callbacks _funcs){
    return http_response(_xRoute, _funcs, UNLINK_TYPE);
}
template <class T>
int Magnetar<T>::purge(string _xRoute, _callbacks _funcs){
    return http_response(_xRoute, _funcs, PURGE_TYPE);
}





template <class T>
void Magnetar<T>::listen()
{

    auto listen_loop = [&]() -> void
    {
        while (1)
        {
            auto _process = [&]() -> void {

            bool cantget = true;
            qProcess = make_shared<HTTP_QUERY>();
            control = make_shared<T>(PORT);

                if (!control->create(_domain, type, protocol))
                throw "error al crear";
            control->setSesions(sesions);
                if (!control->on())
                throw "error al lanzar";
            
            string send_target{};

            string trait = (string)control->getResponse();
            std::pair<string,string> actual_route = qProcess->route_refactor(trait);
            std::vector<listen_routes> sesion_routes = routes; // generate COPY  NOT MOVE X

            for (auto &it : sesion_routes) {

                if ( it.route.getType() == actual_route.first && it.route.getName() == actual_route.second) {

                    string parametros = qProcess->route_refactor_params(trait);
                    send_target = it.callbacks.execute(parametros);
                    cantget = false;

                    break;
                }
            }

            sesion_routes.clear();
            if (cantget) {
                control->send_data(ERROR_GET);
            }
            else {
                control->send_data(send_target);
            }
            control->close();
            control.reset();
            qProcess.reset();
            };

            std::thread process (_process);
            if(process.joinable()) process.join();
        }
    };
    std::thread _sender(listen_loop);
    _sender.join();
}

template <class T> 
void Magnetar<T>::config(int domain, int _type, int _protocol){
        _domain = domain;
        type = _type;
        protocol = _protocol;
}

template <class T>
int Magnetar<T>::setPort(unsigned int _port) noexcept {
    if (_port >= 100) {
        PORT = std::move(_port);
    }
    else {
        return -1;
    }
    return 0;
}

template <class T>
void Magnetar<T>::setSenderData(string _data) {
    if (!_data.empty())
        sender_data = _data;
}

template <class T>
string Magnetar<T>::oneTrigger(string data, int _port, int flag)
{   
    string local_response{};
    if(data!="default"){
        local_response = data;
    } else{
        local_response = sender_data;
    }

    if (_port)
        setPort(_port);
    string response{};
    try
    {
        control = make_shared<T>(PORT);
        if (!control->create(_domain, type, protocol))
            throw "error al crear";
        control->setSesions(sesions);
        if (!control->on(PORT))
            throw "error al lanzar";
        control->send_data(local_response, flag);
        response = control->getResponse();
        control->close();
        control.reset();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return "error";
    }
    return response;
}

template <class T>
void Magnetar<T>::listenSesions()
{
    auto sender_loop = [&]() -> void
    {
        for (int i = 0; i < sesions; i++)
        {
            lock.lock();
            Response = oneTrigger(sender_data, PORT);
            connections.push_back(Response);
            lock.unlock();
        }
    };
    std::thread _sender(sender_loop);
    _sender.join();
}



#endif // !Magnetar_HPP