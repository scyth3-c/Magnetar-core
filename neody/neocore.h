#ifndef NEODIMIO_HPP
#define NEODIMIO_HPP

#include "TCP-IP/mgsockets.h"
#include "utils/notify.h"

#include "HTTP/routes/routes.hpp"
#include "HTTP/request/request.hpp"

#include "processing/parameters/parameter_proccess.h"
#include "workers/route_process.hpp"
#include "workers/send_process.h"
#include "workers/main_process.h"

#include "utils/enums.h"
#include "utils/sysprocess.h"


#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <memory>

#include <string>
#include <vector>
#include <stdexcept>
#include <queue>
#include <atomic>
#include <tuple>
#include <ios>

using std::make_shared;
using std::string;

using workers::Worker_t;
using workers::Send_t;
using workers::pMain_t;


template <class T>
class Neody {
private:

    Worker_t<T> *Worker_maestro = nullptr;
    Send_t<T> *Send_maestro = nullptr;
    pMain_t<T> *Main_maestro = nullptr;

    std::vector<std::shared_ptr<T>> worker_one;
    std::vector<std::tuple<std::shared_ptr<T>, string>> worker_send;
    std::vector<listen_routes> routes;

    std::shared_ptr<T> tcpControl;
    std::shared_ptr<HTTP_QUERY> qProcess = nullptr;

    std::mutex victoria;
    std::mutex victor;
    std::mutex macaco;

    std::condition_variable condition_one;
    std::condition_variable condition_response;
    std::condition_variable condition_access;

    uint16_t PORT{enums::neo::eSize::DEF_PORT};

    void tcpInt();

public:
    [[maybe_unused]] explicit Neody(uint16_t port);
    explicit Neody();

    int http_response(string, _callbacks, string optional);
    [[maybe_unused]] int get(string, _callbacks);
    [[maybe_unused]] int post(string, _callbacks);
    [[maybe_unused]] int put(string, _callbacks);
    [[maybe_unused]] int deleteX(string, _callbacks);
    [[maybe_unused]] int patch(string, _callbacks);
    [[maybe_unused]] int head(string, _callbacks);
    [[maybe_unused]] int options(string, _callbacks);
    [[maybe_unused]] int link(string, _callbacks);
    [[maybe_unused]] int unlink(string, _callbacks);
    [[maybe_unused]] int purge(string, _callbacks);
    
    int setPort(uint16_t) noexcept;
    [[nodiscard]] inline uint16_t getPort() const noexcept{return PORT;};
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

int Neody<T>::http_response(string _xRoute, _callbacks _funcs, string optional_type) {
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
[[maybe_unused]] int Neody<T>::get(string _xRoute, _callbacks _funcs){
    return http_response(_xRoute, _funcs, GET_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::post(string _xRoute, _callbacks _funcs) {
    return http_response(_xRoute, _funcs, POST_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::put(string _xRoute, _callbacks _funcs) {
    return http_response(_xRoute, _funcs, PUT_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::deleteX(string _xRoute, _callbacks _funcs) {
    return http_response(_xRoute, _funcs, DELETE_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::patch(string _xRoute, _callbacks _funcs) {
    return http_response(_xRoute, _funcs, PATCH_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::head(string _xRoute, _callbacks _funcs) {
    return http_response(_xRoute, _funcs, HEAD_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::options(string _xRoute, _callbacks _funcs) {
    return http_response(_xRoute, _funcs, OPTIONS_TYPE);
}
template <class T>
[[maybe_unused]]  int Neody<T>::link(string _xRoute, _callbacks _funcs) {
    return http_response(_xRoute, _funcs, LINK_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::unlink(string _xRoute, _callbacks _funcs) {
    return http_response(_xRoute, _funcs, UNLINK_TYPE);
}
template <class T>
[[maybe_unused]] int Neody<T>::purge(string _xRoute, _callbacks _funcs) {
    return http_response(_xRoute, _funcs, PURGE_TYPE);
}

template <class T>
void Neody<T>::listen() {

    auto Main = Main_maestro->getMainProcess(PORT, qProcess);
    auto worker = Worker_maestro->getWorker(macaco, victor, victoria);
    auto Sender = Send_maestro->getSendProcess(victor);

    std::thread _worker(worker);
    std::thread _response(Sender);
    std::thread _main(Main);

    _main.join();
    _response.join();
    _worker.join();
}


template <class T>
int Neody<T>::setPort(uint16_t _port) noexcept {
    if (_port >= enums::neo::eSize::MIN_PORT) {
        PORT = _port;
    }
    else {
        return enums::neo::eReturn::ERROR;
    }
    return enums::neo::eReturn::OK;
}

template<class T>
void Neody<T>::tcpInt() {

    Main_maestro = new pMain_t(tcpControl, condition_one, worker_one, macaco);
    Worker_maestro = new Worker_t(worker_one, qProcess, condition_one, worker_send, condition_response, routes);
    Send_maestro = new Send_t(worker_send, condition_response);

    tcpControl = make_shared<T>();
    tcpControl->setBuffer(BUFFER);
    tcpControl->setPort(PORT);
    tcpControl->setSessions(SESSION);
}

typedef Neody<Server> Router;

#endif // NEODIMIO_HPP