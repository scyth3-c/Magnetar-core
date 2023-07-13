//
// Created by scythe on 12/07/23.
//

#ifndef MAGNETAR_CORE_WINSTANCES_H
#define MAGNETAR_CORE_WINSTANCES_H

#include <memory>
#include <string>
#include <vector>

#include "route_process.hpp"
#include "send_process.h"
#include "main_process.h"

namespace workers {

    using std::make_shared;
    using std::shared_ptr;
    using std::string;

    using workers::Worker_t;
    using workers::Send_t;
    using workers::pMain_t;
    using std::vector;

    template<class T>
    class wInstances {
    private:
        vector<std::shared_ptr<pMain_t<T>>> pMains;
        vector<std::shared_ptr<Send_t<T>>> Send_ts;
        vector<std::shared_ptr<Worker_t<T>>> Worker_ts;
    public:

        wInstances() = default;

        inline std::pair<shared_ptr<pMain_t<T>>, unsigned int> GenMainWorker(std::shared_ptr<T> &conn, std::condition_variable &con,  std::vector<std::shared_ptr<T>> &_worker, std::mutex& _macaco) {
            pMains.emplace_back(std::make_shared<pMain_t<T>>(conn, con, _worker, _macaco));
            return std::make_pair(pMains.back(), pMains.size()-1);
        }

        inline std::pair<shared_ptr<Send_t<T>>, unsigned int> GenSendWorker(std::vector<std::tuple<std::shared_ptr<T>, std::string>>& _worksend, std::condition_variable& _condition_response){
            Send_ts.emplace_back(std::make_shared<Send_t<T>>(_worksend, _condition_response));
            return  std::make_pair(Send_ts.back(), Send_ts.size()-1);
        }

        inline std::pair<shared_ptr<Worker_t<T>>, unsigned int> GenWorker(std::vector<std::shared_ptr<T>>& _core, std::shared_ptr<HTTP_QUERY>& _qProcess, std::condition_variable& _condition, std::vector<std::tuple<std::shared_ptr<T>, std::string>>& _worksend, std::condition_variable& _condition_response, std::vector<listen_routes>& _routes){
            Worker_ts.emplace_back(std::make_shared<Worker_t<T>>(_core, _qProcess, _condition, _worksend, _condition_response, _routes));
            return  std::make_pair(Worker_ts.back(), Worker_ts.size()-1);
        }

       [[maybe_unused]] inline shared_ptr<pMain_t<T>> getMainWorker(unsigned int const &index) {
            if (!index <= pMains.size()){
                return nullptr;
            }
                return pMains[index];
        }

        [[maybe_unused]] inline shared_ptr<Send_t<T>> getSendWorker(unsigned int const &index){
            if(!index <= Send_ts.size()) {
                return nullptr;
            }
            return Send_ts[index];
        }

        [[maybe_unused]] inline shared_ptr<Worker_t<T>> getWorker(unsigned int const &index){
            if(!index <= Worker_ts.size()) {
                return nullptr;
            }
            return Worker_ts[index];
        }

    };

} // workers

#endif //MAGNETAR_CORE_WINSTANCES_H
