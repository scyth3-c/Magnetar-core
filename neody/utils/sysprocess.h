//
// Created by scythe on 5/07/23.
//
// procesos del sistema

#ifndef NEODY_SYSPROCESS_H
#define NEODY_SYSPROCESS_H

#include <chrono>
#include <thread>

namespace neosys {

    class process {
    public:
        [[maybe_unused]] static inline void _wait(int milis)  { std::this_thread::sleep_for(std::chrono::milliseconds(milis)); }
        /* procesos del sistema aqui */
    };

}

#endif //NEODY_SYSPROCESS_H
