//
// Created by scythe on 5/07/23.
//

#ifndef DEPLOYOCEAN_MAIN_PROCESS_H
#define DEPLOYOCEAN_MAIN_PROCESS_H

#include <stdexcept>
#include <iostream>
#include <memory>
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
         std::condition_variable &condition;
         std::vector<std::shared_ptr<T>> &worker;
         std::mutex &macaco;

        inline void add_queue(std::shared_ptr<T> &base) {
            switch (next_register) {
                case 0:
                    worker.push_back(base);
                    {  condition.notify_all(); macaco.unlock(); }
                    // next_register++
                    break;
                default:
                    break;
            }
        }

    public:

        explicit pMain_t(std::shared_ptr<T> &conn, std::condition_variable &con,  std::vector<std::shared_ptr<T>> &_worker, std::mutex& _macaco) :
        connection(conn),
        condition(con),
        worker(_worker),
        macaco(_macaco)
        {
             next_register = enums::neo::eSize::DEF_REG;
        }

        inline  auto getMainProcess(uint16_t const &PORT, std::shared_ptr<HTTP_QUERY> &qProcess){
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