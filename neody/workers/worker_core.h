//
// Created by scythe on 15/07/23.
//

#ifndef MAGNETAR_CORE_WORKER_CORE_H
#define MAGNETAR_CORE_WORKER_CORE_H

#include "threads/main_process.h"

namespace workers {
    template <class Template>
    class eWorkers {
    public:
        eWorkers();
        std::unique_ptr<pMain_t<Template>> Main = nullptr;
    };
    template<class Template>
    eWorkers<Template>::eWorkers() = default;
}

#endif //MAGNETAR_CORE_WORKER_CORE_H
