//
// Created by scythe on 5/07/23.
//

#ifndef MAIN_PROCESS_H
#define MAIN_PROCESS_H

#include <stdexcept>
#include <memory>
#include <array>
#include <condition_variable>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <unordered_map>

#include "../../utils/enums.h"
#include "../../processing/parameters/parameter_proccess.h"

#include "../../http/routes/routes.hpp"
#include "../../http/request/request.hpp"
#include "../../utils/nterminal.h"


constexpr int BUFFER = enums::neo::eSize::BUFFER;
constexpr int SESSION = enums::neo::eSize::SESSION;
constexpr int INIT_MAX_EVENTS = 10;

namespace workers {
typedef  std::unordered_map<string, std::unique_ptr<listen_routes>> RoutesMap;

    template<class T>
    class pMain_t {

        std::shared_ptr<T> &connection;
        std::mutex &macaco;

                       int epoll_fd;
        unsigned short int next_register;

        std::vector<epoll_event> events;
        RoutesMap  &routes;

       std::array<std::vector<std::shared_ptr<T>>, 0x3> &workers_base;
       std::array<std::condition_variable, 0x3> &conditions_base;

    public:

        explicit pMain_t(std::array<std::vector<std::shared_ptr<T>>, 0x3> &_workers_base, std::array<std::condition_variable, 0x3> &_conditions_base ,  std::shared_ptr<T> &conn, std::mutex& _macaco,  std::unordered_map<string, std::unique_ptr<listen_routes>>& _routes) :

        connection(conn),
        macaco(_macaco),

        epoll_fd(epoll_create1(0)),
        next_register(enums::neo::eSize::DEF_REG),

        events(std::vector<epoll_event>(INIT_MAX_EVENTS)),
        routes(_routes),

        workers_base(_workers_base ),
        conditions_base(_conditions_base)
        {}

        auto getMainProcess(std::shared_ptr<HTTP_QUERY> &qProcess){
            return [&]()->void {
                // creacion del socket
                connection->on();
                const int file_descriptor = connection->getDescription();
                constexpr auto nvalue = static_cast<socklen_t>(-1);

                if(Server::setNonblocking(file_descriptor) == MG_ERROR){
                    close(file_descriptor);
                }

                if (epoll_fd == -1) {
                    throw std::range_error(ND_EPOLL_RANGE);
                }

                epoll_event event{};
                event.events = EPOLLIN;
                event.data.fd = file_descriptor;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, file_descriptor, &event) == ND_NVALUE) {
                    close(epoll_fd);
                    throw std::range_error(ND_EPOLL_CTL);
                }

                while (enums::neo::eStatus::START){
                    try {
                        qProcess = make_shared<HTTP_QUERY>();

                        int notice = epoll_wait(epoll_fd, events.data(), INIT_MAX_EVENTS, ND_NVALUE);
                        if (notice == -1) {
                            terminal(ND_EPOLL_CERR, strerror(errno));
                            break;
                        }

                        for( int i = 0; i < notice; i++) {
                            if(events[i].data.fd == file_descriptor) {

                                sockaddr_in client_addr{};
                                socklen_t  client_adrr_len = sizeof(client_addr);
                                int client_file_descriptor = accept(file_descriptor, reinterpret_cast<sockaddr*>(&client_addr), &client_adrr_len);
                                if(client_adrr_len == nvalue) {
                                    continue;
                                }
                                Server::setNonblocking(client_file_descriptor);

                                event.events = EPOLLIN | EPOLLET;
                                event.data.fd = client_file_descriptor;
                                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_file_descriptor, &event) == ND_NVALUE) {
                                    terminal(ND_EPOLL_CERR, strerror(errno));
                                    close(client_file_descriptor);
                                    continue;
                                }
                            } else {

                                char buffer[DEF_BUFFER_SIZE] = {0};
                                if (const int bytes = recv(events[i].data.fd, buffer, sizeof(buffer), ND_OK); bytes == ND_NVALUE) {
                                    if (errno == EWOULDBLOCK) {
                                        continue;
                                    }
                                    terminal(ND_EPOLL_CERR, strerror(errno));
                                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                                    close(events[i].data.fd);
                                    continue;
                                } else if(bytes == ND_OK) {
                                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                                    close(events[i].data.fd);
                                } else {

                                    shared_ptr<T> base = std::make_shared<T>();

                                    base->setPort(connection->getPort());
                                    base->setSocketId(events[i].data.fd);
                                    base->setResponse(buffer);
                                    base->setEpollEvents(events);
                                    base->setEpollfd(epoll_fd);
                                    base->setNotices(notice);

                                    ExecuteRoute(base, qProcess, routes);

                                }
                            }
                        }
                    }

                    catch(const std::exception& e) {
                       terminal(e.what());
                        close(epoll_fd);
                        if(close(file_descriptor) < ND_OK)
                            throw std::range_error(ND_MAIN_THREAD);
                    }
                }
            };

        }

                 static void ExecuteRoute(const shared_ptr<T> &base,const shared_ptr<HTTP_QUERY>&qProcess, const RoutesMap& routes )  {

                                    string send_target = ERROR_GET;

                                     const string socket_response = base->getResponse();

                                        if (socket_response.empty()){
                                            throw std::range_error(ND_SOCKET_FAIL);
                                        }

                                    auto [type, route] = qProcess->route_refactor(socket_response);

                                        if(const auto itr = routes.find(route + type); itr != routes.end()) {

                                          if(not timeGuard(itr)) {

                                              const string parameters = type == GET_TYPE ?
                                                    qProcess->route_refactor_params_get(socket_response)
                                                   : qProcess->route_refactor_params(socket_response);

                                              auto [data, timekey] = itr->second->callbacks.execute(parameters,
                                              qProcess->headers_from(socket_response),
                                              itr->second->guardRouteMsg
                                              );

                                              if(timekey > ND_OK) {
                                                  itr->second->timekey = timekey;
                                                  itr->second->time_point = std::chrono::system_clock::now();
                                              }
                                              send_target = std::move(data);
                                          } else {
                                              send_target = itr->second->guardRouteMsg != nullptr ?
                                                             utility_t::guard_route(itr->second->timekey, itr->second->guardRouteMsg->data())
                                                            : utility_t::guard_route(itr->second->timekey);
                                          }
                                        }

                                        base->sendResponse(send_target);

                                        if (close(base->getDescription()) < enums::neo::eReturn::OK) {
                                            throw std::range_error(ND_SOCKET_CLOSE);
                                        }
            }



            static bool timeGuard(const RoutesMap::const_iterator & itr) {

                if(itr->second->timekey <= 0)
                    return false;
                if(itr->second->time_point == std::chrono::time_point<std::chrono::system_clock>())
                    return false;

                const auto now = std::chrono::system_clock::now();

                const std::chrono::duration<double> distance = now - itr->second->time_point;
                return distance.count() < itr->second->timekey;
            }


    };
}


#endif //MAIN_PROCESS_H
