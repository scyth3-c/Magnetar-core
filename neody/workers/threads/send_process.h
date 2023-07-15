//
// Created by scythe on 5/07/23.
//

#ifndef DEPLOYOCEAN_SEND_PROCESS_H
#define DEPLOYOCEAN_SEND_PROCESS_H

#include <condition_variable>
#include <mutex>
#include <vector>

#include "../../utils/sysprocess.h"
#include "../../utils/enums.h"

namespace workers {

    template <class T>
    class Send_t{
    private:
        std::condition_variable  &condition_response;
        std::vector<std::tuple<std::shared_ptr<T>, std::string>> &worksend;
    public:
        explicit Send_t(std::vector<std::tuple<std::shared_ptr<T>, std::string>>& _worksend, std::condition_variable& _condition_response) :
        worksend(_worksend),
        condition_response(_condition_response) {}

        inline auto getSendProcess(std::mutex &victor){
            return [&]()->void {
                while(enums::neo::eStatus::START){
                    {
                        std::unique_lock<std::mutex> lock(victor);
                        condition_response.wait(lock);
                    }

                    if(!worksend.empty()){

                        for(auto it = worksend.begin(); it != worksend.end();){
                            try {
                                auto &[sender, data] = *it;
                                sender->sendResponse(data);
                                if (close(sender->getDescription()) < enums::neo::eReturn::OK) {
                                    throw std::range_error("error al tratar de cerrar el socket");
                                }
                                it = worksend.erase(it);

                            } catch (std::exception &e) {
                                it = worksend.erase(it);
                            }
                        }
                    }
                    neosys::process::_wait(10);
                }
            };
        }
    };
}
#endif //DEPLOYOCEAN_SEND_PROCESS_H
