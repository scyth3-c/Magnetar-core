//
// Created by scythe on 5/07/23.
//

#ifndef DEPLOYOCEAN_MAIN_PROCESS_H
#define DEPLOYOCEAN_MAIN_PROCESS_H

#include <stdexcept>
#include <iostream>
#include <memory>
#include <array>
#include <condition_variable>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include "../../utils/enums.h"
#include "../../utils/sysprocess.h"

constexpr int BUFFER = enums::neo::eSize::BUFFER;
constexpr int SESSION = enums::neo::eSize::SESSION;

namespace workers {

    template<class T>
    class pMain_t {
    private:

        char in[255];
        char out[255];

        int rsend, lwrite;

        struct sockaddr_in address{};
        struct timeval timeout = {40,0};
         fd_set  read_fd, write_fd;
         int NewSocket, max_fd;
         std::shared_ptr<T> &connection;
         unsigned short int next_register{};
         std::mutex &macaco;


       std::array<std::vector<std::shared_ptr<T>>, 0x3> &workers_base;
       std::array<std::condition_variable, 0x3> &conditions_base;

        inline void add_queue(shared_ptr<T> base) {

            if (next_register >= workers_base.size()) {
                        next_register = 0;
            }
            workers_base[next_register].push_back(base);
            {  conditions_base[next_register].notify_all(); macaco.unlock(); }
            next_register++;
        }

    public:

        explicit pMain_t(std::array<std::vector<std::shared_ptr<T>>, 0x3> &_workers_base, std::array<std::condition_variable, 0x3> &_conditions_base ,  std::shared_ptr<T> &conn, std::mutex& _macaco) :
        workers_base(_workers_base ),
        conditions_base(_conditions_base),
        connection(conn),
        macaco(_macaco)
        {
             next_register = enums::neo::eSize::DEF_REG;
        }

        inline  auto getMainProcess(std::shared_ptr<HTTP_QUERY> &qProcess){
            return [&]()->void {

                // creacion del socket
                connection->on();

                // temp
                memset(&in,0, 255);
                memset(&out, 0, 255);
                //temp



                while (enums::neo::eStatus::START){
                    try {

                        qProcess = make_shared<HTTP_QUERY>();

                        FD_ZERO(&read_fd);
                        FD_ZERO(&write_fd);
                        FD_SET(connection->getDescription(), &read_fd);
                        FD_SET(connection->getDescription(), &write_fd);
                        FD_SET(STDIN_FILENO, &read_fd);
                        FD_SET(STDIN_FILENO, &write_fd);

                        max_fd = connection->getDescription();

                        int activity = select(max_fd+ 1, &read_fd, &write_fd, (fd_set*)0, &timeout);
                        if (activity < 0){
                            static_assert("boom");
                            neosys::process::_wait(10);
                            continue;
                        }

                        NewSocket = accept(connection->getDescription(),
                                           (struct sockaddr *) &connection->address,
                                           (socklen_t *) &connection->address);


                       if (NewSocket < 0){
                            neosys::process::_wait(10);
                            continue;
                        }

                        if(Server::setNonblocking(NewSocket) == MG_ERROR){
                            close(NewSocket);
                            neosys::process::_wait(10);
                            continue;
                        }
                            if(FD_ISSET(NewSocket, &read_fd)){
                                FD_CLR(NewSocket, &read_fd);
                                memset(&in, 0, 255);
                                rsend = recv(NewSocket, in, 255, 0);
                                if (rsend <= 0){
                                    close(NewSocket);
                                    break;
                                } else if(in[0] != '\0') {
                                    std::cout << "datos:" << in << std::endl;
                                }
                                if (FD_ISSET(STDIN_FILENO, &read_fd)){
                                    fgets(out, 255, stdin);
                                    if (FD_ISSET(STDIN_FILENO, &write_fd)){
                                        FD_CLR(NewSocket, &write_fd);
                                        send(NewSocket, out, 255, 0);
                                        memset(&out, 0, 255);
                                    }
                                }
//                                shared_ptr<T> base = std::make_shared<T>();
//                                base->setPort(connection->getPort());
//                                base->setSocketId(NewSocket);
//                                base->getResponseProcessing();
//
//                                std::cout << base->getSocketId() << std::endl;
//
//                                add_queue(base);

                            }

                    }

                    catch(const std::exception& e) {
                        std::cerr << e.what() << '\n';
                        if(close(connection->getDescription()) < 0)
                         throw std::range_error("error al cerrar conexion principal");
                    }
                }
                if(close(connection->getDescription()) < 0)
                    std::range_error("error al cerrar conexion principal");
            };
        }
    };
}


#endif //DEPLOYOCEAN_MAIN_PROCESS_H
