//
// Created by scythe on 5/07/23.
//

#ifndef MAIN_PROCESS_H
#define MAIN_PROCESS_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <array>
#include <condition_variable>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../../utils/enums.h"
#include "../../processing/parameters/parameter_proccess.h"

#include "../../http/routes/routes.hpp"
#include "../../http/request/request.hpp"


constexpr int BUFFER = enums::neo::eSize::BUFFER;
constexpr int SESSION = enums::neo::eSize::SESSION;
constexpr int INIT_MAX_EVENTS = 10;

namespace workers {

    template<class T>
    class pMain_t {

        std::shared_ptr<T> &connection;
        std::mutex &macaco;

                       int epoll_fd;
        unsigned short int next_register;

        std::vector<epoll_event> events;
        std::vector<listen_routes> &routes;

       std::array<std::vector<std::shared_ptr<T>>, 0x3> &workers_base;
       std::array<std::condition_variable, 0x3> &conditions_base;

    public:

        explicit pMain_t(std::array<std::vector<std::shared_ptr<T>>, 0x3> &_workers_base, std::array<std::condition_variable, 0x3> &_conditions_base ,  std::shared_ptr<T> &conn, std::mutex& _macaco, std::vector<listen_routes>& _routes) :
        connection(conn),
        macaco(_macaco),
        epoll_fd(epoll_create1(0)),
        workers_base(_workers_base ),
        conditions_base(_conditions_base),
        routes(_routes)
        {
             next_register = enums::neo::eSize::DEF_REG;
             events = std::vector<epoll_event>(INIT_MAX_EVENTS);
        }

        inline  auto getMainProcess(std::shared_ptr<HTTP_QUERY> &qProcess){
            return [&]()->void {

                // creacion del socket
                connection->on();
                int file_descriptor = connection->getDescription();

                if(Server::setNonblocking(file_descriptor) == MG_ERROR){
                    close(file_descriptor);
                }

                if (epoll_fd == -1) {
                    throw std::range_error("epoll_create1");
                }

                epoll_event event;
                event.events = EPOLLIN;
                event.data.fd = file_descriptor;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, file_descriptor, &event) == -1) {
                    close(epoll_fd);
                    throw std::range_error("epoll_ctl");
                }

                while (enums::neo::eStatus::START){
                    try {
                        qProcess = make_shared<HTTP_QUERY>();

                        int notice = epoll_wait(epoll_fd, events.data(), INIT_MAX_EVENTS, -1);
                        if (notice == -1) {
                            std::cerr << "epoll_wait: "<< strerror(errno) << std::endl;
                            break;
                        }

                        for( int i = 0; i < notice; i++) {
                            if(events[i].data.fd == file_descriptor) {

                                sockaddr_in client_addr;
                                socklen_t  client_adrr_len = sizeof(client_addr);
                                int client_file_descriptor = accept(file_descriptor, reinterpret_cast<sockaddr*>(&client_addr), &client_adrr_len);
                                if(client_adrr_len == -1) {
                                    continue;
                                }
                                Server::setNonblocking(client_file_descriptor);

                                event.events = EPOLLIN | EPOLLET;
                                event.data.fd = client_file_descriptor;
                                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_file_descriptor, &event) == -1) {
                                    std::cerr << "epoll_ctl: client:  "<< strerror(errno) << std::endl;
                                    close(client_file_descriptor);
                                    continue;
                                }
                            } else {
                                char buffer[DEF_BUFFER_SIZE] = {0};
                                int bytes = recv(events[i].data.fd, buffer, sizeof(buffer), 0);

                                if (bytes == -1) {
                                    if (errno == EWOULDBLOCK) {
                                        continue;
                                    }
                                    std::cerr << "epoll_ctl: client:  "<< strerror(errno) << std::endl;
                                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                                    close(events[i].data.fd);
                                    continue;

                                } else if(bytes == 0) {
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

                                    std::pair<string, string>   actual_route;

                                    string     send_target;
                                    string      parameters{};
                                    bool        cantget = true;

                                        string socket_response = base->getResponse();

                                        if (socket_response.empty()){
                                            throw std::range_error("FAILED TO READ REQUEST");
                                        }

                                        actual_route = qProcess->route_refactor(socket_response);

                                        for (auto &it : routes) {

                                            if (it.route.getType() == actual_route.first && it.route.getName() == actual_route.second) {

                                                parameters = actual_route.first == GET_TYPE ?
                                                     qProcess->route_refactor_params_get(socket_response)
                                                    : qProcess->route_refactor_params(socket_response);

                                                send_target = it.callbacks.execute(parameters,
                                                qProcess->headers_from(socket_response)
                                                );
                                                cantget = false;
                                                break;
                                            }
                                        }

                                        base->sendResponse(cantget ? ERROR_GET : send_target);
                                        if (close(base->getDescription()) < enums::neo::eReturn::OK) {
                                            throw std::range_error("error al tratar de cerrar el socket");
                                        }

                                }
                            }
                        }
                    }

                    catch(const std::exception& e) {
                        std::cerr << e.what() << '\n';
                        close(epoll_fd);
                        if(close(file_descriptor) < 0)
                         throw std::range_error("main_process");
                    }
                }
            };
        }
    };
}


#endif //MAIN_PROCESS_H
