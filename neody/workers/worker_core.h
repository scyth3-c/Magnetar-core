//
// Created by scythe on 15/07/23.
//

#ifndef MAGNETAR_CORE_WORKER_CORE_H
#define MAGNETAR_CORE_WORKER_CORE_H

#include "../HTTP/routes/routes.hpp"
#include "../processing/parameters/parameter_proccess.h"

#include "threads/main_process.h"
#include "threads/send_process.h"
#include "threads/route_process.hpp"


namespace workers {
    template <class Template>
    class eWorkers {
    private:

    public:
        eWorkers();
        ~eWorkers();

        Worker_t<Template> *Worker = nullptr;
        Send_t<Template> *Send = nullptr;
        pMain_t<Template> *Main = nullptr;
    };

    template<class Template>
    eWorkers<Template>::eWorkers() = default;

    template<class Template>
    eWorkers<Template>::~eWorkers() {
        delete Main;
        delete Send;
        delete Worker;
    }
}

#endif //MAGNETAR_CORE_WORKER_CORE_H
