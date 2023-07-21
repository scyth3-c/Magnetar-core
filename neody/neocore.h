#ifndef NEODIMIO_HPP
#define NEODIMIO_HPP

#include "TCP-IP/mgsockets.h"
#include "HTTP/routes/routes.hpp"
#include "HTTP/request/request.hpp"
#include "utils/enums.h"
#include "utils/sysprocess.h"
#include "workers/worker_core.h"
#include "workers/wInstances.h"

#include <thread>
#include <memory>

#include <string>
#include <vector>

using std::make_shared;
using std::string;

using workers::eWorkers;
using workers::wInstances;

template <class T>
class Neody {
private:

    // Control de instancias
    ::eWorkers<T> *eWorkers = nullptr;
    ::wInstances<T> * wInstances = nullptr;

    std::vector<listen_routes> routes;
    std::shared_ptr<T> tcpControl;
    std::shared_ptr<HTTP_QUERY> qProcess = nullptr;

    uint16_t PORT{enums::neo::eSize::DEF_PORT};

    std::mutex lock_push;
    std::mutex lock_send;
    std::mutex lock_process;

    void tcpInt();

public:
    [[maybe_unused]] explicit Neody(uint16_t port);
    explicit Neody();
    ~Neody(){ delete eWorkers; }

    int http_response(string, xcallargs, string optional);
    [[maybe_unused]] int get(string, xcallargs);
    [[maybe_unused]] int post(string, xcallargs);
    [[maybe_unused]] int put(string, xcallargs);
    [[maybe_unused]] int deleteX(string, xcallargs);
    [[maybe_unused]] int patch(string, xcallargs);
    [[maybe_unused]] int head(string, xcallargs);
    [[maybe_unused]] int options(string, xcallargs);
    [[maybe_unused]] int link(string, xcallargs);
    [[maybe_unused]] int unlink(string, xcallargs);
    [[maybe_unused]] int purge(string, xcallargs);
    
    int setPort(uint16_t) noexcept;
    [[nodiscard]] [[maybe_unused]] inline uint16_t getPort() const noexcept{return PORT;};
    void listen();

};

template <class T>
[[maybe_unused]] Neody<T>::Neody(uint16_t port) {
    if (port >= enums::neo::eSize::MIN_PORT) { PORT = port; }
    tcpInt();
}

template <class T>
Neody<T>::Neody() {
    tcpInt();
}

template <class T>

int Neody<T>::http_response(string _xRoute, xcallargs _funcs, string optional_type) {
    try {
        routes.emplace_back(std::move(_xRoute), std::move(_funcs), std::move(optional_type));
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return enums::neo::eReturn::ERROR;
    }
    return enums::neo::eReturn::OK;
}

template <class T>
[[maybe_unused]] int Neody<T>::get(string _xRoute, xcallargs _funcs){
    return http_response(_xRoute, _funcs, GET_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::post(string _xRoute, xcallargs _funcs) {
    return http_response(_xRoute, _funcs, POST_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::put(string _xRoute, xcallargs _funcs) {
    return http_response(_xRoute, _funcs, PUT_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::deleteX(string _xRoute, xcallargs _funcs) {
    return http_response(_xRoute, _funcs, DELETE_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::patch(string _xRoute, xcallargs _funcs) {
    return http_response(_xRoute, _funcs, PATCH_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::head(string _xRoute, xcallargs _funcs) {
    return http_response(_xRoute, _funcs, HEAD_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::options(string _xRoute, xcallargs _funcs) {
    return http_response(_xRoute, _funcs, OPTIONS_TYPE);
}
template <class T>
[[maybe_unused]]  int Neody<T>::link(string _xRoute, xcallargs _funcs) {
    return http_response(_xRoute, _funcs, LINK_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::unlink(string _xRoute, xcallargs _funcs) {
    return http_response(_xRoute, _funcs, UNLINK_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::purge(string _xRoute, xcallargs _funcs) {
    return http_response(_xRoute, _funcs, PURGE_TYPE);
}

template <class T>
void Neody<T>::listen() {

    std::thread _main(eWorkers->Main->getMainProcess(qProcess));
    std::thread _response(eWorkers->Send->getSendProcess(lock_send));

    std::thread _worker1(eWorkers->Worker->getWorker(lock_process, lock_send, lock_push, wInstances->workers[0], wInstances->conditions[0]));
    std::thread _worker2(eWorkers->Worker->getWorker(lock_process, lock_send, lock_push, wInstances->workers[1], wInstances->conditions[1]));
    std::thread _worker3(eWorkers->Worker->getWorker(lock_process, lock_send, lock_push, wInstances->workers[2], wInstances->conditions[2]));


    _main.join();
    _response.join();
    _worker1.join();
}


template <class T>
int Neody<T>::setPort(uint16_t _port) noexcept {
    if (_port >= enums::neo::eSize::MIN_PORT)
        PORT = _port;
    else
        return enums::neo::eReturn::ERROR;
    return enums::neo::eReturn::OK;
}

template<class T>
void Neody<T>::tcpInt() {

    eWorkers = new ::eWorkers<T>();
    wInstances = new ::wInstances<T>();

    eWorkers->Main = new workers::pMain_t<T>(wInstances->workers, wInstances->conditions, tcpControl,  lock_process);
    eWorkers->Send = new workers::Send_t<T>(wInstances->worker_send, wInstances->condition_response);
    eWorkers->Worker = new workers::Worker_t<T>(qProcess, wInstances->worker_send, wInstances->condition_response, routes);

    tcpControl = make_shared<T>();
    tcpControl->setBuffer(BUFFER);
    tcpControl->setPort(PORT);
    tcpControl->setSessions(SESSION);
}

typedef Neody<Server> Router;

#endif // NEODIMIO_HPP