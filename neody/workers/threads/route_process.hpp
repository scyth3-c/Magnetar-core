#ifndef ROUTE_PROCESS
#define ROUTE_PROCESS

#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include "../../HTTP/routes/routes.hpp"
#include "../../utils/enums.h"

using std::string;

namespace workers {

    template <class T>
    class Worker_t {
    private:

        std::vector<listen_routes> &routes;
        std::vector<std::shared_ptr<T>> &core;
        std::shared_ptr<HTTP_QUERY> qProcess = nullptr;
        std::condition_variable  &condition;
        std::condition_variable  &condition_response;
        std::vector<std::tuple<std::shared_ptr<T>, std::string>> &worksend;

    public:
        Worker_t(std::vector<std::shared_ptr<T>>& _core, std::shared_ptr<HTTP_QUERY>& _qProcess, std::condition_variable& _condition, std::vector<std::tuple<std::shared_ptr<T>, std::string>>& _worksend, std::condition_variable& _condition_response, std::vector<listen_routes>& _routes)
                : core(_core),
                  qProcess(_qProcess),
                  condition(_condition),
                  worksend(_worksend),
                  condition_response(_condition_response),
                  routes(_routes)
        {
        }
        ~Worker_t() = default;

        inline auto getWorker(std::mutex &macaco, std::mutex &victor, std::mutex &victoria){

            return [&]()->void{
                while(enums::neo::eStatus::START){

                    {
                        std::unique_lock<std::mutex> lock(macaco);
                        condition.wait(lock);
                    }

                    if(!core.empty()) {

                        qProcess = make_shared<HTTP_QUERY>();
                        std::pair<string, string>   actual_route;

                        string     send_target;
                        string      parametros;
                        bool        cantget = true;

                        for(auto server=core.begin(); server != core.end();){

                            shared_ptr<T> &control = *server;
                            string socket_response = control->getResponse();

                            if (socket_response.empty()){
                                throw std::range_error("FAILED TO READ REQUEST, WAIT FEWS SECS BEFORE STARTING AGAIN");
                            }

                            actual_route = qProcess->route_refactor(socket_response);

                            for (auto &it : routes) {

                                if (it.route.getType() == actual_route.first && it.route.getName() == actual_route.second) {

                                    if (actual_route.first == GET_TYPE){
                                        parametros = qProcess->route_refactor_params_get(socket_response);
                                    } else{
                                        parametros = qProcess->route_refactor_params(socket_response);
                                    }

                                    send_target = it.callbacks.execute(parametros);

                                    cantget = false;

                                    break;

                                }
                            }

                            std::string sendy = cantget ? ERROR_GET : send_target;
                            auto data = std::make_tuple(control, sendy);

                            std::lock_guard<std::mutex> guard(victoria);
                            worksend.push_back(data);
                            {  condition_response.notify_all(); victor.unlock(); }

                            server = core.erase(server);
                        }
                    }

                }
            };
        }
    };

};

#endif // !ROUTE_PROCESS