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
#include "../../utils/enums.h"

constexpr int BUFFER = enums::neo::eSize::BUFFER;
constexpr int SESSION = enums::neo::eSize::SESSION;

namespace workers {

    template<class T>
    class pMain_t {
    private:
         std::shared_ptr<T> &connection = nullptr;
         unsigned short int next_register{};
         std::mutex &macaco;

       std::array<std::vector<std::shared_ptr<T>>, 0x3> &workers_base;
       std::array<std::condition_variable, 0x3> &conditions_base;

        inline void add_queue(std::shared_ptr<T> &base) {
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

                while (enums::neo::eStatus::START){
                    try {

                        qProcess = make_shared<HTTP_QUERY>();

                        if (!connection->create())
                            std::range_error("error al crear");

                        if (!connection->on())
                            std::range_error("error al lanzar");

                        if(close(connection->getDescription()) < 0)
                            std::range_error("error al cerrar conexion principal");

                        add_queue(connection);
                    }
                    catch(const std::exception& e) {
                        std::cerr << e.what() << '\n';
                    }
                }
            };
        }
    };
}


#endif //DEPLOYOCEAN_MAIN_PROCESS_H
