#ifndef NEODIMIO_HPP
#define NEODIMIO_HPP

#include "tcp/mgsockets.h"
#include "http/routes/routes.hpp"
#include "http/request/request.hpp"
#include "utils/enums.h"
#include "workers/worker_core.h"
#include "workers/wInstances.h"

#include <thread>
#include <memory>
#include <string>

using std::make_shared;
using std::make_unique;
using std::string;

using workers::eWorkers;
using workers::wInstances;

template <class T>
class Neody {

    shared_ptr<::eWorkers<T>> eWorkers = nullptr;
    shared_ptr<::wInstances<T>> wInstances = nullptr;

    std::unordered_map<string, std::unique_ptr<listen_routes>> routes;
    std::shared_ptr<T> tcpControl;
    std::shared_ptr<HTTP_QUERY> qProcess = nullptr;

    uint16_t PORT{enums::neo::eSize::DEF_PORT};

    std::mutex lock_process;

    void tcpInt();

public:
    [[maybe_unused]] explicit Neody(uint16_t port);
    explicit Neody();

    int http_response(const string&, xcallargs, const string&);
    [[maybe_unused]] int get(const string&,const xcallargs&);
    [[maybe_unused]] int post(const string&,const xcallargs&);
    [[maybe_unused]] int put(const string&, const xcallargs&);
    [[maybe_unused]] int deleteX(const string&, const xcallargs&);
    [[maybe_unused]] int patch(const string&, const xcallargs&);
    [[maybe_unused]] int head(const string&, const xcallargs&);
    [[maybe_unused]] int options(const string &, const xcallargs&);
    [[maybe_unused]] int link(const string&, const xcallargs&);
    [[maybe_unused]] int unlink(const string&, const xcallargs&);
    [[maybe_unused]] int purge(const string&, const xcallargs&);
    
    int setPort(uint16_t) noexcept;
    [[nodiscard]] [[maybe_unused]] inline uint16_t getPort() const noexcept{return PORT;};
    void listen();

};

template <class T>
[[maybe_unused]] Neody<T>::Neody(const uint16_t port) {
    if (port >= enums::neo::eSize::MIN_PORT) { PORT = port; }
    tcpInt();
}

template <class T>
Neody<T>::Neody() {
    tcpInt();
}

template <class T>

int Neody<T>::http_response(const string &route, xcallargs _funcs,const string& type) {
    try {
        routes[route + type] = make_unique<listen_routes>(route, std::move(_funcs), type);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return enums::neo::eReturn::ERROR;
    }
    return enums::neo::eReturn::OK;
}

template <class T>
[[maybe_unused]] int Neody<T>::get(const string& route,const xcallargs &_funcs){
    return http_response(route, _funcs, GET_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::post(const string& route,const xcallargs &_funcs) {
    return http_response(route, _funcs, POST_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::put(const string& route,const xcallargs &_funcs) {
    return http_response(route, _funcs, PUT_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::deleteX(const string& route,const xcallargs &_funcs) {
    return http_response(route, _funcs, DELETE_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::patch(const string& route,const xcallargs &_funcs) {
    return http_response(route, _funcs, PATCH_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::head(const string& route, const xcallargs &_funcs) {
    return http_response(route, _funcs, HEAD_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::options(const string& route, const xcallargs &_funcs) {
    return http_response(route, _funcs, OPTIONS_TYPE);
}
template <class T>
[[maybe_unused]]  int Neody<T>::link(const string& route, const xcallargs &_funcs) {
    return http_response(route, _funcs, LINK_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::unlink(const string& route,const xcallargs &_funcs) {
    return http_response(route, _funcs, UNLINK_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::purge(const string& route,const xcallargs &_funcs) {
    return http_response(route, _funcs, PURGE_TYPE);
}

template <class T>
void Neody<T>::listen() {
    std::thread _main(eWorkers->Main->getMainProcess(qProcess));
    _main.join();
}

template <class T>
int Neody<T>::setPort(const uint16_t _port) noexcept {
    if (_port >= enums::neo::eSize::MIN_PORT) {
        PORT = _port;
        if(tcpControl != nullptr) {
            tcpControl->setPort(PORT);
            return enums::neo::eReturn::OK;
        }
    }
    return enums::neo::eReturn::ERROR;
}

template<class T>
void Neody<T>::tcpInt() {

    eWorkers = make_shared<::eWorkers<T>>();
    wInstances = make_shared<::wInstances<T>>();
    eWorkers->Main =make_unique<workers::pMain_t<T>>(wInstances->workers, wInstances->conditions, tcpControl,  lock_process, routes);

    tcpControl = make_shared<T>();
    tcpControl->setBuffer(BUFFER);
    tcpControl->setPort(PORT);
    tcpControl->setSessions(SESSION);
}

typedef Neody<Server> Router;
typedef utility_t Convert;

#endif // NEODIMIO_HPP